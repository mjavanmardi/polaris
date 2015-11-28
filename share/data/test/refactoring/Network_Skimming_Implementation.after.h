template<typename TargetType>
bool DoStuff()
{
	auto link = this->link();
	auto uuid = link->uuid();
	link->uuid(3);
	auto network = link->network_reference();
	auto activity_locations = link->activity_locations();
}