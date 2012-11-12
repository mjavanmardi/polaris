#pragma once
#include "Memory_Engine.h"

struct Exchange_Data
{
	Exchange_Data():exchange_interval(0),num_messages(0),comm_lock(0),current_exchange(0),next_exchange(INT_MAX),next_next_exchange(INT_MAX){};
	
	Byte_Balloon send_buffer;
	Byte_Balloon recv_buffer;
	
	int num_messages;
	int current_exchange;
	int next_exchange;
	int next_next_exchange;
	int exchange_interval;
	
	volatile long comm_lock;
};

struct Processing_Parcel
{
	int message_offset;
	int num_messages;
	volatile long parcel_lock;
};

struct Process_Data
{
	Process_Data():process_data_lock(0),num_messages_total(0),num_messages_per_parcel(0){};
	
	void Reset()
	{
		process_data_lock=0;
		process_buffer.clear();
		processing_parcels.clear();
		num_messages_total=0;
		num_messages_per_parcel=0;
		num_parcels=0;
	}
	
	volatile long process_data_lock;
	
	Byte_Balloon process_buffer;
	
	Byte_Balloon processing_parcels;
	
	int num_messages_total;
	int num_messages_per_parcel;
	int num_parcels;
};

struct Exchange_Information
{
	Exchange_Information():partition_exchange_data(nullptr),current_exchange(0),next_exchange(INT_MAX){};
	
	Exchange_Data* partition_exchange_data;
	Exchange_Data* thread_local_exchange_data[_num_threads];
	
	int current_exchange;
	int next_exchange;
	
	Process_Data process_data;
};

static Exchange_Information exchange_information;

struct Head_Message_Base
{
	int length;
	int current_iteration;
	int suggested_next_exchange;
	int suggested_exchange_interval;
	int num_messages;
};

struct Message_Base
{
	int type_index;
	int length;
};

template<typename ComponentType>
void Broadcast_Message(void* msg,int msg_length,int next_exchange)
{
	for(int i=0;i<_num_partitions;i++)
	{
		if(i==_host_rank) continue;
		
		Exchange_Data* thread_exchange_data=&exchange_information.thread_local_exchange_data[_thread_id][i];

		// cannot affect current_exchange, but will help determine next_exchange
		
		if(next_exchange < thread_exchange_data->next_exchange)
		{
			thread_exchange_data->next_exchange=next_exchange;
		}
		
		/*
		if(next_exchange < thread_exchange_data->next_exchange)
		{
			thread_exchange_data->next_next_exchange=thread_exchange_data->next_exchange;
			thread_exchange_data->next_exchange=next_exchange;
		}
		else if(next_exchange == thread_exchange_data->next_exchange)
		{
			// do nothing
		}
		else if(next_exchange < thread_exchange_data->next_next_exchange)
		{
			thread_exchange_data->next_next_exchange=next_exchange;
		}
		*/
		
		//cout << "Thread status " << destination << ": " << thread_exchange_data->next_exchange << "," << thread_exchange_data->next_next_exchange << endl;
		
		// number of messages for this thread / destination combination
		
		thread_exchange_data->num_messages++;
		
		char* mbuf=thread_exchange_data->send_buffer.allocate(msg_length+sizeof(Message_Base));
		
		((Message_Base*)mbuf)->length=msg_length+sizeof(Message_Base);
		((Message_Base*)mbuf)->type_index=ComponentType::component_index;

		memcpy(mbuf+sizeof(Message_Base),msg,msg_length);
	}
}


void Initialize_Exchange_Interval(unsigned int partition,unsigned int exchange_interval)
{
	Exchange_Data* partition_exchange_data=&exchange_information.partition_exchange_data[partition];
	
	partition_exchange_data->exchange_interval=exchange_interval;
	
	int interval_exchange = _iteration + (partition_exchange_data->exchange_interval - _iteration%partition_exchange_data->exchange_interval);
	
	for(int i=0;i<_num_threads;i++)
	{
		Exchange_Data* thread_exchange_data=&exchange_information.thread_local_exchange_data[i][partition];
		
		thread_exchange_data->exchange_interval=exchange_interval;
		
		if(interval_exchange < thread_exchange_data->next_exchange)
		{
			thread_exchange_data->next_exchange=interval_exchange;
		}
	}
	
	if(interval_exchange < partition_exchange_data->next_exchange)
	{
		partition_exchange_data->next_next_exchange = partition_exchange_data->next_exchange;
		partition_exchange_data->next_exchange = interval_exchange;
	}
	else if(interval_exchange == partition_exchange_data->next_exchange)
	{
		// do nothing
	}
	else if(interval_exchange < partition_exchange_data->next_next_exchange)
	{
		partition_exchange_data->next_next_exchange = interval_exchange;
	}

	if(partition_exchange_data->next_exchange < exchange_information.next_exchange)
	{
		exchange_information.next_exchange=partition_exchange_data->next_exchange;
	}	
}

template<typename ComponentType>
void Send_Message(void* msg,unsigned int msg_length,unsigned int destination,unsigned int next_exchange=INT_MAX)
{
	if(destination >= _num_partitions || next_exchange < _iteration) return;
	
	Exchange_Data* thread_exchange_data=&exchange_information.thread_local_exchange_data[_thread_id][destination];
	
	// cannot affect current_exchange, but will help determine next_exchange
	
	if(next_exchange < thread_exchange_data->next_exchange)
	{
		thread_exchange_data->next_exchange=next_exchange;
	}
	
	// number of messages for this thread / destination combination
	
	thread_exchange_data->num_messages++;
	
	char* mbuf=thread_exchange_data->send_buffer.allocate(msg_length+sizeof(Message_Base));
	
	((Message_Base*)mbuf)->length=msg_length+sizeof(Message_Base);
	((Message_Base*)mbuf)->type_index=ComponentType::component_index;

	memcpy(mbuf+sizeof(Message_Base),msg,msg_length);
}