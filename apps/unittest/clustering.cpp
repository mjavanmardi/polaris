#include <gtest/gtest.h>
#include "tmc/Clustering.h"
class ClusteringTest : public testing::Test {
protected:
	virtual void SetUp() {
		kmc = new polaris::KMClustering();
		//Cluster 0
		pt.push_back(1); pt.push_back(1); points.push_back(pt); pt.clear();		// point 0
		pt.push_back(2); pt.push_back(1); points.push_back(pt); pt.clear();	// point 1
		pt.push_back(1); pt.push_back(2); points.push_back(pt); pt.clear();	// point 2
		pt.push_back(2); pt.push_back(2); points.push_back(pt); pt.clear();	// point 3
		//Cluster 1
		pt.push_back(5); pt.push_back(6); points.push_back(pt); pt.clear();		// point 4
		pt.push_back(5.6); pt.push_back(6.2); points.push_back(pt); pt.clear();	// point 5
		pt.push_back(4.8); pt.push_back(5.5); points.push_back(pt); pt.clear();	// point 6
		kmc->Run(points, 2);
		//kmc->PrintCenters();
	}
	polaris::KMClustering* kmc;
	std::vector<std::vector<double> > points;
	std::vector<double> pt;
};

class ClusteringRandom : public testing::Test {
protected:
	virtual void SetUp() {
		kmc = new polaris::KMClustering();
		n = 1000;
		k = 10;
		for (int i = 0; i < n; i++)
		{
			double x = (double)rand() / RAND_MAX;
			double y = (double)rand() / RAND_MAX;
			pt.push_back(x);
			pt.push_back(y);
			points.push_back(pt);
			pt.clear();
		}
		kmc->Run(points, k);
		centers = kmc->centers();
	}
	polaris::KMClustering* kmc;
	std::vector<std::vector<double> > points;
	std::vector<double> pt;
	int n;
	int k;
	polaris::KMClustering::PointTable centers;
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

TEST_F(ClusteringRandom, ClusterDist) {
	using namespace polaris;
	double d, dist;
	int cluster_id;
	for (int i = 0; i < n; i++)
	{
		dist = std::numeric_limits<double>::max();
		for (int j = 0; j < k; j++)
		{
			d = kmc->dist(this->centers[j], this->points[i]);
			if (d < dist)
			{
				cluster_id = j;
				dist = d;
			}
		}
		ASSERT_EQ(cluster_id, kmc->cluster_id(i));
	}
}