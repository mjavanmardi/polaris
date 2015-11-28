// notes:
// think we should cut out the Prototype/Implementation/Concepts/Types namespaces
// always return pointers when dealing with non-primitive types even if the underlying member is not a pointer
#define get(X) typename ComponentType::X

prototype struct Link
{
	tag_as_prototype;
	
	access(int, uuid);
	access(Network_Components::Network<get(network_reference_type)>*, network_reference);
	access(vector<Activity_Location_Components::Activity_Location<get(activity_location_type)>*>*, activity_locations);
};