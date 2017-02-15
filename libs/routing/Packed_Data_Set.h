#pragma once
#include "Interactive_Graph.h"

namespace polaris
{
	template<typename Attributes_Type = NT>
	struct Field_Record_Header
	{
		Field_Record_Header(unsigned int num_records,unsigned int type_identifier):_num_records(num_records),_type_identifier(type_identifier){}

		unsigned int _num_records;
		unsigned int _type_identifier;
		Attributes_Type* begin(){ return (Attributes_Type*)(this+1); }
	};
	
	template<>
	struct Field_Record_Header<NT>
	{
		Field_Record_Header(unsigned int num_records,unsigned int type_identifier):_num_records(num_records),_type_identifier(type_identifier){}

		unsigned int _num_records;
		unsigned int _type_identifier;
		void* begin();
	};

	prototype struct Packed_Data_Set ADD_DEBUG_INFO
	{
		tag_as_prototype;
		
		template<typename Attributes_Type>
		Field_Record_Header<Attributes_Type>* Get(unsigned int edge_id){ return this_component()->Get<Attributes_Type>(edge_id); }

		//template<typename Attributes_Type = NT>
		//struct Field_Record : public Attributes_Type
		//{
		//	Field_Record* next(){ this_component()->next(); }
		//};

		//template<typename Attributes_Type = NT>
		//struct Field_Record_Header
		//{
		//	int num_records;
		//	int type_identifier;
		//	Attributes_Type* begin();
		//};
		//
		//template<>
		//struct Field_Record_Header<NT>
		//{
		//	int num_records;
		//	int type_identifier;
		//	void* begin();
		//};

		//struct Data_Record_Header
		//{
		//	int data_identifier;
		//	int num_fields;
		//	Field_Record_Header<NT>* begin();
		//};

		//struct All_Data
		//{
		//	template<typename Attributes_Type>
		//	Field_Record_Header<Attributes_Type>* Get<Attributes_Type>(edge_id);
		//	
		//	Data_Record_Header* begin;
		//};
	};

	prototype struct Packed_Data_Set_Assembler ADD_DEBUG_INFO
	{
		tag_as_prototype;

		typedef typename ComponentType::packed_data_set_type packed_data_set_type;

		// Approximate data layout - without alignments to cache line
		//		[(DR:FR_A---,FR_B--)(DR:FR_A-,FR_B---)]
		//DR stride----------------->
		//	FR stride------->

		template<typename Field_Type>
		void Add_New_Data_Record(unsigned int data_identifier, Field_Type* record){ this_component()->Add_New_Data_Record<Field_Type>(data_identifier,record); }

		Packed_Data_Set<packed_data_set_type>* Compile(){ return this_component()->Compile(); }
	};
}