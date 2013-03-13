#include <gtest\gtest.h>
#include <Io\Io.h>
extern char* db_path;


class DBTest : public testing::Test {
protected:
	virtual void SetUp() {
		db = ::open_sqlite_database(::db_path);
	}
	auto_ptr<odb::database> db;
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
	ASSERT_FALSE(db.get() == NULL);
}

TEST_F(DBTest, LocationFKLocation_Data){
	typedef odb::query<polaris::io::Location> query;
	typedef odb::result<polaris::io::Location> result;
	odb::transaction t(this->db->begin());
	result r(this->db->query<polaris::io::Location>(query::true_expr));
	int count = 0;
	for (result::iterator i (r.begin()); i!=r.end(); ++i)
	{
		if (i->getLocation_Data() == nullptr)
			count++;
	}
	ASSERT_EQ(count, 0);
}

TEST_F(DBTest, ZoneFKZone_Land_Use){
	typedef odb::query<polaris::io::Zone> query;
	typedef odb::result<polaris::io::Zone> result;
	odb::transaction t(this->db->begin());
	result r(this->db->query<polaris::io::Zone>(query::true_expr));
	int count = 0;
	for (result::iterator i (r.begin()); i!=r.end(); ++i)
	{
		if (i->getZone_Land_Use() == nullptr)
			count++;
	}
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
