#pragma once
#include "Traffic_Simulator_Includes.h"
#include "Activity_Location_Prototype.h"
#include "Traffic_Simulator_Concepts.h"

namespace Network_Components
{
	namespace Types
	{
		enum CALCULATION_STATUS
		{
			PROCESSING,
			COMPLETE
		};
		enum SUB_ITERATIONS
		{
			INITIALIZE = 19,
			PROCESS = 20,
			PATH_BUILDING = 21,
			UPDATE = 22,
			GET_RESULTS=23
		};
	}

	namespace Prototypes
	{

		prototype struct Network_Validator : public ComponentType
		{
			tag_as_prototype;

			//=============================================
			// Primary events
			//---------------------------------------------
			static void Validator_Conditional(ComponentType* _this,Event_Response& response)
			{
				typedef Network_Validator<ComponentType> _Validator_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_Validator_Interface* this_ptr=(_Validator_Interface*)_pthis;

				if (sub_iteration() == Types::SUB_ITERATIONS::GET_RESULTS)
				{
					this_ptr->Get_Results_Event<NT>();
					response.next._iteration = END;
					response.next._sub_iteration = 0;
				}
				else
				{
					THROW_EXCEPTION("ERROR: should not get here in conditional.");
				}
			}
			template<typename T> void Get_Results_Event()
			{
				cout<<endl<<endl<<"====================================================="<<endl<<"Network Validation Results:"<<endl;
				this_component()->Get_Results<NT>();
			}


			//=============================================
			// Primary data accessors
			//---------------------------------------------
			// links back to the network to be skimmed
			accessor(network_reference, NONE, NONE);
			accessor(num_checks_to_perform,NONE,NONE);
			accessor(validation_units, NONE, NONE);

			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------
			template<typename TargetType> void Initialize(int num_checks)
			{			
				this_component()->template Initialize<TargetType>(num_checks);
			
				this_component()->Load_Event<ComponentType>(Validator_Conditional,num_checks*5,Types::SUB_ITERATIONS::GET_RESULTS);
			}
		};

		prototype struct Network_Validation_Unit : public ComponentType
		{
			tag_as_prototype;

			//=============================================
			// Primary events
			//---------------------------------------------
			static void Network_Validation_Conditional(ComponentType* _this,Event_Response& response)
			{
				typedef Network_Validation_Unit<ComponentType> _This_Interface;
				ComponentType* _pthis = (ComponentType*)_this;
				_This_Interface* this_ptr=(_This_Interface*)_pthis;

				if (sub_iteration() == 0)
				{
					response.next._iteration = iteration();
					response.next._sub_iteration = Types::SUB_ITERATIONS::INITIALIZE;
				}
				else if (sub_iteration() == Types::SUB_ITERATIONS::INITIALIZE)
				{
					this_ptr->Initialize_Iteration<NT>();
					response.next._iteration = iteration()+2;
					response.next._sub_iteration = Types::SUB_ITERATIONS::UPDATE;
				}
				else if (sub_iteration() == Types::SUB_ITERATIONS::UPDATE)
				{
					this_ptr->Update_Validator<NT>();
					this_ptr->current_index<int&>()++;

					if (this_ptr->current_index<int>() == this_ptr->num_checks_to_perform<int>())
					{
						if (this_ptr->do_as_destination<bool>())
						{
							response.next._iteration = END;
						}
						else
						{
							response.next._iteration = iteration()+1;
							this_ptr->current_index(0);
							this_ptr->do_as_destination(true);
						}
					}
					else response.next._iteration = iteration()+1;
					response.next._sub_iteration = 0;
				}
				else
				{
					response.next._iteration = iteration()+1;
					response.next._sub_iteration = 0;
				}
			}


			//=============================================
			// Primary data accessors
			//---------------------------------------------
			// links back to the network to be skimmed
			accessor(network_validator, NONE, NONE);
			accessor(router,NONE,NONE);
			accessor(location,NONE,NONE);
			accessor(current_index,NONE,NONE);
			accessor(num_checks_to_perform,NONE,NONE);
			accessor(do_as_destination,NONE,NONE);
			accessor(network_reference,NONE,NONE);
			accessor(origin_routing_results,NONE,NONE);
			accessor(destination_routing_results,NONE,NONE);
			accessor(link_failure,NONE,NONE);
			accessor(turn_failure,NONE,NONE);
			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------
			template<typename TargetType> void Initialize(TargetType location, requires(TargetType,check(TargetType, is_pointer) && check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype)))
			{
				this_component()->Initialize<TargetType>(location);

				// Load the skim updating event, which recalculates network skims at every Update interval (set in implementation)
				this_component()->Load_Event<ComponentType>(Network_Validation_Conditional,0,Types::SUB_ITERATIONS::INITIALIZE);
			}			
			template<typename TargetType> void Initialize(TargetType location, requires(TargetType,!check(TargetType, is_pointer) || !check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype)))
			{
				assert_check(TargetType, is_pointer,"TargetType is not a pointer" );
				assert_check(strip_modifiers(TargetType), Activity_Location_Components::Concepts::Is_Activity_Location_Prototype, "TargetType is not a valid location interface");
			}			
			
			template<typename TargetType> void Initialize_Iteration()
			{
				this_component()->Initialize_Iteration<TargetType>();
			}

			template<typename TargetType> void Update_Validator()
			{
				this_component()->Update_Validator<TargetType>();
			}

			//=============================================
			// Primary function accessors - used to pass through to the specific skimm table based on time-key
			//---------------------------------------------
		};

	}
}

