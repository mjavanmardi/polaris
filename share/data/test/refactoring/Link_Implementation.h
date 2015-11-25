implementation struct Link_Implementation:public Polaris_Component<MasterType,INHERIT(Link_Implementation),Execution_Object>
{
	typedef typename Polaris_Component<MasterType,INHERIT(Link_Implementation),Execution_Object>::Component_Type ComponentType;
		
	m_data(int, uuid, NONE, check(strip_modifiers(TargetType), is_arithmetic));
	m_container(boost::container::vector<typename MasterType::activity_location_type*>, activity_locations, NONE, NONE);
	m_prototype(Network_Components::Prototypes::Network,typename MasterType::network_type, network_reference, NONE, NONE);
};