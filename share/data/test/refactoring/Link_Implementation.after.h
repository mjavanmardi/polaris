implementation struct Link_Implementation:public Polaris_Component<MasterType,INHERIT(Link_Implementation),Execution_Object>
{
	typedef typename Polaris_Component<MasterType,INHERIT(Link_Implementation),Execution_Object>::Component_Type ComponentType;
	
	data(int, uuid);
	
	typedef typename MasterType::activity_location_type activity_location_type;
	data(vector<Activity_Location_Components::Activity_Location<activity_location_type>*>, activity_locations);
	
	typedef typename MasterType::network_type network_type;
	data(Network_Components::Network<network_type>*, network_reference);
};