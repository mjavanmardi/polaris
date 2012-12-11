
#include "Application_Includes.h"


// notice how components are no longer doubly defined, they are defined once in MasterType now
struct MasterType
{
	typedef Polaris_Component<RNG_Components::Implementations::RngStream_Implementation,MasterType,Data_Object> RNG;

	typedef m_array<int>	matrix_int;
	typedef m_array<double> matrix_double;
	typedef Polaris_Component<PopSyn::Implementations::Synthesis_Zone_Implementation, MasterType, Data_Object> zone;
	typedef Polaris_Component<PopSyn::Implementations::Synthesis_Region_Implementation, MasterType, Data_Object> region;
	typedef Polaris_Component<PopSyn::Implementations::IPF_Solver_Settings_Implementation, MasterType, Data_Object> IPF_Solver_Settings;
	typedef Polaris_Component<PopSyn::Implementations::ADAPTS_Population_Unit_Implementation, MasterType, Data_Object> pop_unit;
};




int main()
{
	ofstream out;
	out.open("test.xls",ios_base::out);
	//ostream& out = cout;
	out << "test";


	// CREATE RNG for later use
	define_component_interface(Rand_Interface,MasterType::RNG,RNG_Prototype,NULLTYPE);
	Rand_Interface* rand = (Rand_Interface*)Allocate<MasterType::RNG>();
	rand->Initialize<double>(50.0,0.0,100.0);

	// IPF Solver Settings
	define_component_interface(solver_itf,MasterType::IPF_Solver_Settings,PopSyn::Prototypes::Solver_Settings_Prototype,NULLTYPE);
	solver_itf* solver = (solver_itf*)Allocate<MasterType::IPF_Solver_Settings>();
	solver->Iterations<int>(100);
	solver->Tolerance<double>(0.05);

	// Create analysis region
	define_component_interface(region_itf,MasterType::region, PopSyn::Prototypes::Synthesis_Region_Prototype,NULLTYPE);
	define_container_and_value_interface(synthesis_zones_itf,zone_itf,region_itf::get_type_of(Synthesis_Zone_Collection),Containers::Random_Access_Sequence_Prototype, PopSyn::Prototypes::Synthesis_Zone_Prototype,NULLTYPE);
	region_itf* my_region = (region_itf*)Allocate<MasterType::region>();
	my_region->Solver_Settings(solver);
	synthesis_zones_itf& zones_collection = my_region->Synthesis_Zone_Collection<synthesis_zones_itf&>();

	// Create analysis zone
	zone_itf* my_zone = (zone_itf*)Allocate<MasterType::zone>();
	my_zone->Solver_Settings(solver);
	zones_collection.push_back(my_zone);
	
	//-----------------------------------------------------------------------------------------
	// Get reference to the region distribution, marginals and sample, and their iterators
	define_simple_container_interface(joint_itf,MasterType::region::Target_Joint_Distribution_type,Multidimensional_Random_Access_Array_Prototype, MasterType::zone::Target_Joint_Distribution_type::unqualified_value_type ,NULLTYPE);
	define_simple_container_interface(marginal_itf,MasterType::region::Target_Marginal_Distribution_type,Multidimensional_Random_Access_Array_Prototype, MasterType::zone::Target_Marginal_Distribution_type::unqualified_value_type ,NULLTYPE);
	define_container_and_value_interface(sample_itf, _pop_unit_itf, MasterType::region::Sample_Data_type, Associative_Container_Prototype, PopSyn::Prototypes::Population_Unit_Prototype ,NULLTYPE);
	define_component_interface(pop_unit_itf,MasterType::pop_unit,PopSyn::Prototypes::Population_Unit_Prototype,NULLTYPE);
	
	joint_itf& dist = my_region->Target_Joint_Distribution<joint_itf&>();
	joint_itf::iterator itr = dist.begin();
	marginal_itf& marg = my_region->Target_Marginal_Distribution<marginal_itf&>();
	typedef marginal_itf::index_type index_type;
	sample_itf& sample = my_region->Sample_Data<sample_itf&>();
	sample_itf::iterator sample_itr;

	//-----------------------------------------------------------------------------------------
	// Initialize the distribution and marginals
	joint_itf::index_type index;
	index.push_back(3); index.push_back(4); index.push_back(4);
	dist.resize(index,1.0);
	marg.resize(index,0);

	//-----------------------------------------------------------------------------------------
	// fill the distribution with randomly generated samples
	for (uint i=0; i<200; ++i)
	{
		pop_unit_itf* p = (pop_unit_itf*)Allocate<MasterType::pop_unit>();
		p->Index<uint>((uint)rand->Next_Rand<double>()*47.0);
		p->Weight<double>(rand->Next_Rand<double>());
		p->ID<uint>(p->Index<uint>());
		dist[p->Index<uint>()] += p->Weight<double>();
		
		//pair<uint, pop_unit_itf*>(1,p);
		sample.insert(pair<uint,MasterType::pop_unit*>(p->Index<uint>(),(MasterType::pop_unit*)p));
		sample_itf::Test_Type t;
		int x=1;
	}
	for (sample_itr = sample.begin(); sample_itr != sample.end(); ++sample_itr)
	{
		double sum = 0;
		/*for (sample_vector_itr = sample_itr->second.begin(); sample_vector_itr != sample_itr->second.end(); ++ sample_vector_itr) sum += ((pop_unit_itf*)*sample_vector_itr)->Weight<double>();
		for (sample_vector_itr = sample_itr->second.begin(); sample_vector_itr != sample_itr->second.end(); ++ sample_vector_itr) ((pop_unit_itf*)*sample_vector_itr)->Normalize_Weight<double>(sum);*/
	}

	out <<endl<<"Normalized Joint Distribution: " << endl;
	dist.write(out);

	//-----------------------------------------------------------------------------------------
	// Set the marginals to random values for each dimension
	for (uint d=0; d< dist.num_dimensions(); ++d)
	{
		double sum = 0;
		// set the last value for each dimension/index pair, i.e. the marginal value, to a random number
		for (uint i=0; i< dist.dimensions()[d]-1; ++i)
		{
			double value = (int)(rand->Next_Rand<double>()*90+10);
			sum += value;
			//dist->back(d,i) = value;
			marg[index_type(d,i)] = value;
		}
		//dist->back(d, dist->dimensions()[d]-2) = 250.0 - sum;
		marg.back(d) = 250.0 - sum;
	}


	//-----------------------------------------------------------------------------------------
	// set the marginal for the zone within the region
	marginal_itf& zmarg = my_zone->Target_Marginal_Distribution<marginal_itf&>();
	zmarg.resize(index,0);
	for (uint d=0; d< dist.num_dimensions(); ++d)
	{
		double sum = 0;
		// set the last value for each dimension/index pair, i.e. the marginal value, to a random number
		for (uint i=0; i< dist.dimensions()[d]-1; ++i)
		{
			double value = (int)(rand->Next_Rand<double>()*20+2);
			sum += value;
			//dist->back(d,i) = value;
			zmarg[index_type(d,i)] = value;
		}
		//dist->back(d, dist->dimensions()[d]-2) = 250.0 - sum;
		zmarg.back(d) = 83.0 - sum;
	}

	// Print
	marg.write(out);
	zmarg.write(out);


	//-----------------------------------------------------------------------------------------
	// fit the random marginal distribution
	my_region->Synthesize_Population<NULLTYPE>();
	
	// Print
	out << endl << endl;
	dist.write(out);

	joint_itf& zdist = my_zone->Target_Joint_Distribution<joint_itf&>();
	zdist.write(out);

	


	START();

	// Break Here
	bool pause=true;
	cin >> pause;
}


