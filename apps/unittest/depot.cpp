#include <gtest/gtest.h>
#include "tmc/depot.h"
#include "Io/Io.h"
class DepotTest : public testing::Test {
protected:
	virtual void SetUp() 
	{
		depot_link = 3;
		shared_ptr<polaris::io::Depot> d_input ( new polaris::io::Depot(1, 1, 1, 5, 5, "depot", 3, 3));
		std::vector<int> depot_links;
		depot_links.push_back(5);
		depot_links.push_back(6);
		depot_links.push_back(7);
		depot_links.push_back(8);
		shared_ptr<polaris::io::LinkList> ll (new polaris::io::LinkList(1, depot_links));
		d_input->setLinks(ll);
		d = new polaris::Depot(d_input);
	}
	virtual void TearDown()
	{
		delete d;
	}
	polaris::Depot* d;
	int depot_link;
};


TEST_F(DepotTest, DepotPolicy) {
		std::vector<polaris::LinkID> response;
		int nruns = 1000;
		int at_depot = 0;
		for (int i = 0; i < nruns; i++)
		{
			response = d->getTruckLinks();
			for (auto it = response.begin(); it != response.end(); ++it)
			{
				if (*it == this->depot_link)
					at_depot++;
			}
		}
	EXPECT_NEAR(1.0/5, (double)at_depot/nruns/5, 0.1);
}
