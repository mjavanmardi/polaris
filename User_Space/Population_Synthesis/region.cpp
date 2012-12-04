#include "region.h"

Region::Region(double ID, vector<int> &dim_sizes) : Zone(ID, dim_sizes)
{
	_zones.clear();
	_randseed = 1;
	Rand = Prob_Generator();
	Rand.Set_Seed(_randseed);
}

Region::Region(const Region& obj) : Zone(obj)
{
	_zones.clear();
	for (vector<Zone>::const_iterator z = obj._zones.begin(); z!= obj._zones.end(); z++)
	{
		_zones.push_back(*z);
	}
}

void Region::add_zone(Zone &z)
{
	_zones.push_back(z);

	for (int i=0; i<Region::_dim_sizes.size(); i++)
	{
		for (int j=0; j<Region::_marginals[i].size(); j++) marginal(i,j) += z.marginal(i,j);
	}
}

void Region::add_sample_data(Linker &link, File_Reader &fr)
	{
		double x;

		vector<int> index = operator()(true);
		for (int i=0; i<_ndim; i++)
		{
			fr.Get_Data<double>(x,link.get_pums_column(i));
			index[i] = link.find_index_in_dimension(i,x);
		}

		uint ind = Region::get_index(index);
		double sample_id;
		double weight;
		fr.Get_Data<double>(sample_id,link.sample_id_col);
		fr.Get_Data<double>(weight, link.sample_weight_col);
		vector<double> data;
		fr.Get_Data<double>(data,link.get_pums_data_cols());

		Pop_Unit P = Pop_Unit(sample_id,ind,weight,data);

		hash_map<uint, vector<Pop_Unit>>::iterator i;
		if ((i = _sample.find(ind)) != _sample.end())
		{
			i->second.push_back(P);
		}
		else
		{
			vector<Pop_Unit> v;
			v.push_back(P);
			_sample.insert(pair<uint,vector<Pop_Unit>>(ind,v));
		}

		operator[](index) += weight;
	}

void Region::reweight_sample()
{
	// Divide each population unit in sample by sum of weights of all population units in the same cell
	for (hash_map<uint, vector<Pop_Unit>>::iterator itr = _sample.begin(); itr != _sample.end(); itr++)
	{
		for (vector<Pop_Unit>::iterator p=itr->second.begin(); p!= itr->second.end(); p++)
		{
			p->normalize(_data[itr->first]);
		}
	}
}

bool Region::Synthesize(double TOL, int MAX_ITER, int MAX_SELECT_ITER)
{
	_randseed = (uint)(_sample.size() * time(NULL));
	Rand.Set_Seed(_randseed);

	// normalize sample weights
	reweight_sample();

	// regional IPF -> used to seed Zonal IPF.
	IPF(TOL,MAX_ITER);

	// zonal synthesis process
	for (vector<Zone>::iterator zone_itr=_zones.begin(); zone_itr != _zones.end(); zone_itr++)
	{
		//===============================================================================================================
		// RUN IPF PROCESS
		//---------------------------------------------------------------------------------------------------------------
		zone_itr->Copy(*this);

		zone_itr->IPF(TOL, MAX_ITER);

		//===============================================================================================================
		// DO SAMPLE SELECTION PROCESS
		//---------------------------------------------------------------------------------------------------------------
		zone_itr->Select_HH(MAX_SELECT_ITER, _sample, Rand);

	}
	return true;
}

void Region::start_synthesis(double TOL, int MAX_ITER, int MAX_SELECT_ITER)
{
	_thread = boost::thread(&Region::Synthesize, this, TOL, MAX_ITER, MAX_SELECT_ITER);
}

bool Region::write(File_Writer &fw, File_Writer &fw_sample)
{
	stringstream ss;
	ss<<endl<<"Region: "<<fixed<<_id<<endl;
	fw.Write(ss);

	vector<Zone>::iterator zone_itr;

	for (zone_itr = _zones.begin(); zone_itr != _zones.end(); zone_itr++)
	{
		zone_itr->write(fw);
		zone_itr->write_sample(fw_sample);
	}

	_zones.clear();

	return true;
}

bool Region::write()
{
	cout<<endl<<"Region: "<<_id<<endl;

	vector<Zone>::iterator zone_itr;

	for (zone_itr = _zones.begin(); zone_itr != _zones.end(); zone_itr++)
	{
		zone_itr->write();
	}

	cout<<endl;
	return true;
}