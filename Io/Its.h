#ifndef Its
#define Its

#include "Shared.h"

namespace polaris{ namespace io {
//Forward declarations.
//
class LinkList;
class Component;
class VMS;
class HAR;
class VSS;
class Depot;
class OpenShoulder;
class Action;
class Action_Key;



#pragma db object
class LinkList
{
public:
    // Default Constructor
    LinkList () {}        
	LinkList (int id_, std::vector<int > links_)
	: id (id_), links (links_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const std::vector<int >& getLinks () const {return links;}
	void setLinks (const std::vector<int >& links_) {links = links_;}
	void setLink (const int  links_) {links.push_back(links_);}
	//Data Fields
private:
	friend class odb::access;
	#pragma db auto id
	int id;
	std::vector<int > links;
};


#pragma db object
class Component
{
public:
    // Default Constructor
    Component () {}        
	Component (int id_, std::string name_, std::string icon_, std::vector<weak_ptr<Action> > actions_)
	: id (id_), name (name_), icon (icon_), actions (actions_)
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
	const std::vector<weak_ptr<Action> >& getActions () const {return actions;}
	void setActions (const std::vector<weak_ptr<Action> >& actions_) {actions = actions_;}
	void setAction (const weak_ptr<Action>  actions_) {actions.push_back(actions_);}
	//Data Fields
private:
	friend class odb::access;
	#pragma db id
	int id;
	std::string name;
	std::string icon;
	#pragma db inverse(component)
	std::vector<weak_ptr<Action> > actions;
};


#pragma db object
class VMS
{
public:
    // Default Constructor
    VMS () {}        
	VMS (int id_, shared_ptr<Component> component_, int link_, int dir_, float offset_, float setback_, int initial_event_)
	: id (id_), component (component_), link (link_), dir (dir_), offset (offset_), setback (setback_), initial_event (initial_event_)
	{
	}
	VMS (int id_, int link_, int dir_, float offset_, float setback_, int initial_event_)
	: id (id_), link (link_), dir (dir_), offset (offset_), setback (setback_), initial_event (initial_event_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const shared_ptr<Component> getComponent () const {return component;}
	void setComponent (const shared_ptr<Component> component_) {component = component_;}
	const int& getLink () const {return link;}
	void setLink (const int& link_) {link = link_;}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_) {dir = dir_;}
	const float& getOffset () const {return offset;}
	void setOffset (const float& offset_) {offset = offset_;}
	const float& getSetback () const {return setback;}
	void setSetback (const float& setback_) {setback = setback_;}
	const int& getInitial_Event () const {return initial_event;}
	void setInitial_Event (const int& initial_event_) {initial_event = initial_event_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db auto id
	int id;
	#pragma db not_null
	shared_ptr<Component> component;
	int link;
	int dir;
	float offset;
	float setback;
	#pragma db default(-1)
	int initial_event;
};


#pragma db object
class HAR
{
public:
    // Default Constructor
    HAR () {}        
	HAR (int id_, shared_ptr<Component> component_, int link_, int dir_, float offset_, float setback_, int initial_event_, shared_ptr<LinkList> links_)
	: id (id_), component (component_), link (link_), dir (dir_), offset (offset_), setback (setback_), initial_event (initial_event_), links (links_)
	{
	}
	HAR (int id_, int link_, int dir_, float offset_, float setback_, int initial_event_)
	: id (id_), link (link_), dir (dir_), offset (offset_), setback (setback_), initial_event (initial_event_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const shared_ptr<Component> getComponent () const {return component;}
	void setComponent (const shared_ptr<Component> component_) {component = component_;}
	const int& getLink () const {return link;}
	void setLink (const int& link_) {link = link_;}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_) {dir = dir_;}
	const float& getOffset () const {return offset;}
	void setOffset (const float& offset_) {offset = offset_;}
	const float& getSetback () const {return setback;}
	void setSetback (const float& setback_) {setback = setback_;}
	const int& getInitial_Event () const {return initial_event;}
	void setInitial_Event (const int& initial_event_) {initial_event = initial_event_;}
	const shared_ptr<LinkList> getLinks () const {return links;}
	void setLinks (const shared_ptr<LinkList> links_) {links = links_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db auto id
	int id;
	#pragma db not_null
	shared_ptr<Component> component;
	int link;
	int dir;
	float offset;
	float setback;
	#pragma db default(-1)
	int initial_event;
	shared_ptr<LinkList> links;
};


#pragma db object
class VSS
{
public:
    // Default Constructor
    VSS () {}        
	VSS (int id_, shared_ptr<Component> component_, int link_, int dir_, float offset_, float setback_, int initial_speed_, std::string speed_, shared_ptr<LinkList> links_)
	: id (id_), component (component_), link (link_), dir (dir_), offset (offset_), setback (setback_), initial_speed (initial_speed_), speed (speed_), links (links_)
	{
	}
	VSS (int id_, int link_, int dir_, float offset_, float setback_, int initial_speed_, std::string speed_)
	: id (id_), link (link_), dir (dir_), offset (offset_), setback (setback_), initial_speed (initial_speed_), speed (speed_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const shared_ptr<Component> getComponent () const {return component;}
	void setComponent (const shared_ptr<Component> component_) {component = component_;}
	const int& getLink () const {return link;}
	void setLink (const int& link_) {link = link_;}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_) {dir = dir_;}
	const float& getOffset () const {return offset;}
	void setOffset (const float& offset_) {offset = offset_;}
	const float& getSetback () const {return setback;}
	void setSetback (const float& setback_) {setback = setback_;}
	const int& getInitial_Speed () const {return initial_speed;}
	void setInitial_Speed (const int& initial_speed_) {initial_speed = initial_speed_;}
	const std::string& getSpeed () const {return speed;}
	void setSpeed (const std::string& speed_) {speed = speed_;}
	const shared_ptr<LinkList> getLinks () const {return links;}
	void setLinks (const shared_ptr<LinkList> links_) {links = links_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db auto id
	int id;
	#pragma db not_null
	shared_ptr<Component> component;
	int link;
	int dir;
	float offset;
	float setback;
	#pragma db default(35)
	int initial_speed;
	#pragma db default("list(35,40,45,55,60)")
	std::string speed;
	shared_ptr<LinkList> links;
};


#pragma db object
class Depot
{
public:
    // Default Constructor
    Depot () {}        
	Depot (int id_, shared_ptr<Component> component_, int link_, int dir_, float offset_, float setback_, shared_ptr<LinkList> links_, int fleet_size_, int available_)
	: id (id_), component (component_), link (link_), dir (dir_), offset (offset_), setback (setback_), links (links_), fleet_size (fleet_size_), available (available_)
	{
	}
	Depot (int id_, int link_, int dir_, float offset_, float setback_, int fleet_size_, int available_)
	: id (id_), link (link_), dir (dir_), offset (offset_), setback (setback_), fleet_size (fleet_size_), available (available_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const shared_ptr<Component> getComponent () const {return component;}
	void setComponent (const shared_ptr<Component> component_) {component = component_;}
	const int& getLink () const {return link;}
	void setLink (const int& link_) {link = link_;}
	const int& getDir () const {return dir;}
	void setDir (const int& dir_) {dir = dir_;}
	const float& getOffset () const {return offset;}
	void setOffset (const float& offset_) {offset = offset_;}
	const float& getSetback () const {return setback;}
	void setSetback (const float& setback_) {setback = setback_;}
	const shared_ptr<LinkList> getLinks () const {return links;}
	void setLinks (const shared_ptr<LinkList> links_) {links = links_;}
	const int& getFleet_Size () const {return fleet_size;}
	void setFleet_Size (const int& fleet_size_) {fleet_size = fleet_size_;}
	const int& getAvailable () const {return available;}
	void setAvailable (const int& available_) {available = available_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db auto id
	int id;
	#pragma db not_null
	shared_ptr<Component> component;
	int link;
	int dir;
	float offset;
	float setback;
	shared_ptr<LinkList> links;
	#pragma db default(99999)
	int fleet_size;
	#pragma db default(99999)
	int available;
};


#pragma db object
class OpenShoulder
{
public:
    // Default Constructor
    OpenShoulder () {}        
	OpenShoulder (int id_, shared_ptr<Component> component_, shared_ptr<LinkList> links_, bool open_)
	: id (id_), component (component_), links (links_), open (open_)
	{
	}
	OpenShoulder (int id_, bool open_)
	: id (id_), open (open_)
	{
	}
	//Accessors
	const int& getId () const {return id;}
	void setId (const int& id_) {id = id_;}
	const shared_ptr<Component> getComponent () const {return component;}
	void setComponent (const shared_ptr<Component> component_) {component = component_;}
	const shared_ptr<LinkList> getLinks () const {return links;}
	void setLinks (const shared_ptr<LinkList> links_) {links = links_;}
	const bool& getOpen () const {return open;}
	void setOpen (const bool& open_) {open = open_;}
	//Data Fields
private:
	friend class odb::access;
	#pragma db auto id
	int id;
	#pragma db not_null
	shared_ptr<Component> component;
	shared_ptr<LinkList> links;
	#pragma db default(false)
	bool open;
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
