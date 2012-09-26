#pragma once
#include "Repository_Base_Includes.h"

//==================================================================================================================
/// EXAMPLE Component namespace -	used to organize similar high level transportation objects, i.e. Traveler, Vehicle,
///									Link, etc. which may have many different implementations depending on usage
///
/// Each component namespace consists of 6 standard sub-namespaces across two files.  The namespaces and files are:
///
///	1. [COMPONENT_NAME]_Interfaces.h file:
///		a.) Types namespace
///		b.) Concepts namespace
///		c.) Interfaces namespace
///		d.) Data Structures namespace
///
///	2. [COMPONENT_NAME]_Bases.h file:
///		e.) Bases namespace
///		f.) Components namespace
///
namespace EXAMPLE_COMPONENTS
{
	//==================================================================================================================
	/// Namespace of useful types.  Add any types that convey semantic infomration regarding inputs, outpute, requirements, etc. 
	/// for components in this namespace.  Use well defined typenames that convey the purpose of the Tag
	//------------------------------------------------------------------------------------------------------------------
	namespace Types
	{
		typedef true_type MY_TYPE;
	}


	//==================================================================================================================
	/// Namespace which holds all the concepts which enforce requirements and restrictions on template members, including components 
	/// List all concepts that may be used by this namespace here
	//------------------------------------------------------------------------------------------------------------------
	namespace Concepts
	{
		/// EXAMPLE Concept: replace CONCEPT_NAME and CHECK below, and add/remove requirements as necessary
		concept CONCEPT_NAME
		{
			begin_requirements_list;
			requires_typename_state(none, CHECK, true_type,"ERROR_MESSAGE");
			end_requirements_list(CHECK);
		};
	}


	//==================================================================================================================
	/// Namespace which includes the interface(or interfaces if necessary) which links to the various Bases regarding the transportation object
	//------------------------------------------------------------------------------------------------------------------
	namespace Interfaces
	{
		//------------------------------------------------------------------------------------------------------------------
		/// RENAME THE Inteface struct below.  This is the inteface to a POLARIS component
		//------------------------------------------------------------------------------------------------------------------
		template <typename ThisType, typename CallerType>
		struct INTERFACE_NAME
		{
			/// INTERFACE FUNCTION EXAMPLE - this example dispatched the Initialize function call to the component base
			facet void Initialize()
			{
				return PTHIS(ThisType)->Initialize<Dispatch<ThisType>,CallerType,TargetType>();
			}
			 
			// ACCESSOR EXAMPLE - this creates an accessor called ACCESSOR_NAME
			facet_accessor(ACCESSOR_NAME);
		};
	}


	//==================================================================================================================
	/// Namespace of useful related data structures for the components
	//------------------------------------------------------------------------------------------------------------------
	namespace Data_Structures
	{
		//------------------------------------------------------------------------------------------------------------------
		// Example of a useful structure
		// Access structure by 'Name'
		// Structure has a 'Value' member of type 'Data_Type'
		// Structure gets true_type tags of Type_Trait_1, Type_Trait_2, etc. which can be concept checked.
		// i.e. Type_Trait_1 could be "Feet", or "Length";
		//------------------------------------------------------------------------------------------------------------------
		typedef float Data_Type;
		Basic_Data_Struct(Name, Data_Type, Type_Trait_1, Type_Trait_2, Type_Trait_3);
		Basic_Data_Struct(Length_Measurement, float, length, feet);
	}
}