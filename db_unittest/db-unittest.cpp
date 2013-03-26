#include <gtest\gtest.h>
#include <Io\Io.h>
#include <Io\Geometry.h>
extern char* db_path;


class DBTest : public testing::Test {
protected:
	virtual void SetUp() {
		db = open_sqlite_database(db_path);
	}
	auto_ptr<odb::database> db;
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
		db = ::open_sqlite_database(::db_path);
	}
	typedef odb::query<T> query;
	typedef T TT;
	typedef odb::result<T> result;
	auto_ptr<odb::database> db;
};




TEST_F(DBTest, OpenFunction){
	EXPECT_FALSE(this->db.get() == NULL);
}

TEST_F(DBTest, LocationFKLocation_Data){
	typedef odb::query<polaris::io::Location> query;
	typedef odb::result<polaris::io::Location> result;
	odb::transaction t(this->db->begin());
	std::cout << 1 << "\n";
	result r(this->db->query<polaris::io::Location>(query::location_data.is_null()));
	std::cout << 2 << "\n";
	int count = 0;
	std::cout << 3 << "\n";
	for (result::iterator i (r.begin()); i!=r.end(); ++i)
	{
			count++;
	}
	std::cout << 4 << "\n";
	t.commit();
	std::cout << 5 << "\n";
	ASSERT_EQ(count, 0);
}

TEST_F(DBTest, ZoneFKZone_Land_Use){
	typedef odb::query<polaris::io::Zone> query;
	typedef odb::result<polaris::io::Zone> result;
	odb::transaction t(this->db->begin());
	result r(this->db->query<polaris::io::Zone>(query::zone_land_use.is_null()));
	int count = 0;
	for (result::iterator i (r.begin()); i!=r.end(); ++i)
	{
		if (i->getZone_Land_Use() == nullptr)
			count++;
	}
	t.commit();
	ASSERT_EQ(count, 0);
}

TEST_F(DBTest, LinkFKNode_A){
	typedef odb::query<polaris::io::Link> query;
	typedef odb::result<polaris::io::Link> result;
	odb::transaction t(this->db->begin());
	result r(this->db->query<polaris::io::Link>(query::true_expr));
	int count = 0;
	for (result::iterator i (r.begin()); i!=r.end(); ++i)
	{
		if (i->getNode_A() == nullptr)
			count++;
	}
	t.commit();
	ASSERT_EQ(count, 0);
}
TEST_F(DBTest, LinkFKNode_B){
	typedef odb::query<polaris::io::Link> query;
	typedef odb::result<polaris::io::Link> result;
	odb::transaction t(this->db->begin());
	result r(this->db->query<polaris::io::Link>(query::true_expr));
	int count = 0;
	for (result::iterator i (r.begin()); i!=r.end(); ++i)
	{
		if (i->getNode_B() == nullptr)
			count++;
	}
	t.commit();
	ASSERT_EQ(count, 0);
}

typedef ::testing::Types<polaris::io::Link, polaris::io::Node, polaris::io::Zone> MyTypes;
TYPED_TEST_CASE(TableTest, MyTypes);

TYPED_TEST(TableTest, IsEmpty) {
	odb::transaction t(this->db->begin());
	typename TestFixture::result r(this->db->query<typename TestFixture::TT>(typename TestFixture::query::true_expr));
	ASSERT_FALSE(r.empty());
	t.commit();
}

TEST(SpatialiteT, LinkPoints)
{
	using namespace polaris::io;
	std::map<int, shape_geometry> res;
	res = GetLinkPoints(::db_path);
	ASSERT_GE(0, res.size());
}
