#include "Io/Io.h"
#include "apps/lprobability/data_structures.h"
#include <string>
#include "utils/types.h"
#include "utils/utils.h"
#include "Io/Io.h"
#include "Exceptions.h"
#include "tmc/DataStructures.h"
#include "Detector1DU.h"
#include "Clustering.h"

/** @file */ 

namespace polaris {

/// Populates Matrix with the data specified in the Link_Probability Table in the database
int read_probability_factors(std::string& db_prefix, polaris::Matrix& mat)
{
	using namespace polaris;
	using namespace std;
	using namespace odb;
	typedef odb::query<polaris::io::Link_Probability> query;
	typedef odb::result<io::Link_Probability> result;
	unique_ptr<odb::database> db = open_sqlite_database(db_prefix);
	odb::transaction t(db->begin());
	result r(db->query<io::Link_Probability>(query::true_expr));
	for (result::iterator i (r.begin ()); i != r.end (); ++i)
	{
		mat(i->getDir_Link_From(), i->getDir_Link_To()) = i->getCount();
	}
	t.commit();
	return errno;
}

/// Calculate a relevance score for an event.
double calculate_rs(const polaris::Matrix& link_factors, std::vector<polaris::LinkID>& e_links, std::vector<double> e_links_w, std::vector<polaris::LinkID>& m_links)
{
	if (e_links.size()!=e_links_w.size())
		throw size_ex;
	using namespace std;
	using namespace polaris;
	typedef vector<LinkID>::iterator link_it;
	double sum_event_link, result;
	result  = 0;
	for (int i = 0; i<e_links.size();i++)
	{
		sum_event_link = 0;
		for (link_it it = m_links.begin(); it!=m_links.end();++it)
		{
			sum_event_link += link_factors(*it, e_links[i]);
		}
		result += sum_event_link * e_links_w[i];
	}
	return result;
}
/// Populates graph from database Link table.
void populate_graph(const std::string db_prefix, polaris::Digraph& g)
{
	using namespace polaris;
	using namespace std;
	unique_ptr<odb::database> db = open_sqlite_database(db_prefix);
	odb::transaction t(db->begin());
	LinkR r(db->query<io::Link>(LinkQ::true_expr));
	for (LinkIt it( r.begin()); it != r.end(); ++it)
	{
		if (it->getLanes_Ab()>0)
		{
			g.addEdge(it->getNode_A()->getNode(), it->getNode_B()->getNode(), (float) (it->getLength() / it->getFspd_Ab()), utils::get_link_uid(it->getLink(),0));
		}
		if (it->getLanes_Ba()>0)
		{
			g.addEdge(it->getNode_B()->getNode(), it->getNode_A()->getNode(), (float) (it->getLength() / it->getFspd_Ba()), utils::get_link_uid(it->getLink(),1));
		}
		
	}
	t.commit();
}
/// Populates the road graph using the connectivity table, so that the rules of the road (turn prohibitions) are taken into account
void populate_graph_conn(const std::string db_prefix, polaris::Digraph& g)
{
	using namespace polaris;
	using namespace std;
	unique_ptr<odb::database> db = open_sqlite_database(db_prefix);
	odb::transaction t(db->begin());
	ConnectR r(db->query<io::Connect>(ConnectQ::true_expr));
	//shared_ptr<io::Link> link_from, link_to;
	io::db_ptr<io::Link>::Type link_from, link_to;
	for (auto it( r.begin()); it != r.end(); ++it)
	{
		link_from = it->getLink();
		link_to = it->getTo_Link();

		if(it->getDir() == 0) 
		{
			// add a-b direction fro to_link
			g.addEdge(link_from->getNode_A()->getNode(), link_from->getNode_B()->getNode(), (float)(link_from->getLength() / link_from->getFspd_Ab()));
			if (link_from->getNode_B()->getNode() == link_to->getNode_A()->getNode()) 
			{
				//add a-b direction for from_link
				g.addEdge(link_to->getNode_A()->getNode(), link_to->getNode_B()->getNode(), (float)(link_to->getLength() / link_to->getFspd_Ab()));
			}
			else
			{
				//add b-a direction for from_link
				g.addEdge(link_to->getNode_B()->getNode(), link_to->getNode_A()->getNode(), (float)(link_to->getLength() / link_to->getFspd_Ba()));
			}
		}
		else 
		{	// add b-a direction for from_link
			g.addEdge(link_from->getNode_B()->getNode(), link_from->getNode_A()->getNode(), (float)(link_from->getLength() / link_from->getFspd_Ba()));
			if (link_from->getNode_A()->getNode() == link_to->getNode_A()->getNode()) 
			{
				//add a-b direction for from_link
				g.addEdge(link_to->getNode_A()->getNode(), link_to->getNode_B()->getNode(), (float)(link_to->getLength() / link_to->getFspd_Ab()));
			}
			else
			{
				//add b-a direction for from_link
				g.addEdge(link_to->getNode_B()->getNode(), link_to->getNode_A()->getNode(), (float)(link_to->getLength() / link_to->getFspd_Ba()));
			}
		}
	}
	t.commit();
}


/// Reads historic MOEs from LinkMOE table
void read_moe(const std::string& db_prefix, std::vector<polaris::io::LinkMOE>& out_moes, int start_time, int end_time)
{
	using namespace polaris;
	unique_ptr<odb::database> db = open_sqlite_database(db_prefix);
	odb::transaction t(db->begin());
	LinkMOER r(db->query<io::LinkMOE>(LinkMOEQ::start_time >= start_time && LinkMOEQ::start_time< end_time));
	for (auto it (r.begin()); it != r.end(); ++it)
		out_moes.push_back(*it.load().get());
}

void tain_detector_slow(const std::string db_prefix, std::unordered_map<polaris::LinkID, polaris::Detector1DU<double> >& out_link_detectors)
{
	using namespace polaris;
	std::unordered_map<polaris::LinkID, polaris::Detector1DU<double> > result;
	polaris::LinkID id;
	polaris::Detector1DU<double> detector;
	std::vector<double> training_data;
	unique_ptr<odb::database> db = open_sqlite_database(db_prefix);
	odb::transaction t(db->begin());
	LinkMOER rmoe(db->query<io::LinkMOE>(LinkMOEQ::true_expr));
	LinkR rl(db->query<io::Link>(LinkQ::true_expr));
	LinkMOEQ q (LinkMOEQ::link_uid == LinkMOEQ::_ref(id));
	out_link_detectors.clear();
	int count = 0;
	for (auto it_l (rl.begin()); it_l != rl.end(); ++it_l )
	{
		if (it_l->getLanes_Ab() > 0)
		{
			id = it_l->getLink() * 2;
			LinkMOER r(db->query(q));
			training_data.clear();
			for (auto it = (r.begin()); it != r.end(); ++it)
				training_data.push_back(it->getLink_Speed());
			if (training_data.size()!=0)
			{
				detector.Train(training_data);
				out_link_detectors[id] = detector;
			}
		}

	}

}
/// This function reads the speed data from LinkMOE data and uses is to train the outliers detector
void tain_detector(const std::string db_prefix, std::unordered_map<polaris::LinkID, polaris::Detector1DU<double> >& out_link_detectors)
{
	using namespace polaris;
	polaris::LinkID id;
	polaris::Detector1DU<double> detector;
	std::vector<double> training_data;
	unique_ptr<odb::database> db = open_sqlite_database(db_prefix);
	odb::transaction t(db->begin());
	LinkMOER rmoe(db->query<io::LinkMOE>(LinkMOEQ::true_expr));
	for (auto it = (rmoe.begin()); it!= rmoe.end(); ++it  )
	{
		id = it->getLink_Uid();
		auto it_ld = out_link_detectors.find(id);
		if (it_ld == out_link_detectors.end())
		{
			detector.Clear();
			detector.Update(it->getLink_Speed());
			out_link_detectors[id] = detector;
		}
		else
			it_ld->second.Update(it->getLink_Speed());
	}

}

void accident_clustering(const std::string db_prefix, int k, std::vector<std::vector<double> >& out_centers)
{
	vector<double> pt;
	std::set<std::pair<double, double> > point_set;
	std::pair<double, double> point_pair;
	std::vector<std::vector<double> > points;
	using namespace polaris;
	unique_ptr<odb::database> db = open_sqlite_database(db_prefix);
	odb::transaction t(db->begin());
	Microsoft_EventR r(db->query<io::Microsoft_Event>(Microsoft_EventQ::true_expr));
	for (auto it(r.begin()); it!=r.end(); ++it)
	{
		pt.clear();
		pt.push_back(it->getLng());
		pt.push_back(it->getLat());
		point_pair.first = it->getLng();
		point_pair.second = it->getLat();
		auto sit = point_set.find(point_pair);
		if (sit == point_set.cend())
		{
			points.push_back(pt);
			point_set.insert(point_pair);
		}
	}	
	polaris::KMClustering *kmc = new polaris::KMClustering();
	kmc->Run(points, k);
	out_centers = kmc->centers();
	kmc->PrintCenters();
	return;
}


}
