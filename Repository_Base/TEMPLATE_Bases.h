#pragma once

#include "TEMPLATE_interface.h"


namespace EXAMPLE_COMPONENTS
{
	//==================================================================================================================
	/// Namespace of Base classes related to the Interface(s) from the component namespace.
	/// The bases are combined with the interfaces to create components by adding data.
	//------------------------------------------------------------------------------------------------------------------
	namespace Bases
	{
		//------------------------------------------------------------------------------------------------------------------
		/// Example of a Base class.  There can be many versions of a base class for a given Interface
		//------------------------------------------------------------------------------------------------------------------
		struct BASE_NAME
		{
			/// Handler for a general Initializer dispatched from an Interface
			facet void Initialize(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				// DO STUFF TO LOCAL DATA MEMBERS HERE
			}
			/// Error Handler for general Initializers.  In this case, stops the initializer from being called directly
			facet void Initialize(call_requirements(requires(ThisType,!Is_Dispatched)))
			{
				assert_requirements(TargetType,Is_Dispatched,"TargetType is not dispatched");
			}


			//==========================================================================================
			/// Local data is created and the accessors defined manually as follows:
			int _EXAMPLE_DATA; // create the data
			// create a handler for the GET version of the ACCESSOR_NAME accessor created in the interface
			template<typename ThisType, typename CallerType, typename TargetType>
			TargetType ACCESSOR_NAME(call_requirements(requires(ThisType,Is_Dispatched)))
			{
				return (TargetType)_EXAMPLE_DATA; // return the local data member cast to the TargetType
			}
			// create a handler for the SET version of the ACCESSOR_NAME accessor created in the interface, 'facet' macro replaces the template definition, giving the same typenames as above
			facet void ACCESSOR_NAME(TargetType set_value, call_requirements(requires(ThisType,Is_Dispatched)))
			{
				_EXAMPLE_DATA = (int)set_value; // return the local data member cast to the TargetType
			}
			// Make sure to create an error handler for the accessors by negating the requirements
			facet void ACCESSOR_NAME(TargetType set_value, call_requirements(!(requires(ThisType,Is_Dispatched))))
			{
				assert_requirements(TargetType,Is_Dispatched,"TargetType is not dispatched");
			}
			// create a tag that says the getter and setter have been defined - the existence of this tag is checked if the interface accessors are invoked
			tag_getter_setter(ACCESSOR_NAME);



			//==========================================================================================
			// The process can be simplified as shown below using the 'member_data' macro, which creates the variable "double _GETTER_DATA" and its accessors
			member_data(double,GETTER_DATA,true,false);  // to turn the getter/setter to on, either enter a requirement or 'true' when requested.  'false' turns the accessor off
			member_data(double,GETTER_SETTER_DATA,requires(TargetType,is_arithmetic),true);  //this version has both accessors and requires the target of the getter to be arithmetic
		};
	}


	//==================================================================================================================
	/// Components namespace.  This is where the objects created using the interface(s) / base(s) reside
	//------------------------------------------------------------------------------------------------------------------
	namespace Components
	{
		typedef Polaris_Component<Interfaces::INTERFACE_NAME,Bases::BASE_NAME> COMPONENT_NAME;
	}	
}