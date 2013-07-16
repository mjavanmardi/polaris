#pragma once
#include <vector>
#include "utils/types.h"
#include "Io/Io.h"
#include <algorithm>
namespace polaris {
class Depot
{
public:
	Depot (){}
	// initialize an object using a record from Depot table in database
	Depot (polaris::io::Depot& instance)
	{
		links_covered_ = instance.getLinks()->getLinks();
		if (instance.getPolicy() == "depot")
			policy_ = depot;
		if (instance.getPolicy() == "enroute")
			policy_ = enroute;
		if (instance.getPolicy() == "fixed_location")
			policy_ = fixed_location;
		depot_link_ = instance.getLink()*2 + instance.getDir();
		fleet_size_ = instance.getFleet_Size();
		CalculateLinkPresenseFactor();
	}
	// this will run the calculations and will return the locations of the trucks at a given time step
	const std::vector<LinkID>& getTruckLinks(int time)
	{
		CalculateTruckLinks();
		return truck_links_;
	}
private:
	std::vector<LinkID> truck_links_;
	std::vector<LinkID> links_covered_;
	// the weight of every link having a truck on it at every given time
	std::vector<double> w_links_covered_;
	int fleet_size_;
	enum policy_t {depot, enroute, fixed_location} policy_;
	LinkID depot_link_;
	void CalculateTruckLinks()
	{
		truck_links_.clear();
		int index;
		for (int i = 0; i < fleet_size_; i++)
		{
			index = rand() % links_covered_.size();
			truck_links_.push_back(links_covered_[index]);
		}

	}
	void CalculateLinkPresenseFactor()
	{
		size_t ln = w_links_covered_.size();
		w_links_covered_.resize(ln, 0);
		double rem_prob;
		double p_at_depot;
		std::vector<int>::iterator it;
		switch(policy_)
		{
		case depot:
			p_at_depot = 0.7;
			
			it = find(links_covered_.begin(), links_covered_.end(), depot_link_);
			if (it == links_covered_.end())
			{
				links_covered_.push_back(depot_link_);
				w_links_covered_.push_back(p_at_depot);
			}
			else
			{
				w_links_covered_[links_covered_.begin() - it] = p_at_depot;
			}
			
			rem_prob = (1.0 - p_at_depot) / links_covered_.size();
			for (unsigned int i = 0; i < ln; i++)
			{
				w_links_covered_[i] = rem_prob;
			}
			break;
		case enroute:
			rem_prob = 1.0 / links_covered_.size();
			for (unsigned int i = 0; i < ln; i++)
			{
				w_links_covered_[i] = rem_prob;
			}
			break;
		case fixed_location:
			rem_prob = 0.3 / links_covered_.size();
			for (unsigned int i = 0; i < ln; i++)
			{
				w_links_covered_[i] = rem_prob;
			}
			int k = 20; //select 20 locations
			for (int i = 0; i < k; i++)
			{
				int index = rand() % ln;
				w_links_covered_[i] = 0.7 / k;
			}

		}


	}
};
}