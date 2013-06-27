#include <iostream>
#include <gtest/gtest.h>
#include <gtest/gtest.h>
#include <Io/Io.h>
#include "spatial/spatial.h"
#include <Io/Utils.h>

char *db_path;

class DBTest : public testing::Test {
protected:
	virtual void SetUp() {
		db = open_sqlite_database(db_path);
	}
	unique_ptr<odb::database> db;
};


class SpatiaLiteTest : public testing::Test {
protected:
	virtual void SetUp() {
		db_handle = open_spatialite_database(db_path);
	}
	sqlite3* db_handle;
};

template <typename T>
class TableTest : public testing::Test {
protected:
	virtual void SetUp() {
		db = open_sqlite_database(db_path);
	}
	typedef odb::query<T> query;
	typedef T TT;
	typedef odb::result<T> result;
	unique_ptr<odb::database> db;
};




TEST_F(DBTest, OpenFunction){
	EXPECT_FALSE(this->db.get() == NULL);
}

//TEST_F(DBTest, Path){
//	typedef odb::query<polaris::io::Path> query;
//	typedef odb::result<polaris::io::Path> result;
//	odb::transaction t1(this->db->begin());
//	result r(this->db->query<polaris::io::Path>(query::true_expr));	
//	int count = 0;
//	int len = (int)r.begin()->getLinks().size();
//	for (result::iterator i (r.begin()); i!=r.end(); ++i)
//	{
//	  count++;
//	}	
//	ASSERT_EQ(count, 8);
//	ASSERT_EQ(len, 2);
//	t1.commit();
//}



TEST_F(DBTest, LocationFKLocation_Data){
	typedef odb::query<polaris::io::Location> query;
	typedef odb::result<polaris::io::Location> result;
	odb::transaction t(this->db->begin());
	result r(this->db->query<polaris::io::Location>(query::location_data.is_null()));
	int count = 0;
	for (result::iterator i (r.begin()); i!=r.end(); ++i)
	{
	  count++;
	}
	t.commit();
	ASSERT_EQ(count, 0);
}

TEST_F(DBTest, LocationFKZone){
	typedef odb::query<polaris::io::Location> query;
	typedef odb::result<polaris::io::Location> result;
	odb::transaction t(this->db->begin());
	result r(this->db->query<polaris::io::Location>(query::zone.is_null()));
	int count = 0;
	for (result::iterator i (r.begin()); i!=r.end(); ++i)
	{
	  count++;
	}
	t.commit();
	ASSERT_EQ(count, 0);
}

TEST_F(DBTest, LocationFKLink){
	typedef odb::query<polaris::io::Location> query;
	typedef odb::result<polaris::io::Location> result;
	odb::transaction t(this->db->begin());
	result r(this->db->query<polaris::io::Location>(query::link.is_null()));
	int count = 0;
	for (result::iterator i (r.begin()); i!=r.end(); ++i)
	{
	  count++;
	}
	t.commit();
	ASSERT_EQ(count, 0);
}

TEST_F(DBTest, LocationLinkAttribute){
	typedef odb::query<polaris::io::Location> query;
	typedef odb::result<polaris::io::Location> result;
	odb::session s;
	odb::transaction t(this->db->begin());
	result r(this->db->query<polaris::io::Location>(query::true_expr));
	int count = 0;
	for (result::iterator i (r.begin()); i!=r.end(); ++i)
	{
		i->getLocation();
		i->getLink()->getLink();
		count++;
	}
	t.commit();
	ASSERT_NE(count, 0);
}

TEST_F(DBTest, Depot){
	typedef odb::query<polaris::io::Depot> query;
	typedef odb::result<polaris::io::Depot> result;
	odb::transaction t(this->db->begin());
	result r(this->db->query<polaris::io::Depot>(query::true_expr));
	int count = 0;
	for (result::iterator i (r.begin()); i!=r.end(); ++i)
	{
	  count++;
	}
	t.commit();
	ASSERT_EQ(count, 1);
}


TEST_F(DBTest, EventgetInstances)
{
	using namespace polaris::io;
	typedef odb::query<Network_Event> query;
	typedef odb::result<Network_Event> result;
	odb::session s;
	odb::transaction t(this->db->begin());
	result r(this->db->query<Network_Event>(query::true_expr));
	int count=0;
	for (result::iterator i (r.begin()); i!=r.end(); ++i)
	{
		count++;
	}
	t.commit();
	ASSERT_EQ(count, 5);
}



TEST_F(DBTest, LinkFKNode_A){
	typedef odb::query<polaris::io::Link> query;
	typedef odb::result<polaris::io::Link> result;
	odb::transaction t(this->db->begin());
	result r(this->db->query<polaris::io::Link>(query::node_a.is_null()));
	int count = 0;
	for (result::iterator i (r.begin()); i!=r.end(); ++i)
	{

			count++;
	}
	t.commit();
	ASSERT_EQ(count, 0);
}
TEST_F(DBTest, LinkFKNode_B){
	typedef odb::query<polaris::io::Link> query;
	typedef odb::result<polaris::io::Link> result;
	odb::transaction t(this->db->begin());
	result r(this->db->query<polaris::io::Link>(query::node_b.is_null()));
	int count = 0;
	for (result::iterator i (r.begin()); i!=r.end(); ++i)
	{

			count++;
	}
	t.commit();
	ASSERT_EQ(count, 0);
}

TEST_F(DBTest, LinkLanes){
	typedef odb::query<polaris::io::Link> query;
	typedef odb::result<polaris::io::Link> result;
	odb::transaction t(this->db->begin());
	result r(this->db->query<polaris::io::Link>(query::true_expr));
	int count = 0;
	for (result::iterator i (r.begin()); i!=r.end(); ++i)
	{
		i->getLanes_Ab();
		count++;
	}
	t.commit();
	ASSERT_NE(count, 0);
}

typedef ::testing::Types<polaris::io::Link, polaris::io::Node, polaris::io::Zone> MyTypes;
TYPED_TEST_CASE(TableTest, MyTypes);

/*TYPED_TEST(TableTest, IsEmpty) {
	odb::transaction t(this->db->begin());
	typename TestFixture::result r(this->db->query<typename TestFixture::TT>(typename TestFixture::query::true_expr));
	ASSERT_FALSE(r.empty());
	t.commit();
	}*/

TEST(SpatialiteT, LinkPoints)
{
	using namespace polaris::io;
	std::map<int, shape_geometry> res;
	res = GetLinkPoints(::db_path);
	ASSERT_EQ(res.size(), 31205);
}

TEST(SpatialiteT, NumberNodes)
{
	using namespace polaris::io;
	int res;
	res = polaris::CountRows(db_path, "Node");
	ASSERT_EQ(res, 18904);
}

TEST(SpatialiteT, NumberLinks)
{
	using namespace polaris::io;
	int res;
	res = polaris::CountRows(db_path, "Link");
	ASSERT_EQ(res, 31205);
}

TEST(SpatialiteT, LinksInsidePoly)
{
	using namespace polaris::io;
	std::vector<int> res;
	res = GetLinksInsideDepotPolygon(db_path);
	ASSERT_EQ(res.size(), 275);
}


TEST(SpatialiteT, GetLinkShapes)
{
	using namespace polaris::io;
	std::map<int, LineString2D> res;
	res = GetLinkShapes(db_path);
	ASSERT_EQ(res.size(), 31205);
}
TEST(SpatialiteT, GetCountyPolygons)
{
	using namespace polaris::io;
	std::map<std::string, Polygon2D>  res;
	res = GetCountyPolygons(db_path);
	ASSERT_EQ(res.size(), 102);
}
TEST(SpatialiteT, GetCountyPolygons1)
{
	using namespace polaris::io;
	std::map<std::string, Polygon2D>  res;
	res = GetCountyPolygons(db_path);
	Polygon2D pl = res["DeKalb"];
	EXPECT_NEAR(pl.exterior.coords[0].x, 368759, 2);
	EXPECT_NEAR(pl.exterior.coords[0].y, 4668052, 2);
}

TEST(SpatialiteT, GetLinkShapes1)
{
	using namespace polaris::io;
	std::map<int, LineString2D> res;
	res = GetLinkShapes(db_path);
	LineString2D pt = res[1];
	EXPECT_NEAR(pt.coords[0].x, 427146.3855, 0.001);
	EXPECT_NEAR(pt.coords[1].x, 427390.287021, 0.001);
}

TEST(SpatialiteT, GetDepotPolygons)
{
	using namespace polaris::io;
	std::map<int, Polygon2D>  res;
	res = GetDepotPolygons(db_path);
	ASSERT_EQ(res.size(), 1);
}
TEST(SpatialiteT, GetDepotPolygons1)
{
	using namespace polaris::io;
	std::map<int, Polygon2D>  res;
	res = GetDepotPolygons(db_path);
	Polygon2D pl = res[1];
	EXPECT_NEAR(pl.exterior.coords[0].x, 448813, 2);
	EXPECT_NEAR(pl.exterior.coords[0].y, 4639254, 2);
}


GTEST_API_ int main(int argc, char **argv) {
  std::cout << "Running main() from main.cpp\n";
  testing::InitGoogleTest(&argc, argv);
  if (argc != 2) 
  {
	  std::cout << "Usage: " << argv[0] << "[gtest arguments] partial_path_to_database\n";
	  return 1;
  }
  ::db_path = argv[1];
  return RUN_ALL_TESTS();
}
