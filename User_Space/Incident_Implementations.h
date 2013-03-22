#pragma once
#include "Incident_Prototype.h"

namespace Incident_Components
{
	namespace Types
	{

	}

	namespace Concepts
	{
	}
	
	namespace Implementations
	{
		implementation struct Base_Incident : public Polaris_Component<APPEND_CHILD(Base_Incident),MasterType,Data_Object,ParentType>
		{
			virtual int Identify(){return ComponentType::component_index;}

			member_data(int,incident_type,none,none);
			member_data(int,geometry,none,none);
			member_data(int,time_detected,none,none);
			member_data(int,expected_duration,none,none);
			member_data(int,start_time,none,none);
			member_data(int,end_time,none,none);
			member_data(int,expired,none,none);
		};

		implementation struct Weather_Incident : public Base_Incident<MasterType,NT,APPEND_CHILD(Weather_Incident)>
		{
			virtual int Identify(){return ComponentType::component_index;}

			member_data(int,weather_type,none,none);
			member_data(int,precipitation_rate,none,none);
			member_data(int,snow_depth,none,none);
			member_data(int,visibility,none,none);
			member_data(int,temperature,none,none);
		};
		
		implementation struct Accident_Incident : public Base_Incident<MasterType,NT,APPEND_CHILD(Accident_Incident)>
		{
			virtual int Identify(){return ComponentType::component_index;}

			member_data(int,lanes,none,none);
			member_data(int,severity,none,none);
			member_data(int,accident_type,none,none);
			member_data(int,vehicles_involved,none,none);
			member_data(int,vehicle_type,none,none);
			member_data(int,injury_type,none,none);
		};

		implementation struct Congestion_Incident : public Base_Incident<MasterType,NT,APPEND_CHILD(Congestion_Incident)>
		{
			virtual int Identify(){return ComponentType::component_index;}

			member_data(int,travel_time,none,none);
			member_data(int,slowdown_index,none,none);
		};
		
		implementation struct Lane_Closure_Incident : public Base_Incident<MasterType,NT,APPEND_CHILD(Lane_Closure_Incident)>
		{
			virtual int Identify(){return ComponentType::component_index;}

			member_data(int,lanes,none,none);
		};




		implementation struct Incident_Manager_Implementation : public Polaris_Component<APPEND_CHILD(Incident_Manager_Implementation),MasterType,Execution_Object,ParentType>
		{
			typedef Incident<typename MasterType::type_of(base_incident),ComponentType> Base_Incident_Interface;
			typedef Incident<typename MasterType::type_of(weather_incident),ComponentType> Weather_Incident_Interface;
			typedef Incident<typename MasterType::type_of(accident_incident),ComponentType> Accident_Incident_Interface;
			typedef Incident<typename MasterType::type_of(congestion_incident),ComponentType> Congestion_Incident_Interface;
			typedef Incident<typename MasterType::type_of(lane_closure_incident),ComponentType> Lane_Closure_Incident_Interface;

			declare_feature_conditional_implementation(Incident_Manager_Conditional)
			{
				response.next._iteration =_iteration + 1;
				response.next._sub_iteration = Scenario_Components::Types::END_OF_ITERATION;

				response.result = true;
			}

			declare_feature_event_implementation(Incident_Manager_Event)
			{

			}

			feature_implementation vector< Base_Incident_Interface* >& Get_Incidents(int start_time,int end_time)
			{
			}

			feature_implementation vector< Base_Incident_Interface* >& Get_Incidents(string geometry)
			{
			}
			
			feature_implementation vector< Base_Incident_Interface* >& Get_Incidents(int link_id)
			{
				list< Base_Incident_Interface* >::iterator itr;

				for(itr=_incident_container.begin();itr!=_incident_container.end();itr++)
				{
					Base_Incident_Interface* base_incident=*itr;
					//Do Base stuff

					switch(base_incident->Identify())
					{
					case typename MasterType::type_of(weather_incident)::component_index:
						Weather_Incident_Interface* incident;
						//Do Weather stuff
						break;
					case typename MasterType::type_of(accident_incident)::component_index:
						Accident_Incident_Interface* incident;
						//Do Accident stuff
						break;
					//ETC....
					default:
						break;
					};
				}


			}

			feature_implementation void Accept_Incident(Incident<TargetType,NT>* incident)
			{
				_incident_container->push_back((Base_Incident_Interface*)incident);
			}

			member_data(list< Base_Incident_Interface* >,incident_container);
		};
	}
}
