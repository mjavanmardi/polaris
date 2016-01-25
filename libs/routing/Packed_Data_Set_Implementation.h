#pragma once
#include "Packed_Data_Set.h"

namespace polaris
{
	implementation struct Packed_Data_Set_Implementation : public Polaris_Component<MasterType,INHERIT(Packed_Data_Set_Implementation)>
	{
		Packed_Data_Set_Implementation():_field_counter(0),_data_lookup(nullptr),_field_lookup(nullptr),_ordered_data_lookup(nullptr),_ordered_field_lookup(nullptr){}

		typedef Packed_Data_Set_Implementation packed_data_set_type;

		typedef size_t field_size;
		typedef int field_index;
		typedef int edge_id;
		typedef size_t field_hash_code;

		template<typename Attributes_Type>
		Field_Record_Header<Attributes_Type>* Get(unsigned int edge_id)
		{
			if(!_field_hash_to_index->count( typeid(Attributes_Type).hash_code() ))
			{
				THROW_WARNING("Field not found: " << typeid(Attributes_Type).name());
				return nullptr;
			}

			field_index index = (*_field_hash_to_index)[typeid(Attributes_Type).hash_code()];
			
			if(!_data_navigation->count(edge_id))
			{
				THROW_WARNING("No records for edge: " << edge_id);
				return nullptr;
			}

			Field_Record_Header<NT>* field_record_header = (*_data_navigation)[edge_id][index];

			if(field_record_header == nullptr)
			{
				THROW_WARNING("No records for field: " << typeid(Attributes_Type).name() << " edge: " << edge_id);
				return nullptr;
			}

			return (Field_Record_Header<Attributes_Type>*)field_record_header;
		}

		template<typename Field_Type>
		void Add_New_Data_Field()
		{
			_ordered_field_lookup->push_back( typeid(Field_Type).hash_code() );

			(*_field_lookup)[ typeid(Field_Type).hash_code() ] = pair<field_index,field_size>(_field_counter,sizeof(Field_Type));

			++_field_counter;
		}

		template<typename Field_Type>
		void Add_New_Data_Record(unsigned int data_identifier, Field_Type* record)
		{
			// Create data containers if not otherwise created
			if( _data_lookup == nullptr ) _data_lookup = new std::unordered_map< edge_id, std::vector< pair< field_hash_code, std::vector< void* > > > >();
			if( _field_lookup == nullptr ) _field_lookup = new std::unordered_map< field_hash_code, pair<field_index,field_size> >();
			if( _ordered_data_lookup == nullptr ) _ordered_data_lookup = new std::vector<edge_id>();
			if( _ordered_field_lookup == nullptr ) _ordered_field_lookup = new std::vector<field_hash_code>();


			// If user is providing a new field, add it
			if( !_field_lookup->count( typeid(Field_Type).hash_code() ) ) Add_New_Data_Field<Field_Type>();

			// order the input data identifiers so user can optimize memory locality
			_ordered_data_lookup->push_back( data_identifier );

			unsigned int field_index = (*_field_lookup)[typeid(Field_Type).hash_code()].first;

			std::vector< pair< field_hash_code, std::vector< void* >> >& fields = (*_data_lookup)[data_identifier];


			// build out all fields up to the one added
			for(int i=0;i<field_index + 1;i++)
			{
				fields.push_back( pair< field_hash_code, std::vector< void* >>((*_ordered_field_lookup)[i],std::vector< void* >()) );
			}


			// get field corresponding to data of interest
			std::vector< void* >& data_records = fields[field_index].second;


			// add the data pointer
			data_records.push_back(record);
		}

		Packed_Data_Set<packed_data_set_type>* Compile()
		{
			// WRONG! this should allocate one big array per field and copy to it, not many small arrays per edge! The current implementation defeats the entire purpose of this class!



			/*
			_field_hash_to_index = new std::unordered_map< field_hash_code, field_index >();

			// create a mapping between hash code and index
			for(std::unordered_map< field_hash_code, pair<field_index,field_size> >::iterator itr = _field_lookup->begin();itr!= _field_lookup->end();itr++)
			{
				(*_field_hash_to_index)[ itr->first ] = itr->second.first;
			}

			_data_navigation = new std::unordered_map< edge_id, Field_Record_Header<NT>** >();

			// go over the edges in the order they were submitted
			for(std::vector< edge_id >::iterator itr = _ordered_data_lookup->begin();itr != _ordered_data_lookup->end();itr++)
			{
				// for each edge, allocate enough space to reference all fields submitted
				(*_data_navigation)[ *itr ] = new Field_Record_Header<NT>*[_field_counter];

				Field_Record_Header<NT>** field_record_itr = (*_data_navigation)[ *itr ];

				// nullify all fields, this will be used to signify cases where an edge did not have a particular field submitted
				for(unsigned int i=0; i < _field_counter; i++) field_record_itr[i] = nullptr;

				// retrieve the data record submissions for this edge and loop over field submissions
				std::vector< pair< field_hash_code, std::vector< void* > > >& field_lookup = (*_data_lookup)[ *itr ];

				for(std::vector< pair< field_hash_code, std::vector< void* > > >::iterator field_itr = field_lookup.begin();field_itr!=field_lookup.end();field_itr++)
				{
					// retrieve records from a particular field for this edge

					pair< field_hash_code, std::vector< void* > >& field = *field_itr;

					// look up field attributes information necessary to determine the size of the linear allocation to contain all records

					pair<field_index,field_size>& field_info = (*_field_lookup)[field.first];

					unsigned int records_size = field_info.second * field.second.size() + sizeof(Field_Record_Header<NT>);

					field_record_itr[field_info.first] = (Field_Record_Header<NT>*) (new char[records_size]);

					Field_Record_Header<NT>* field_record_header = field_record_itr[field_info.first];

					new (field_record_header) Field_Record_Header<NT>(field.second.size(),(unsigned int)field.first);

					char* field_record_iterator = (char*)(field_record_header + 1);

					// copy over each record

					for(std::vector< void* >::iterator record_itr = field.second.begin();record_itr!=field.second.end();record_itr++)
					{
						memcpy(field_record_iterator,*record_itr,field_info.second);

						field_record_iterator+=field_info.second;
					}
				}

			}
			

			delete _data_lookup;
			delete _ordered_data_lookup;
			delete _field_lookup;
			delete _ordered_field_lookup;

			*/
			return ((Packed_Data_Set<packed_data_set_type>*)this);
		}

		unsigned int _field_counter;



		std::unordered_map< field_hash_code, field_index >* _field_hash_to_index;
		std::unordered_map< edge_id, Field_Record_Header<NT>** >* _data_navigation;

		std::unordered_map< edge_id, std::vector< pair< field_hash_code, std::vector< void* > > > >* _data_lookup;
		std::vector< edge_id >* _ordered_data_lookup;

		std::unordered_map< field_hash_code, pair<field_index,field_size> >* _field_lookup;
		std::vector< field_hash_code >* _ordered_field_lookup;
	};
}