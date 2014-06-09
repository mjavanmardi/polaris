#pragma once

#include "Synthesis_Region_Prototype.h"

namespace PopSyn
{

	namespace Implementations
	{
		//==========================================================================================
		// This is the base region implementation, should work for any popsyn algorithm
		implementation struct _Synthesis_Region_Base_Implementation
		{
			/// Zone collection associated with this region
			typedef dense_hash_map<long long, Prototypes::Synthesis_Zone<typename MasterType::synthesis_zone_type>*> zone_map_type;
			m_container(zone_map_type, Synthesis_Zone_Collection, NONE, NONE);

			/// Pointer to the top-level population synthesizer class		
			m_prototype(PopSyn::Prototypes::Population_Synthesizer,typename MasterType::population_synthesis_type, parent_reference, NONE, NONE);

			/// Temporary container used to associate households with persons, keyed on household id
			typedef dense_hash_map<double, Household_Components::Prototypes::Household_Properties<typename MasterType::household_static_properties_type>*> __temp_sample_map_type;		
			m_container(__temp_sample_map_type, Temporary_Sample_Data, NONE, NONE); 

			/// Output file stream
			m_data(ostream*,Output_Stream, NONE, NONE);

			// pass through functions to access the Top-level class scenario/network
			template<typename TargetType> TargetType scenario_reference()
			{
				return this->parent_reference<type_of(parent_reference)&>().scenario_reference<TargetType>();
			}
			template<typename TargetType> TargetType network_reference()
			{
				return this->parent_reference<type_of(parent_reference)&>().network_reference<TargetType>();
			}

			// Function to initialize the google hash maps
			template<typename TargetType> void Init()
			{
				this->_Synthesis_Zone_Collection.set_empty_key(-1);
				this->_Synthesis_Zone_Collection.set_deleted_key(-2);

				this->_Temporary_Sample_Data.set_empty_key(-1);
				this->_Temporary_Sample_Data.set_deleted_key(-2);
			}

		};
		//------------------------------------------------------------------------------------------

		//==========================================================================================
		// Recreate the three classes below for different population synthesis algorithm - inherit from the appropriate zone implementation
		//------------------------------------------------------------------------------------------
		implementation struct _Synthesis_Region_Polaris_Implementation : public _Synthesis_Region_Base_Implementation<MasterType>, _Synthesis_Zone_Polaris_Implementation<MasterType> 
		{
		};
		implementation struct Synthesis_Region_Implementation_Simple : public Polaris_Component<MasterType,INHERIT(Synthesis_Region_Implementation_Simple),Execution_Object>, _Synthesis_Region_Polaris_Implementation<MasterType>
		{
			// Methods for adding data to the region from which to sample - these will have to be rewritten in a new class if non-IPF methods are implemented
			
			// Use the current line in File_Reader and the info in linker to construct a new household object
			template<typename TargetType> void Add_Household_Sample(File_IO::File_Reader& fr, TargetType linker, requires(TargetType,check(TargetType, is_pointer)))
			{
				typedef get_mapped_component_type(Sample_Data_type) sample_type;
				typedef Household_Components::Prototypes::Household_Properties<sample_type> pop_unit_itf;

				Target_Joint_Distribution_type::index_type index;

				// find which marginal category each control variable belongs to and place in index
				double x;
				for (int i=0; i<linker->number_of_hh_dimensions(); i++)
				{
					fr.Get_Data<double>(x,linker->get_pums_column(i));
					index.push_back(linker->find_index_in_dimension(i,x));
				}

				// Create the household sample object
				typename sample_type::ID_type sample_id;
				typename sample_type::Weight_type weight;
				fr.Get_Data<typename sample_type::ID_type>(sample_id,linker->sample_id_column());
				fr.Get_Data<typename sample_type::Weight_type>(weight, linker->sample_weight_column());
				boost::container::vector<double> data;
				fr.Get_Data<double>(data,linker->get_pums_data_columns());
				pop_unit_itf* p = (pop_unit_itf*)Allocate<sample_type>();			
				p->ID(sample_id);				
				p->Weight(weight);
				p->template Characteristics<boost::container::vector<double>*>(&data);
				p->Index(_Target_Joint_Distribution.get_index(index));

				// Update the sample and joint distribution with the current population unit
				pair<typename Sample_Data_type::key_type,pop_unit_itf*> item = pair<typename Sample_Data_type::key_type,pop_unit_itf*>(p->template Index<typename Sample_Data_type::key_type&>(),p);
				_Sample_Data.insert(item);
				_Target_Joint_Distribution[index] += weight;
					
				// also add to temporary sample data, so that the person file records can attach to household records
				pair<typename Temporary_Sample_Data_type::key_type,pop_unit_itf*> tmp_item = pair<typename Temporary_Sample_Data_type::key_type,pop_unit_itf*>(sample_id,p);
				_Temporary_Sample_Data.insert(tmp_item);
			}
			// Use the current line in File_Reader and the info in linker to construct a new person object
			template<typename TargetType> void Add_Person_Sample(File_IO::File_Reader& fr, TargetType linker, requires(TargetType,check(TargetType, is_pointer)))
			{
				typedef get_mapped_component_type(Sample_Data_type) sample_type;
				typedef Household_Components::Prototypes::Household_Properties<sample_type> pop_unit_itf;
				typedef Random_Access_Sequence<typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_data_itf;
				typedef typename get_component_type(person_sample_data_itf) person_sample_type;
				typedef Person_Components::Prototypes::Person_Properties<person_sample_type> person_unit_itf;

				typedef PopSyn::Prototypes::Popsyn_File_Linker<typename MasterType::popsyn_file_linker_type> linker_itf;
				linker_itf* link = (linker_itf*)linker;

				Target_Person_Joint_Distribution_type::index_type index;

				// find which marginal category each control variable belongs to and place in index
				double x;
				for (int i=0; i<linker->number_of_person_dimensions(); i++)
				{
					fr.Get_Data<double>(x,link->get_pums_column(i,false));
					index.push_back(link->find_index_in_dimension(i,x,false));
				}

				typename person_sample_type::ID_type sample_id;
				typename person_sample_type::Weight_type weight=0;
				
				// Get person header info
				fr.Get_Data<typename person_sample_type::ID_type>(sample_id,link->person_sample_id_column());
				fr.Get_Data<typename person_sample_type::ID_type>(weight,link->person_weight_column());
				boost::container::vector<double> data;
				fr.Get_Data<double>(data,link->get_person_data_columns());

				// Create the person object
				person_unit_itf* p = (person_unit_itf*)Allocate<person_sample_type>();		
				p->ID(sample_id);				
				p->template Characteristics<boost::container::vector<double>*>(&data);	
				p->Index(_Target_Person_Joint_Distribution.get_index(index));

				// find the household that the person belongs to and add
				typename Temporary_Sample_Data_type::iterator sample_itr = _Temporary_Sample_Data.find(sample_id);
				if (sample_itr != _Temporary_Sample_Data.end())
				{
					pop_unit_itf* pop_unit = (pop_unit_itf*)sample_itr->second;
					pop_unit->template Persons_Container<person_sample_data_itf*>()->push_back(p);
				}
				else
				{
					THROW_WARNING("Warning: person sample unit refers to non-existent household sample unit, observation ignored.");
				}
				// Update the sample and joint distribution with the current population unit			
				if (_Target_Person_Joint_Distribution.size()) _Target_Person_Joint_Distribution[index] += weight;
			}
		};
		implementation struct Synthesis_Region_Implementation_Full : public Synthesis_Region_Implementation_Simple<MasterType,INHERIT(Synthesis_Region_Implementation_Full)>
		{
		};
	}
}

