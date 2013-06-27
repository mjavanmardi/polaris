#include <gtest/gtest.h>
#include "tmc/Clustering.h"
class ClusteringTest : public testing::Test {
protected:
	virtual void SetUp() {
		kmc = new polaris::KMClustering();
		//Cluster 0
		pt.push_back(1); pt.push_back(1); points.push_back(pt); pt.clear();		// point 0
		pt.push_back(1.1); pt.push_back(1.2); points.push_back(pt); pt.clear();	// point 1
		pt.push_back(1.8); pt.push_back(2); points.push_back(pt); pt.clear();	// point 2
		pt.push_back(0.1); pt.push_back(0.1); points.push_back(pt); pt.clear();	// point 3
		//Cluster 1
		pt.push_back(5); pt.push_back(6); points.push_back(pt); pt.clear();		// point 4
		pt.push_back(5.6); pt.push_back(6.2); points.push_back(pt); pt.clear();	// point 5
		pt.push_back(4.8); pt.push_back(5.5); points.push_back(pt); pt.clear();	// point 6
		kmc->Run(points, 2);
	}
	polaris::KMClustering* kmc;
	std::vector<std::vector<double> > points;
	std::vector<double> pt;
};

TEST_F(ClusteringTest, ClusterID) {
	using namespace polaris;
	int clister_1 = kmc->cluster_id(0);
	int clister_2 = kmc->cluster_id(4);
	EXPECT_EQ(kmc->cluster_id(1), clister_1);
	EXPECT_EQ(kmc->cluster_id(2), clister_1);
	EXPECT_EQ(kmc->cluster_id(3), clister_1);
	EXPECT_EQ(kmc->cluster_id(5), clister_2);
	EXPECT_EQ(kmc->cluster_id(6), clister_2);
}