#ifndef System
#define System

#include "Shared.h"

namespace polaris
{
namespace io
{
//Forward declarations.
//
class Dimension;
class Meta;
class Quantity;

#pragma db value
class table_field
{
public:
	std::string table;
	std::string field;

};


static bool operator< (const table_field& param1, const table_field& param2)
{
	return (param2.table + param2.field < param1.table + param1.field);
}

#pragma db object
class Quantity
{
public:
	// Default Constructor
	Quantity () {}        
	Quantity (table_field tf_, shared_ptr<Dimension> quantity_)
	: tf (tf_), quantity (quantity_)
	{
	}
	//Accessors
	const table_field& getTf () const {return tf;}
	void setTf (const table_field& tf_) {tf = tf_;}
	const shared_ptr<Dimension>& getQuantity () const {return quantity;}
	void setQuantity (const shared_ptr<Dimension>& quantity_) {quantity = quantity_;}
//Data Fields
private:
	friend class odb::access;
	#pragma db id
	table_field tf;
	shared_ptr<Dimension> quantity;
};

#pragma db object
class Dimension
{
public:
	// Default Constructor
	Dimension () {}        
	Dimension (std::string quantity_, int mass_, int length_, int time_, int lane_, int usd_)
	: quantity (quantity_), mass (mass_), length (length_), time (time_), lane (lane_), usd (usd_)
	{
	}
	//Accessors
	const std::string& getQuantity () const {return quantity;}
	void setQuantity (const std::string& quantity_) {quantity = quantity_;}
	const int& getMass () const {return mass;}
	void setMass (const int& mass_) {mass = mass_;}
	const int& getLength () const {return length;}
	void setLength (const int& length_) {length = length_;}
	const int& getTime () const {return time;}
	void setTime (const int& time_) {time = time_;}
	const int& getLane () const {return lane;}
	void setLane (const int& lane_) {lane = lane_;}
	const int& getUsd () const {return usd;}
	void setUsd (const int& usd_) {usd = usd_;}
//Data Fields
private:
	friend class odb::access;
	#pragma db id
	std::string quantity;
	int mass;
	int length;
	int time;
	int lane;
	int usd;
};

#pragma db object
class Meta
{
public:
	// Default Constructor
	Meta () {}        
	Meta (int id_, std::string key_, std::string value_)
	: id (id_), key (key_), value (value_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const std::string& getKey () const {return key;}
	void setKey (const std::string& key_) {key = key_;}
	const std::string& getValue () const {return value;}
	void setValue (const std::string& value_) {value = value_;}
//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int id;
	std::string key;
	std::string value;
};
}
}
#endif
