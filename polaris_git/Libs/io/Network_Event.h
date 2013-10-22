#ifndef Ne
#define Ne
	
#include "Shared.h"

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
	Network_Event (int id_, std::string name_, std::string icon_, std::vector<std::weak_ptr<Event_Instance> > instances_, std::vector<shared_ptr<Event_Key> > keys_)
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
	const std::vector<std::weak_ptr<Event_Instance> >& getInstances () const {return instances;}
	void setInstances (const std::vector<std::weak_ptr<Event_Instance> >& instances_) {instances = instances_;}
	void setInstanc (const std::weak_ptr<Event_Instance>  instances_) {instances.push_back(instances_);}
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
	std::vector<std::weak_ptr<Event_Instance> > instances;
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
	Event_Instance (int id_, shared_ptr<Network_Event> event_, std::vector<shared_ptr<Event_Instance_Value> > values_, std::vector<int> links_, float location_x_, float location_y_, std::string reporter_, std::string confidence_level_, int time_detected_, int expected_duraition_, bool expired_, int start_time_, int end_time_, std::string note_)
	: id (id_), event (event_), values (values_), links (links_), location_x (location_x_), location_y (location_y_), reporter (reporter_), confidence_level (confidence_level_), time_detected (time_detected_), expected_duraition (expected_duraition_), expired (expired_), start_time (start_time_), end_time (end_time_), note (note_)
	{
	}
	Event_Instance (int id_, std::vector<int> links_, float location_x_, float location_y_, std::string reporter_, std::string confidence_level_, int time_detected_, int expected_duraition_, bool expired_, int start_time_, int end_time_, std::string note_)
	: id (id_), links (links_), location_x (location_x_), location_y (location_y_), reporter (reporter_), confidence_level (confidence_level_), time_detected (time_detected_), expected_duraition (expected_duraition_), expired (expired_), start_time (start_time_), end_time (end_time_), note (note_)
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
	const std::string& getConfidence_Level () const {return confidence_level;}
	void setConfidence_Level (const std::string& confidence_level_) {confidence_level = confidence_level_;}
	const int& getTime_Detected () const {return time_detected;}
	void setTime_Detected (const int& time_detected_) {time_detected = time_detected_;}
	const int& getExpected_Duraition () const {return expected_duraition;}
	void setExpected_Duraition (const int& expected_duraition_) {expected_duraition = expected_duraition_;}
	const bool& getExpired () const {return expired;}
	void setExpired (const bool& expired_) {expired = expired_;}
	const int& getStart_Time () const {return start_time;}
	void setStart_Time (const int& start_time_) {start_time = start_time_;}
	const int& getEnd_Time () const {return end_time;}
	void setEnd_Time (const int& end_time_) {end_time = end_time_;}
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
	std::string confidence_level;
	int time_detected;
	int expected_duraition;
	bool expired;
	int start_time;
	int end_time;
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
