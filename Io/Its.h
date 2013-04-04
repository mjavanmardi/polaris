#ifndef Its
#define Its
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
class Component;
class Component_Key;
class Instance;
class Instance_Value;
class Action;
class Action_Key;


#pragma db object
class Component
{
public:
	// Default Constructor
	Component () {}        
	Component (int id_, std::string name_, std::string icon_, std::vector<weak_ptr<Instance> > instances_, std::vector<weak_ptr<Action> > actions_, std::vector<shared_ptr<Component_Key> > keys_)
	: id (id_), name (name_), icon (icon_), instances (instances_), actions (actions_), keys (keys_)
	{
	}
	Component (int id_, std::string name_, std::string icon_)
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
	const std::vector<weak_ptr<Instance> >& getInstances () const {return instances;}
	void setInstances (const std::vector<weak_ptr<Instance> >& instances_) {instances = instances_;}
	void setInstanc (const weak_ptr<Instance>  instances_) {instances.push_back(instances_);}
	const std::vector<weak_ptr<Action> >& getActions () const {return actions;}
	void setActions (const std::vector<weak_ptr<Action> >& actions_) {actions = actions_;}
	void setAction (const weak_ptr<Action>  actions_) {actions.push_back(actions_);}
	const std::vector<shared_ptr<Component_Key> >& getKeys () const {return keys;}
	void setKeys (const std::vector<shared_ptr<Component_Key> >& keys_) {keys = keys_;}
	void setKey (const shared_ptr<Component_Key>  keys_) {keys.push_back(keys_);}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int id;
	std::string name;
	std::string icon;
	#pragma db inverse(component)
	std::vector<weak_ptr<Instance> > instances;
	#pragma db inverse(component)
	std::vector<weak_ptr<Action> > actions;
	std::vector<shared_ptr<Component_Key> > keys;
};

#pragma db object
class Component_Key
{
public:
	// Default Constructor
	Component_Key () {}        
	Component_Key (std::string key_, std::string value_type_, std::string value_constraint_, bool is_state_, bool required_, std::string note_)
	: key (key_), value_type (value_type_), value_constraint (value_constraint_), is_state (is_state_), required (required_), note (note_)
	{
	}
	//Accessors
	const std::string& getKey () const {return key;}
	void setKey (const std::string& key_) {key = key_;}
	const std::string& getValue_Type () const {return value_type;}
	void setValue_Type (const std::string& value_type_) {value_type = value_type_;}
	const std::string& getValue_Constraint () const {return value_constraint;}
	void setValue_Constraint (const std::string& value_constraint_) {value_constraint = value_constraint_;}
	const bool& getIs_State () const {return is_state;}
	void setIs_State (const bool& is_state_) {is_state = is_state_;}
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
	bool is_state;
	bool required;
	std::string note;
};

#pragma db object
class Instance
{
public:
	// Default Constructor
	Instance () {}        
	Instance (int id_, shared_ptr<Component> component_, int location_link_, float location_offset_, float location_x_, float location_y_, std::string owner_, int operational_status_, std::string schedule_, std::vector<shared_ptr<Instance_Value> > values_)
	: id (id_), component (component_), location_link (location_link_), location_offset (location_offset_), location_x (location_x_), location_y (location_y_), owner (owner_), operational_status (operational_status_), schedule (schedule_), values (values_)
	{
	}
	Instance (int id_, int location_link_, float location_offset_, float location_x_, float location_y_, std::string owner_, int operational_status_, std::string schedule_)
	: id (id_), location_link (location_link_), location_offset (location_offset_), location_x (location_x_), location_y (location_y_), owner (owner_), operational_status (operational_status_), schedule (schedule_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const shared_ptr<Component> getComponent () const {return component;}
	void setComponent (const shared_ptr<Component> component_) {component = component_;}
	const int& getLocation_Link () const {return location_link;}
	void setLocation_Link (const int& location_link_) {location_link = location_link_;}
	const float& getLocation_Offset () const {return location_offset;}
	void setLocation_Offset (const float& location_offset_) {location_offset = location_offset_;}
	const float& getLocation_X () const {return location_x;}
	void setLocation_X (const float& location_x_) {location_x = location_x_;}
	const float& getLocation_Y () const {return location_y;}
	void setLocation_Y (const float& location_y_) {location_y = location_y_;}
	const std::string& getOwner () const {return owner;}
	void setOwner (const std::string& owner_) {owner = owner_;}
	const int& getOperational_Status () const {return operational_status;}
	void setOperational_Status (const int& operational_status_) {operational_status = operational_status_;}
	const std::string& getSchedule () const {return schedule;}
	void setSchedule (const std::string& schedule_) {schedule = schedule_;}
	const std::vector<shared_ptr<Instance_Value> >& getValues () const {return values;}
	void setValues (const std::vector<shared_ptr<Instance_Value> >& values_) {values = values_;}
	void setValu (const shared_ptr<Instance_Value>  values_) {values.push_back(values_);}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	int id;
	#pragma db not_null
	shared_ptr<Component> component;
	int location_link;
	float location_offset;
	float location_x;
	float location_y;
	std::string owner;
	int operational_status;
	std::string schedule;
	std::vector<shared_ptr<Instance_Value> > values;
};

#pragma db object
class Instance_Value
{
public:
	// Default Constructor
	Instance_Value () {}        
	Instance_Value (int id_, shared_ptr<Component_Key> key_, std::string value_)
	: id (id_), key (key_), value (value_)
	{
	}
	Instance_Value (int id_, std::string value_)
	: id (id_), value (value_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const shared_ptr<Component_Key> getKey () const {return key;}
	void setKey (const shared_ptr<Component_Key> key_) {key = key_;}
	const std::string& getValue () const {return value;}
	void setValue (const std::string& value_) {value = value_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id auto
	int id;
	shared_ptr<Component_Key> key;
	std::string value;
};

#pragma db object
class Action
{
public:
	// Default Constructor
	Action () {}        
	Action (int id_, shared_ptr<Component> component_, std::vector<shared_ptr<Action_Key> > keys_, std::string name_, std::string note_)
	: id (id_), component (component_), keys (keys_), name (name_), note (note_)
	{
	}
	Action (int id_, std::string name_, std::string note_)
	: id (id_), name (name_), note (note_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const shared_ptr<Component> getComponent () const {return component;}
	void setComponent (const shared_ptr<Component> component_) {component = component_;}
	const std::vector<shared_ptr<Action_Key> >& getKeys () const {return keys;}
	void setKeys (const std::vector<shared_ptr<Action_Key> >& keys_) {keys = keys_;}
	void setKey (const shared_ptr<Action_Key>  keys_) {keys.push_back(keys_);}
	const std::string& getName () const {return name;}
	void setName (const std::string& name_) {name = name_;}
	const std::string& getNote () const {return note;}
	void setNote (const std::string& note_) {note = note_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db auto id
	int id;
	#pragma db not_null
	shared_ptr<Component> component;
	std::vector<shared_ptr<Action_Key> > keys;
	std::string name;
	std::string note;
};

#pragma db object
class Action_Key
{
public:
	// Default Constructor
	Action_Key () {}        
	Action_Key (std::string key_, std::string value_type_, std::string value_constraint_, bool required_, std::string note_)
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
}}
#endif