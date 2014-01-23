#pragma once

#include "TEMPLATE_interface.h"


namespace EXAMPLE_COMPONENTS
{
	//==================================================================================================================
	/// Namespace of Base classes related to the Interface(s) from the component namespace.
	/// The bases are combined with the interfaces to create components by adding data.
	//------------------------------------------------------------------------------------------------------------------
	namespace Implementations
	{
		//------------------------------------------------------------------------------------------------------------------
		/// Example of a Base class.  There can be many versions of a base class for a given Interface
		/// The template parameter MasterType contains information about all types defined int the current application, and
		/// be used to reference other types in the program
		//------------------------------------------------------------------------------------------------------------------
		implementation struct IMPLEMENTATION_NAME
		{
			//==========================================================================================
			/// Handler for a general Initializer dispatched from an Interface
			template<typename TargetType> void Initialize(call_requirements(requires(TargetType,ComponentType,Concepts::CONCEPT_NAME)))
			{
				// DO STUFF TO LOCAL DATA MEMBERS HERE
			}
			/// Error Handler for general Initializers.  In this case, stops the initializer from being called directly
			template<typename TargetType> void Initialize(call_requirements(requires(TargetType,ComponentType,!Concepts::CONCEPT_NAME)))
			{
				assert_requirements(ComponentType,Concepts::CONCEPT_NAME,"ComponentType does not meet CONCEPT_NAME requirements");
			}


			//==========================================================================================
			/// Local data is created and the accessors defined MANUALLY as follows:
			int _EXAMPLE_DATA; // create the data
			// create a handler for the GET version of the ACCESSOR_NAME accessor created in the interface
			template<typename TargetType> TargetType ACCESSOR_NAME()
			{
				return (TargetType)_EXAMPLE_DATA; // return the local data member cast to the TargetType
			}
			// create a handler for the SET version of the ACCESSOR_NAME accessor created in the interface, 'facet' macro replaces the template definition, giving the same typenames as above
			template<typename TargetType> void ACCESSOR_NAME(TargetType set_value, call_requirements(requires(TargetType,ComponentType,Concepts::CONCEPT_NAME)))
			{
				_EXAMPLE_DATA = (int)set_value; // return the local data member cast to the TargetType
			}
			// Make sure to create an error handler for the accessors by negating the requirements
			template<typename TargetType> void ACCESSOR_NAME(TargetType set_value, call_requirements(!(requires(TargetType,ComponentType,Concepts::CONCEPT_NAME))))
			{
				assert_requirements(ComponentType,Concepts::CONCEPT_NAME,"ComponentType does not meet CONCEPT_NAME requirements");
			}
			// create a tag that says the getter and setter have been defined - the existence of this tag is checked if the interface accessors are invoked
			tag_getter_setter(ACCESSOR_NAME);



			//==========================================================================================
			// The process can be simplified as shown below using the 'm_data' macro, which creates the variable "double _GETTER_DATA" and its accessors
			m_data(double,GETTER_DATA,true,false);  // to turn the getter/setter to on, either enter a requirement or 'true' when requested.  'false' turns the accessor off
			m_data(double,GETTER_SETTER_DATA,requires(TargetType,TargetType,is_arithmetic),true);  //this version has both accessors and requires the target of the getter to be arithmetic


			//==========================================================================================
			// Finally, type information for other types can be ed through MasterType.  The example below
			// Creates a base member of SOME_OTHER_TYPE and creates accessors to its interface.
			m_prototype_basic(Null_Prototype<typename MasterType::SOME_OTHER_TYPE>, some_other_type);
		};
	}


	//==================================================================================================================
	/// Components namespace.  This is where the objects created using the interface(s) / base(s) reside
	//------------------------------------------------------------------------------------------------------------------
	namespace Components
	{
		//==========================================================================================
		/// The struct below creates the polaris component comprised of the interface/base and any parent types.
		/// The polaris component can be ed through the "type" member of the struct;
		implementation struct COMPONENT_NAME
		{
			typedef NULLTYPE GROUPLIST_TYPE; // no groupboost::container::list type defined
			typedef Data_Object OBJECT_TYPE; // The polaris component type is a "Data_Object", i.e. contains no schedulable functions
			typedef Polaris_Component< MasterType,Implementations::IMPLEMENTATION_NAME<MasterType>, PARENT_TYPE,GROUPLIST_TYPE, OBJECT_TYPE> type;
		};
	}


	/// After the above component is defined it can be added to MasterType, which should reside in the application project, as follows:
	struct EXAMPLE_Master_Type
	{
		typedef EXAMPLE_Master_Type T;

		//==============================================================================================
		// Signalization Types
		typedef EXAMPLE_COMPONENTS::Components::COMPONENT_NAME<T>::type	COMPONENT_TYPE;
	};
	/// This can then be accessed in other components as shown above: i.e.
	/// MasterType::COMPONENT_TYPE
}
