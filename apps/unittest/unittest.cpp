#include <iostream>
#include <gtest/gtest.h>
#include "utils/utils.h"
#include "tmc/Tmc.h"
#include "tmc/DFS.h"
#include "tmc/WeakCC.h"
#include "tmc/Detector1DU.h"
#include <random>
// #include "LinkProbability/DataStructures.h"

char *db_path;

class Detector1DUTest : public testing::Test {
protected:
	virtual void SetUp() {
		int nrolls=10000; 
		std::vector<double> training_data;
		// std::default_random_engine generator;
		std::minstd_rand0 generator;
		std::normal_distribution<double> distribution(5.0,2.0);
		d2 = new polaris::Detector1DU<double>();
		d2->Update(15);
		d2->Update(22);
		d2->Update(33);
		d2->Clear();
		for (int i=0; i<nrolls; ++i)
		{
			double number = distribution(generator);
			training_data.push_back(number);
			d2->Update(number);
		}
		d = new polaris::Detector1DU<double>();
		d->Train(training_data);
		d1 = new polaris::Detector1DU<double>();
		d1->Train(5.0, 2.0);
	}
	// virtual void TearDown() {
	// }
	polaris::Detector1DU<double>* d;
	polaris::Detector1DU<double>* d1;
	polaris::Detector1DU<double>* d2;
};

TEST_F(Detector1DUTest, Calssify) {
	using namespace polaris;
	ASSERT_TRUE(d->Calssify(5+2*3+0.1));
	ASSERT_FALSE(d->Calssify(5));
	ASSERT_FALSE(d->Calssify(5+2));
	ASSERT_FALSE(d->Calssify(5-2));
	ASSERT_FALSE(d->Calssify(5+2*2));
	ASSERT_FALSE(d->Calssify(5-2*2));
	ASSERT_TRUE(d->Calssify(5-2*3-0.1));
	ASSERT_FALSE(d->Calssify(5+2*3-0.1));
	ASSERT_FALSE(d->Calssify(5-2*3+0.1));
	ASSERT_TRUE(d1->Calssify(5+2*3+0.1));
	ASSERT_TRUE(d1->Calssify(5-2*3-0.1));
	ASSERT_FALSE(d1->Calssify(5+2*3-0.1));
	ASSERT_FALSE(d1->Calssify(5-2*3+0.1));
	ASSERT_FALSE(d1->Calssify(5));
	ASSERT_FALSE(d1->Calssify(5+2));
	ASSERT_FALSE(d1->Calssify(5-2));
	ASSERT_FALSE(d1->Calssify(5+2*2));
	ASSERT_FALSE(d1->Calssify(5-2*2));

	ASSERT_TRUE(d2->Calssify(5+2*3+0.1));
	ASSERT_TRUE(d2->Calssify(5-2*3-0.1));
	ASSERT_FALSE(d2->Calssify(5+2*3-0.1));
	ASSERT_FALSE(d2->Calssify(5-2*3+0.1));
	ASSERT_FALSE(d2->Calssify(5));
	ASSERT_FALSE(d2->Calssify(5+2));
	ASSERT_FALSE(d2->Calssify(5-2));
	ASSERT_FALSE(d2->Calssify(5+2*2));
	ASSERT_FALSE(d2->Calssify(5-2*2));
}

TEST_F(Detector1DUTest, Score) {
	using namespace polaris;
	ASSERT_LT(d->Score(5), d->Score(5+0.1));
	ASSERT_GT(d->Score(5-2.5), d->Score(5+2));
}
// To use a test fixture, derive a class from testing::Test.
class TMC : public testing::Test {
protected:
	virtual void SetUp() {
		link_factors(1,2) = 15;
		link_factors(1,3) = 75;
		m_links.push_back(1);
		e_links.push_back(2);
		e_links.push_back(3);
		eA_links_w.push_back(100);
		eA_links_w.push_back(50);
		eB_links_w.push_back(5);
		eB_links_w.push_back(20);
	}

	// virtual void TearDown() will be called after each test is run.
	// You should define it if there is cleanup work to do.  Otherwise,
	// you don't have to provide it.
	//
	// virtual void TearDown() {
	// }
	polaris::Matrix link_factors;
	std::vector<polaris::LinkID> e_links, m_links;
	std::vector<double> eA_links_w, eB_links_w;
};

// To use a test fixture, derive a class from testing::Test.
class DigraphTest : public testing::Test {
protected:
	virtual void SetUp() {

		g.addEdge(0,1,1,0);
		g.addEdge(1,2,1,1);
		g.addEdge(1,3,1,2);

		g.addEdge(5,11,1,4);
		g.addEdge(11,4,1,3);
		g.addEdge(11,6,1,5);

		g.addEdge(7,8,1,6);
		g.addEdge(8,9,1,7);
		g.addEdge(9,10,1,8);

		dfs_0  = new polaris::DFS(g, 0);
		dfs_5  = new polaris::DFS(g, 5);
		dfs_7  = new polaris::DFS(g, 7);
		cc = new polaris::WeakCC();
		cc->Run(g);
	}
	//virtual void TearDown() {
	//}
	polaris::Digraph g;
	polaris::DFS* dfs_0;
	polaris::DFS* dfs_5;
	polaris::DFS* dfs_7;
	polaris::WeakCC* cc;
};
TEST_F(DigraphTest, DFS) {
	using namespace polaris;
	EXPECT_EQ(dfs_0->Reachable().size(), 4);
	EXPECT_EQ(dfs_5->Reachable().size(), 4);
	EXPECT_EQ(dfs_7->Reachable().size(), 4);
}
TEST_F(DigraphTest, WeakCC_Count) {
	using namespace polaris;
	EXPECT_EQ(cc->Count(), 3);
	g.addEdge(3,5,1);
	cc = new polaris::WeakCC();
	cc->Run(g);
	EXPECT_EQ(cc->Count(), 2);
	g.addEdge(5,9,1);
	cc = new polaris::WeakCC();
	cc->Run(g);
	EXPECT_EQ(cc->Count(), 1);
}
TEST_F(DigraphTest, WeakCC) {
	using namespace polaris;
	EXPECT_EQ(cc->Id(0), 0);
	EXPECT_EQ(cc->Id(1), 0);
	EXPECT_EQ(cc->Id(2), 0);
	EXPECT_EQ(cc->Id(3), 0);

	EXPECT_EQ(cc->Id(4), 1);
	EXPECT_EQ(cc->Id(5), 1);
	EXPECT_EQ(cc->Id(6), 1);
	EXPECT_EQ(cc->Id(11), 1);

	EXPECT_EQ(cc->Id(8), 2);
	EXPECT_EQ(cc->Id(7), 2);
	EXPECT_EQ(cc->Id(9), 2);
	EXPECT_EQ(cc->Id(10), 2);
}
TEST_F(DigraphTest, DFS0) {
	using namespace polaris;
	EXPECT_EQ(dfs_0->Marked(0), true);
	EXPECT_EQ(dfs_0->Marked(1), true);
	EXPECT_EQ(dfs_0->Marked(2), true);
	EXPECT_EQ(dfs_0->Marked(3), true);
	EXPECT_EQ(dfs_0->Marked(4), false);
}

TEST_F(DigraphTest, DFS5) {
	using namespace polaris;
	EXPECT_EQ(dfs_5->Marked(5), true);
	EXPECT_EQ(dfs_5->Marked(4), true);
	EXPECT_EQ(dfs_5->Marked(6), true);
	EXPECT_EQ(dfs_5->Marked(11), true);
	EXPECT_EQ(dfs_5->Marked(9), false);
}

TEST_F(DigraphTest, DFS7) {
	using namespace polaris;
	EXPECT_EQ(dfs_7->Marked(7), true);
	EXPECT_EQ(dfs_7->Marked(8), true);
	EXPECT_EQ(dfs_7->Marked(9), true);
	EXPECT_EQ(dfs_7->Marked(10), true);
	EXPECT_EQ(dfs_7->Marked(4), false);
}

TEST_F(DigraphTest, LinkComponentsTest) {
	using namespace polaris;
	std::unordered_map<int, std::vector<polaris::LinkID> > lc = cc->LinkComponents();
	EXPECT_EQ(lc.size(), 3);
	EXPECT_EQ(lc.at(0).size(), 3);
	EXPECT_EQ(lc.at(1).size(), 3);
	EXPECT_EQ(lc.at(2).size(), 3);
	EXPECT_EQ(lc.at(0).at(0), 0);
	EXPECT_EQ(lc.at(0).at(1), 1);
	EXPECT_EQ(lc.at(0).at(2), 2);
	EXPECT_EQ(lc.at(1).at(0), 3);
	EXPECT_EQ(lc.at(1).at(1), 5);
	EXPECT_EQ(lc.at(1).at(2), 4);
	EXPECT_EQ(lc.at(2).at(0), 6);
	EXPECT_EQ(lc.at(2).at(1), 7);
	EXPECT_EQ(lc.at(2).at(2), 8);
}


TEST(LinkID, Dir0) {
	using namespace polaris;
	LinkDir ll;
	ll.dir = 0;
	ll.link_id = 5123;
	int uid =  utils::get_link_uid(ll);
	LinkDir ll_rev =  utils::get_link_dir(uid);
	EXPECT_EQ(ll.link_id*2, uid);
	EXPECT_EQ(false, (bool)ll_rev.dir);
	EXPECT_EQ(5123, ll_rev.link_id);
}

TEST(LinkID, Dir1) {
	using namespace polaris;
	LinkDir ll;
	ll.dir = 1;
	ll.link_id = 5123;
	int uid =  utils::get_link_uid(ll);
	LinkDir ll_rev =  utils::get_link_dir(uid);
	EXPECT_EQ(ll.link_id*2+1, uid);
	EXPECT_EQ(true, ll_rev.dir);
	EXPECT_EQ(5123, ll_rev.link_id);
}
TEST_F(TMC, RSTreeLinkNet) {
	using namespace polaris;
	double eA_score = calculate_rs(link_factors, e_links, eA_links_w, m_links);
	double eB_score = calculate_rs(link_factors, e_links, eB_links_w, m_links);
	EXPECT_LT(eB_score, eA_score);
	EXPECT_EQ(eA_score, 5250);
	EXPECT_EQ(eB_score, 1575);
}

TEST_F(TMC, RSTreeLinkNetException) {
	using namespace polaris;
	e_links.push_back(1);
	ASSERT_THROW(calculate_rs(link_factors, e_links, eA_links_w, m_links), polaris::size_mismatch_ex);
}

GTEST_API_ int main(int argc, char **argv) {
  std::cout << "Running main() from main.cpp\n";
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
