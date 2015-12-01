prototype struct Link
{
	tag_as_prototype;

	accessor(uuid, check(strip_modifiers(TargetType), is_arithmetic), NONE);
	accessor(network_reference, NONE, NONE);
	accessor(activity_locations, NONE, NONE);
};