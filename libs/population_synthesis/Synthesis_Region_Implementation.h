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
			typedef dense_hash_map<typename MasterType::household_static_properties_type::ID_type, Household_Components::Prototypes::Household_Properties<typename MasterType::household_static_properties_type>*> __temp_sample_map_type;		
			m_container(__temp_sample_map_type, Temporary_Sample_Data, NONE, NONE); 

			/// Output file stream
			m_data(ostream*,Output_Stream, NONE, NONE);

			// pass through functions to access the Top-level class scenario/network
			template<typename TargetType> TargetType scenario_reference()
			{
				return this->template parent_reference<type_of(parent_reference)&>().template scenario_reference<TargetType>();
			}
			template<typename TargetType> TargetType network_reference()
			{
				return this->template parent_reference<type_of(parent_reference)&>().template network_reference<TargetType>();
			}
			template<typename TargetType> TargetType file_linker_reference()
			{
				return this->template parent_reference<type_of(parent_reference)&>().template file_linker<TargetType>();
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
		implementation struct _Polaris_Synthesis_Region_Implementation : public _Synthesis_Region_Base_Implementation<MasterType>, _Polaris_Synthesis_Zone_Implementation<MasterType> 
		{
		};
		implementation struct Polaris_Synthesis_Region_Implementation_Simple : public Polaris_Component<MasterType,INHERIT(Polaris_Synthesis_Region_Implementation_Simple),Execution_Object>, _Polaris_Synthesis_Region_Implementation<MasterType>
		{
			typedef _Polaris_Synthesis_Region_Implementation<MasterType> BaseType;

			typedef typename Polaris_Component<MasterType,INHERIT(Polaris_Synthesis_Region_Implementation_Simple),Execution_Object>::Component_Type ComponentType;
			typedef ComponentType Component_Type;

			using_member(BaseType,Sample_Data);
			using_member(BaseType,Temporary_Sample_Data);
			using_member(BaseType,Target_Joint_Distribution);
			using_member(BaseType,Target_Marginal_Distribution);
			using_member(BaseType,Target_Person_Joint_Distribution);
			using_member(BaseType,Synthesis_Zone_Collection);
			using_member(BaseType,Solver_Settings);

			// Use the current line in File_Reader and the info in linker to construct a new household object
			template<typename TargetType> void Add_Household_Sample(File_IO::File_Reader& fr, TargetType linker, requires(TargetType,check(TargetType, is_pointer)))
			{
				typedef get_mapped_component_type(Sample_Data_type) sample_type;
				typedef Household_Components::Prototypes::Household_Properties<sample_type> pop_unit_itf;

				typename Target_Joint_Distribution_type::index_type index;

				// find which marginal category each control variable belongs to and place in index
				double x;
				for (int i=0; i<linker->number_of_hh_dimensions(); i++)
				{
					fr.Get_Data<double>(x,linker->get_pums_column(i)); // read each data value from the pums file for this household
					index.push_back(linker->find_index_in_dimension(i,x)); // find which marginal category the value belongs to for each dimension and save in index
				}

				// find out which test category the test control variable belongs
				int test_index = -1;
				if (fr.Get_Data<double>(x,linker->get_pums_column(0,true,true)))
					test_index = linker->find_index_in_dimension(0,x,true,true);

				// Create the household sample object by reading from the input file
				typename sample_type::ID_type sample_id;
				typename sample_type::Weight_type weight;
				fr.Get_Data<typename sample_type::ID_type>(sample_id,linker->sample_id_column()); // get id from the id column in linker
				fr.Get_Data<typename sample_type::Weight_type>(weight, linker->sample_weight_column()); // get weight from the weight column
				std::vector<double> data;
				fr.Get_Data<double>(data,linker->get_pums_data_columns()); // dump the rest of the columns to the data vector
				pop_unit_itf* p = (pop_unit_itf*)Allocate<sample_type>();			
				p->ID(sample_id);				
				p->Weight(weight);
				p->template Characteristics<std::vector<double>*>(&data);
				p->Index(_Target_Joint_Distribution.get_index(index));	// save the index (converted to 1-dimensional value) into the joint distribution for this household
				p->Test_Index(test_index);

				// Update the sample and joint distribution with the current population unit
				pair<typename Sample_Data_type::key_type,pop_unit_itf*> item = pair<typename Sample_Data_type::key_type,pop_unit_itf*>(p->template Index<typename Sample_Data_type::key_type&>(),p);
				_Sample_Data.insert(item);
				_Target_Joint_Distribution[index] += weight;
				
				// also add to temporary sample data, so that the person file records can attach to household records
				pair<typename BaseType::Temporary_Sample_Data_type::key_type,pop_unit_itf*> tmp_item = pair<typename BaseType::Temporary_Sample_Data_type::key_type,pop_unit_itf*>(sample_id,p);
				_Temporary_Sample_Data.insert(tmp_item);
			}
			
			// Use the current line in File_Reader and the info in linker to construct a new person object
			template<typename TargetType> void Add_Person_Sample(File_IO::File_Reader& fr, TargetType linker, requires(TargetType,check(TargetType, is_pointer)))
			{
				typedef get_mapped_component_type(Sample_Data_type) sample_type;
				typedef Household_Components::Prototypes::Household_Properties<sample_type> pop_unit_itf;
				typedef Random_Access_Sequence<typename pop_unit_itf::get_type_of(Persons_Container)> person_sample_data_itf;
				typedef get_component_type(person_sample_data_itf) person_sample_type;
				typedef Person_Components::Prototypes::Person_Properties<person_sample_type> person_unit_itf;

				typedef PopSyn::Prototypes::Popsyn_File_Linker<typename MasterType::popsyn_file_linker_type> linker_itf;
				linker_itf* link = (linker_itf*)linker;

				typename Target_Person_Joint_Distribution_type::index_type index;

				// find which marginal category each control variable belongs to and place in index
				double x;
				for (int i=0; i<linker->number_of_person_dimensions(); i++)
				{
					fr.Get_Data<double>(x,link->get_pums_column(i,false));
					index.push_back(link->find_index_in_dimension(i,x,false));
				}
				// find out which test category the test control variable belongs
				int test_index = -1;
				if (fr.Get_Data<double>(x,linker->get_pums_column(0,false,true)))
					test_index = linker->find_index_in_dimension(0,x,false,true);

				typename person_sample_type::ID_type sample_id;
				typename person_sample_type::Weight_type weight=0;
				
				// Get person header info
				fr.Get_Data<typename person_sample_type::ID_type>(sample_id,link->person_sample_id_column());
				fr.Get_Data<typename person_sample_type::ID_type>(weight,link->person_weight_column());
				std::vector<double> data;
				fr.Get_Data<double>(data,link->get_person_data_columns());

				// Create the person object
				person_unit_itf* p = (person_unit_itf*)Allocate<person_sample_type>();		
				p->ID(sample_id);				
				p->template Characteristics<std::vector<double>*>(&data);	
				p->Index(_Target_Person_Joint_Distribution.get_index(index));
				p->Test_Index(test_index);

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
			
			// MAIN SYNTHESIS ROUTINE:
			template<typename TargetType> void Synthesize_Population(requires(TargetType,check_stripped_type(BaseType,Concepts::Is_IPF_Compatible)))
			{
				typedef Prototypes::Synthesis_Zone<ComponentType> base_type;
				base_type* base_itf = (base_type*)this;

				//==========================================================
				#pragma region Define Interfaces to Region distribution, marginals, sample, and zones collections
				//==========================================================
				// Get the solution settings
				typedef typename get_type_of(Target_Joint_Distribution)::value_type value_type;
				typedef Prototypes::Solver_Settings<type_of(Solver_Settings)> solution_settings_itf;
				typedef Pair_Associative_Container<type_of(Synthesis_Zone_Collection)> zones_itf;
				typedef Prototypes::Synthesis_Zone<get_mapped_component_type(type_of(Synthesis_Zone_Collection))> zone_itf;

				typedef Multidimensional_Random_Access_Array< type_of(Target_Joint_Distribution),value_type> mway_itf;
				typedef Multidimensional_Random_Access_Array< type_of(Target_Marginal_Distribution),value_type> marg_itf;
				typedef Pair_Associative_Container< type_of(Sample_Data)> sample_itf;
				typedef Household_Components::Prototypes::Household_Properties <get_mapped_component_type(type_of(Sample_Data))>  hh_unit_itf;
				typedef Random_Access_Sequence< typename hh_unit_itf::get_type_of(Persons_Container)> person_sample_itf;
				typedef Person_Components::Prototypes::Person_Properties <get_component_type(person_sample_itf)>  person_unit_itf;

				#pragma endregion
				

				//==========================================================
				// Get the references to the collections from the region
				solution_settings_itf& settings = this->template Solver_Settings<solution_settings_itf&>();				/* Get Solution settings		*/
				zones_itf& zones_collection		= this->template Synthesis_Zone_Collection<zones_itf&>();				/* Zones Collection				*/
				mway_itf& mway					= base_itf->template Target_Joint_Distribution<mway_itf&>();			/* Joint Distribution			*/
				marg_itf& marg					= base_itf->template Target_Marginal_Distribution<marg_itf&>();			/* Marginal Distribution		*/
				mway_itf& mway_per				= base_itf->template Target_Person_Joint_Distribution<mway_itf&>();		/* Joint Distribution			*/
				marg_itf& marg_per				= base_itf->template Target_Person_Marginal_Distribution<marg_itf&>();	/* Marginal Distribution		*/
				sample_itf* sample				= base_itf->template Sample_Data<sample_itf*>();						/* Population Sample Collection	*/


				//======================================================================
				// MAIN SYNTHESIS ROUTINE. 
				//----------------------------------------------------------
				// A. Fit the region distribution to region marginal for the household (true) and person (false) levels
				base_itf->template Fit_Joint_Distribution_To_Marginal_Data<hh_unit_itf>();
				base_itf->template Fit_Joint_Distribution_To_Marginal_Data<person_unit_itf>();

				//----------------------------------------------------------
				// B. Cycle through zones and solve for each
				for (typename zones_itf::iterator zone_itr = zones_collection.begin(); zone_itr != zones_collection.end(); ++zone_itr)
				{
					zone_itf* zone = zone_itr->second;												/* Get interface to zone */
					zone->template Target_Joint_Distribution<mway_itf&>().Copy(mway);				/*1a. push region distribution to zone */
					zone->template Target_Person_Joint_Distribution<mway_itf&>().Copy(mway_per);	/*1b. push region distribution to zone */
					zone->template Fit_Joint_Distribution_To_Marginal_Data<hh_unit_itf>();			/*2a. Fit the zone hh distribution to the zone marginals */
					zone->template Fit_Joint_Distribution_To_Marginal_Data<person_unit_itf>();		/*2a. Fit the zone person distribution to the zone marginals */
					zone->template Integerize_Joint_Distribution<hh_unit_itf>();			/*2a. Fit the zone hh distribution to the zone marginals */
					zone->template Select_Synthetic_Population_Units<sample_itf*>(sample);			/*3. Select households from regional sample into the zone synthesized sample */

				}	
				cout <<endl<<"Region " << this->template ID<uint>() <<" is complete."<<endl;
			}
		};
		implementation struct Polaris_Synthesis_Region_Implementation_Full : public Polaris_Synthesis_Region_Implementation_Simple<MasterType,INHERIT(Polaris_Synthesis_Region_Implementation_Full)>
		{
			typedef typename Polaris_Component<MasterType,INHERIT(Polaris_Synthesis_Region_Implementation_Full),Execution_Object>::Component_Type ComponentType;	
		};
	}
}

