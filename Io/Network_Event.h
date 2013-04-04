#ifndef Ne
#define Ne
#pragma warning(disable:4068)    
#include <vector>
#include <string>
#include <odb/core.hxx>
// Include TR1 <memory> header in a compiler-specific fashion. Fall back
// on the Boost implementation if the compiler does not support TR1.
//
#include <odb/tr1/memory.hxx>
using std::tr1::shared_ptr;
using std::tr1::weak_ptr;

namespace polaris{ namespace io {
//Forward declarations.
//
class Network_Event;
class Event_Key;
class Event_Instance;
class Event_Instance_Value;


#pragma db object
class Network_Event
{
public:
    // Default Constructor
    Network_Event () {}        
	Network_Event (int id_, std::string name_, std::string icon_, std::vector<weak_ptr<Event_Instance> > instances_, std::vector<shared_ptr<Event_Key> > keys_)
	: id (id_), name (name_), icon (icon_), instances (instances_), keys (keys_)
	{
	}
	Network_Event (int id_, std::string name_, std::string icon_)
	: id (id_), name (name_), icon (icon_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const std::string& getName () const {return name;}
	void setName (const std::string& name_) {name = name_;}
	const std::string& getIcon () const {return icon;}
	void setIcon (const std::string& icon_) {icon = icon_;}
	const std::vector<weak_ptr<Event_Instance> >& getInstances () const {return instances;}
	void setInstances (const std::vector<weak_ptr<Event_Instance> >& instances_) {instances = instances_;}
	void setInstanc (const weak_ptr<Event_Instance>  instances_) {instances.push_back(instances_);}
	const std::vector<shared_ptr<Event_Key> >& getKeys () const {return keys;}
	void setKeys (const std::vector<shared_ptr<Event_Key> >& keys_) {keys = keys_;}
	void setKey (const shared_ptr<Event_Key>  keys_) {keys.push_back(keys_);}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int id;
	std::string name;
	std::string icon;
	#pragma db inverse(event)
	std::vector<weak_ptr<Event_Instance> > instances;
	std::vector<shared_ptr<Event_Key> > keys;
};

#pragma db object
class Event_Key
{
public:
    // Default Constructor
    Event_Key () {}        
	Event_Key (std::string key_, std::string value_type_, std::string value_constraint_, bool required_, std::string note_)
	: key (key_), value_type (value_type_), value_constraint (value_constraint_), required (required_), note (note_)
	{
	}
	//Accessors
	const std::string& getKey () const {return key;}
	void setKey (const std::string& key_) {key = key_;}
	const std::string& getValue_Type () const {return value_type;}
	void setValue_Type (const std::string& value_type_) {value_type = value_type_;}
	const std::string& getValue_Constraint () const {return value_constraint;}
	void setValue_Constraint (const std::string& value_constraint_) {value_constraint = value_constraint_;}
	const bool& getRequired () const {return required;}
	void setRequired (const bool& required_) {required = required_;}
	const std::string& getNote () const {return note;}
	void setNote (const std::string& note_) {note = note_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	std::string key;
	std::string value_type;
	std::string value_constraint;
	bool required;
	std::string note;
};

#pragma db object
class Event_Instance
{
public:
    // Default Constructor
    Event_Instance () {}        
	Event_Instance (int id_, shared_ptr<Network_Event> event_, std::vector<shared_ptr<Event_Instance_Value> > values_, std::vector<int> links_, float location_x_, float location_y_, std::string reporter_, std::string confedence_level_, std::string note_)
	: id (id_), event (event_), values (values_), links (links_), location_x (location_x_), location_y (location_y_), reporter (reporter_), confedence_level (confedence_level_), note (note_)
	{
	}
	Event_Instance (int id_, std::vector<int> links_, float location_x_, float location_y_, std::string reporter_, std::string confedence_level_, std::string note_)
	: id (id_), links (links_), location_x (location_x_), location_y (location_y_), reporter (reporter_), confedence_level (confedence_level_), note (note_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const shared_ptr<Network_Event> getEvent () const {return event;}
	void setEvent (const shared_ptr<Network_Event> event_) {event = event_;}
	const std::vector<shared_ptr<Event_Instance_Value> >& getValues () const {return values;}
	void setValues (const std::vector<shared_ptr<Event_Instance_Value> >& values_) {values = values_;}
	void setValu (const shared_ptr<Event_Instance_Value>  values_) {values.push_back(values_);}
	const std::vector<int>& getLinks () const {return links;}
	void setLinks (const std::vector<int>& links_) {links = links_;}
	void setLink (const int links_) {links.push_back(links_);}
	const float& getLocation_X () const {return location_x;}
	void setLocation_X (const float& location_x_) {location_x = location_x_;}
	const float& getLocation_Y () const {return location_y;}
	void setLocation_Y (const float& location_y_) {location_y = location_y_;}
	const std::string& getReporter () const {return reporter;}
	void setReporter (const std::string& reporter_) {reporter = reporter_;}
	const std::string& getConfedence_Level () const {return confedence_level;}
	void setConfedence_Level (const std::string& confedence_level_) {confedence_level = confedence_level_;}
	const std::string& getNote () const {return note;}
	void setNote (const std::string& note_) {note = note_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	int id;
	#pragma db not_null
	shared_ptr<Network_Event> event;
	std::vector<shared_ptr<Event_Instance_Value> > values;
	std::vector<int> links;
	float location_x;
	float location_y;
	std::string reporter;
	std::string confedence_level;
	std::string note;
};

#pragma db object
class Event_Instance_Value
{
public:
    // Default Constructor
    Event_Instance_Value () {}        
	Event_Instance_Value (int id_, shared_ptr<Event_Key> key_, std::string value_)
	: id (id_), key (key_), value (value_)
	{
	}
	Event_Instance_Value (int id_, std::string value_)
	: id (id_), value (value_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const shared_ptr<Event_Key> getKey () const {return key;}
	void setKey (const shared_ptr<Event_Key> key_) {key = key_;}
	const std::string& getValue () const {return value;}
	void setValue (const std::string& value_) {value = value_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	int id;
	shared_ptr<Event_Key> key;
	std::string value;
};
}}
#endif