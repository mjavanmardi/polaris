template<typename TargetType>
bool DoStuff()
{
	typedef Link_Components::Prototypes::Link<typename type_of(link_reference)> link_itf;
	
	typedef Activity_Location_Components::Prototypes::Activity_Location<typename remove_pointer<typename zone_itf::get_type_of(origin_activity_locations)::value_type>::type> location_itf;
	typedef Random_Access_Sequence<typename zone_itf::get_type_of(origin_activity_locations),location_itf*> locations_itf;
	
	typedef Network_Components::Prototypes::Network<typename type_of(network_reference)> network_itf;
	
	link_itf* link = this->link<  link_itf*>();
	int uuid = link->template uuid<int>();
	link->template uuid<int>(3);
	network_itf* network = link->template network_reference< network_itf* >();
	locations_itf* activity_locations = link->template activity_locations<locations_itf* >();
}