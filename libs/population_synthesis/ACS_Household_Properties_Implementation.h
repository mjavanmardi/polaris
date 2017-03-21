#pragma once

#include "activity_simulator/Household_Properties_Prototype.h"



namespace Household_Components
{
	namespace Implementations
	{
		//==================================================================================
		/// Properties classes
		//----------------------------------------------------------------------------------
		implementation struct ACS_Household_Static_Properties_Implementation : public Polaris_Component< MasterType,INHERIT(ACS_Household_Static_Properties_Implementation), Data_Object>
		{
			// Tag as Implementation
			typedef typename Polaris_Component<MasterType,INHERIT(ACS_Household_Static_Properties_Implementation),Data_Object>::Component_Type ComponentType;

			//=================================================================
			// Basic Person Characteristics Used in Popsyn algorithms
			m_data(long long,ID, NONE, NONE);
			m_data(double,Weight, NONE, NONE);
			m_data(uint,Index, NONE, NONE);				//index into the joint-distribution matrix of the region (convert using region.get_index())
			m_data(uint,Test_Index, NONE, NONE);		//index into the test marginal distribution for this household

			//=================================================================
			// Census specific household data, used in ABM routines
			m_data(Types::HHTYPE, Household_type, NONE, NONE);
			m_data(int, Household_size, NONE, NONE);
			m_data(int, Number_of_workers, NONE, NONE);
			m_data(int, Number_of_vehicles, NONE, NONE);		
			member_component_and_feature_accessor(Income, Value, Basic_Units::Prototypes::Currency, Basic_Units::Implementations::Currency_Implementation<NT>);

			//=================================================================
			// Container which holds linked persons in ACS File for this household info unit
			m_container(std::vector<Person_Components::Prototypes::Person_Properties<typename MasterType::person_static_properties_type>*>, Persons_Container, NONE, NONE);

			template <typename T> T Number_of_Adults()
			{

			}

			// Characteristics setter
			template<typename TargetType> void Characteristics(std::vector<double>* data)
			{
				if (data->size() != 5) THROW_EXCEPTION("ERROR: linker file definition out of date.")

				// these setters correspond exactly to the ACS-PUMS definitions and layout as given in pums_file.txt.  if pumsfile changes change these functions
				typedef Prototypes::Household_Properties<ComponentType> this_itf;
				this_itf* pthis = (this_itf*)this;
						
				pthis->Household_type<Types::HHTYPE>((Types::HHTYPE)(int)(*data)[0]);
				pthis->Household_size<int>((*data)[1]);
				pthis->Number_of_vehicles<int>((*data)[2]);
				pthis->Number_of_workers<int>((*data)[3]);
				pthis->Income<Basic_Units::Currency_Variables::Dollars>((*data)[4]);

			}
		};

	}
}
