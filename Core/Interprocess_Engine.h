#pragma once
#include "Interprocess_Structures.h"

typedef void (*Handler)(void*,char*);

struct Communication_Object
{
	__forceinline void Swap_Handler(Handler new_handler)
	{
		handler_register=new_handler;
	}
	
	template<typename ComponentType>
	void Load_Register(Handler handler)
	{
		handler_register=handler;
	}

	Handler handler_register;
};

#ifdef WINDOWS
class Interprocess_Engine
{

};
#else

class Interprocess_Engine
{
public:
	Interprocess_Engine():is_head_node(false)
	{
		ipc_lock=0;
		ipc_threads_counter_A=0;
		ipc_threads_counter_B=0;

		Initialize();
	}
	
	enum Parse_Phase
	{
		NEW_WORD,
		MID_WORD,
		END_WORD,
		END_PARSE
	};
	
	void Initialize()
	{
		//cout << "hello world!" << endl;
		
		identify_job(job_name);
		
		//cout << "job name is: " << job_name << endl;
		
		
		
		identify_self(hostname,is_head_node);
		
		//cout << "hostname is: " << hostname << endl;
		//cout << "is_head_node is: " << is_head_node << endl;
		
		
		
		identify_others(job_name,all_names);
		
		//vector<string>::iterator itr;
		//for(itr=all_names.begin();itr!=all_names.end();itr++) cout << "one other name is: " << (*itr) << endl;
		
		
		
		node_names_to_ip_addresses(all_names,all_ips,hostname,host_ip);
		
		//cout << "host_ip is: " << host_ip << endl;
		//for(itr=all_ips.begin();itr!=all_ips.end();itr++) cout << "one other ip is: " << (*itr) << endl;
		
		
		determine_ranks(all_ips,rank_to_ip,host_ip,host_rank);
		//cout << "host_rank is: " << host_rank << endl;
		
		//unordered_map<int,string>::iterator mitr;
		
		//for(mitr=rank_to_ip.begin();mitr!=rank_to_ip.end();mitr++)
		//{
			//cout << "one (rank,ip) combo is: (" << mitr->first << "," << mitr->second << ")" << endl;
		//}
		
		
		if(is_head_node)
		{
			stringstream s;
			
			unordered_map<int,string>::iterator mitr;
			
			for(mitr=rank_to_ip.begin();mitr!=rank_to_ip.end();mitr++)
			{
				if(mitr->first!=host_rank)
				{
					s << "ssh " << mitr->second << " 'cd POLARIS && export PBS_JOBID=" << job_name.c_str() << " && module load compiler/gcc471 && ./Core > Out_" << mitr->first << ".txt &' ";
					
					//cout << "opening child process: " << s.str() << endl;

					popen(s.str().c_str(),"r");
					
					s.str("");
				}
			}
		}
		
		connect_all_to_all();
		
		//msg_all_to_all();
		
		num_partitions=rank_to_ip.size();
		
		//cout << "Number of partitions: " << num_partitions << endl;
		
		//cout << "Initializing exchange data" << endl;
		
		for(int i=0;i<num_threads;i++)
		{
			exchange_information.thread_local_exchange_data[i]=new Exchange_Data[num_partitions];
		}
		
		exchange_information.partition_exchange_data=new Exchange_Data[num_partitions];
		
		// set an initial max message size for the exchange
		
		for(int i=0;i<num_partitions;i++)
		{
			exchange_information.partition_exchange_data[i].recv_buffer.allocate(Max_Message_Size);
		}
		
		//cout << "Done initializing exchange data" << endl;
	}
	
	void identify_job(string& jobname)
	{
		char buffer[255];for(int i=0;i<255;i++) buffer[i]=0;
		
		FILE* from_cluster;
		
		from_cluster=popen("echo $PBS_JOBID", "r");	
		
		fgets(buffer,255,from_cluster);	
		
		pclose(from_cluster);
		
		jobname=string(buffer);
		
		jobname.resize(jobname.size()-1);
	}
	
	void identify_self(string& host_name,bool& is_headnode)
	{
		char buffer[255];for(int i=0;i<255;i++) buffer[i]=0;
		
		FILE* from_cluster;
		
		from_cluster=popen("hostname", "r");
		
		fgets(buffer,255,from_cluster);	
		
		pclose(from_cluster);
		
		parse_node_name(host_name,buffer);
		
		
		
		for(int i=0;i<255;i++) buffer[i]=0;
		
		from_cluster=popen("echo $PBS_JOBNAME", "r");
		
		fgets(buffer,255,from_cluster);	
		
		pclose(from_cluster);
		
		is_headnode=(buffer[0]!='\n');
	}
	
	void identify_others(string& jobname,vector<string>& allnames)
	{
		char buffer[255];for(int i=0;i<255;i++) buffer[i]=0;
		
		FILE* from_cluster;
		
		stringstream s;
		
		s << "/mnt/lustre/bin/jobnodes " << jobname.c_str();
		
		from_cluster=popen(s.str().c_str(), "r");	
	
		fgets(buffer,255,from_cluster);	
	
		pclose(from_cluster);
		
		char* buf=&buffer[0];
		
		string name;
		
		while(true)
		{
			buf=parse_node_name(name,buf);
			
			allnames.push_back(name);
			
			name.clear();
			
			if(buf==nullptr) break;
		}
	}

	void node_names_to_ip_addresses(vector<string>& allnames,vector<string>& allips,string& host_name,string& hostip)
	{
		stringstream s;	
		
		vector<string>::iterator itr;
		
		char buffer[255];

		FILE* from_cluster;
		
		for(itr=allnames.begin();itr!=allnames.end();itr++)
		{
			for(int i=0;i<255;i++) buffer[i]=0;
			
			s.str("");
			
			s << "nslookup ";
			s << itr->c_str();
			s << "-ipinfin | grep Address | tail -1 | awk '{ print $2 }'";
			
			from_cluster=popen(s.str().c_str(),"r");
			
			fgets(buffer,255,from_cluster);
			
			pclose(from_cluster);
			
			string str(buffer);
			str.resize(str.size()-1);
			
			if((*itr)==host_name) hostip=str;
			
			allips.push_back(str);
		}
	}
	
	void determine_ranks(vector<string>& allips, unordered_map<int,string>& ranktoip,string& hostip,int& rank)
	{
		vector<string>::iterator itr;
		
		int rank_counter=0;
		
		for(itr=allips.begin();itr!=allips.end();itr++)
		{
			ranktoip[rank_counter]=string((*itr));
			if((*itr)==hostip) rank=rank_counter;
			++rank_counter;
		}
	}

	char* parse_node_name(string& node_name,char* buffer)
	{
		Parse_Phase parse_phase=NEW_WORD;
		
		while(true)
		{
			if(parse_phase==MID_WORD && (*buffer)==',') parse_phase=END_WORD;
			else if(parse_phase==MID_WORD && (*buffer)=='\n') parse_phase=END_PARSE;
			else if((*buffer)=='\n') break;
			
			if(parse_phase==NEW_WORD)
			{
				node_name.push_back((*buffer));
				parse_phase=MID_WORD;
			}
			else if(parse_phase==MID_WORD)
			{
				node_name.push_back((*buffer));
			}
			else if(parse_phase==END_WORD)
			{
				node_name=node_name;
				parse_phase=NEW_WORD;
				buffer++;
				break;
			}
			else if(parse_phase==END_PARSE)
			{
				node_name=node_name;
				buffer=nullptr;
				break;
			}

			buffer++;
		}

		return buffer;
	}
	
	void connect_all_to_all()
	{
		int listening_socket=socket(AF_INET,SOCK_STREAM,0);
		
		//cout << "listener: " << listening_socket << endl;
		
		sockaddr_in server_socket_address;
		server_socket_address.sin_family = AF_INET;
		server_socket_address.sin_addr.s_addr=INADDR_ANY;
		server_socket_address.sin_port = htons(40111);
		
		int bound=bind(listening_socket,(struct sockaddr*)&server_socket_address,sizeof(server_socket_address));
		
		//cout << "bound: " << bound << endl;
		
		int listening=listen(listening_socket,10);
		
		SLEEP(1);
		
		//cout << "listening: " << listening << endl;	

		for(int i=0;i<(rank_to_ip.size()-1);i++)
		{
			if(i>=host_rank)
			{
				int connecting_socket=socket(AF_INET,SOCK_STREAM,0);
				
				//cout << "connecter socket: " << connecting_socket << endl;
				
				sockaddr_in server_socket_address;
				
				server_socket_address.sin_family = AF_INET;
				inet_aton(rank_to_ip[i+1].c_str(),(in_addr*)&server_socket_address.sin_addr.s_addr);
				server_socket_address.sin_port = htons(40111);
				
				//cout << "connecting from: " << rank_to_ip[host_rank].c_str() << " to " << rank_to_ip[i+1].c_str() << endl;
				
				int connected=connect(connecting_socket,(struct sockaddr*) &server_socket_address, sizeof(server_socket_address) );
				
				//cout << "connected: " << connected << endl;
				
				rank_to_socket[i+1]=connecting_socket;
			}
			if(i<host_rank)
			{
				SLEEP(1);
				
				int accepting_socket;
				
				accepting_socket=accept(listening_socket,NULL,NULL);
				
				//cout << "accepting: " << accepting_socket << endl;
				
				rank_to_socket[i]=accepting_socket;
			}
		}
		
		//unordered_map<int,int>::iterator mitr;
		
		//for(mitr=rank_to_socket.begin();mitr!=rank_to_socket.end();mitr++)
		//{
		//	cout << "one (rank,socket) combo is: (" << mitr->first << "," << mitr->second << ")" << endl;
		//}
	}
	
	void msg_all_to_all()
	{
		char msg[255];
		for(int i=0;i<255;i++) msg[i]='\0';
		stringstream s;
		
		for(int sender=0;sender<(rank_to_ip.size());sender++)
		{
			if(sender==host_rank)
			{
				for(int receiver=0;receiver<(rank_to_ip.size());receiver++)
				{
					if(receiver!=host_rank)
					{
						//cout << "sending to " << receiver << endl;
						
						s << "Hello from " << host_rank << "\0";
						
						send(rank_to_socket[receiver],s.str().c_str(),s.str().size(),0);
						
						//cout << "sent" << endl;
						
						s.str("");
					}
				}
			}
			else
			{
				//cout << "receiving from " << sender << endl;
				
				recv(rank_to_socket[sender],msg,255,0);
				
				//cout << msg << endl;			
			}
		}
	}
	
	void Merge_Messages()
	{
		// first, merge all of the messages by partition, use one thread for now in order to not confuse the FSB
		
		for(int i=0;i<num_partitions;i++)
		{
			Exchange_Data* partition_exchange_data=&exchange_information.partition_exchange_data[i];
			
			// consolidate message buffers should the exchange be happening this iteration
			
			if(partition_exchange_data->current_exchange==iteration)
			{
				//the head of the partition message buffer is reserved to contain the msg header
				
				char* hbuf=partition_exchange_data->send_buffer.allocate(sizeof(Head_Message_Base));
				
				((Head_Message_Base*)hbuf)->num_messages=0;
				
				for(int j=0;j<num_threads;j++)
				{
					Exchange_Data* thread_exchange_data=&exchange_information.thread_local_exchange_data[j][i];
					
					//cout << "\t\t\tThread status " << i << ": " << thread_exchange_data->next_exchange << "," << thread_exchange_data->next_next_exchange << endl;
					
					// reconcile partition need to exchange with local need to exchange, current has been "set in stone" already
					
					if(thread_exchange_data->next_exchange < partition_exchange_data->next_exchange)
					{
						partition_exchange_data->next_next_exchange=partition_exchange_data->next_exchange;
						partition_exchange_data->next_exchange=thread_exchange_data->next_exchange;
					}
					else if(thread_exchange_data->next_exchange == partition_exchange_data->next_exchange)
					{
						// do nothing
					}
					else if(thread_exchange_data->next_exchange < partition_exchange_data->next_next_exchange)
					{
						partition_exchange_data->next_next_exchange=thread_exchange_data->next_exchange;
					}
					
					//cout << "\t\t\tpartition exchange status " << i << ": " << partition_exchange_data->next_exchange << "," << partition_exchange_data->next_next_exchange << endl;
					
					((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->num_messages+=thread_exchange_data->num_messages;
					
					char* mbuf=partition_exchange_data->send_buffer.allocate(thread_exchange_data->send_buffer.size());
					
					memcpy(mbuf,thread_exchange_data->send_buffer.buffer(),thread_exchange_data->send_buffer.size());
					
					// done with thread local send buffer
					
					thread_exchange_data->send_buffer.clear();
					thread_exchange_data->num_messages=0;
				}

				//the head of the partition message buffer gets length and suggested next exchange information
				
				((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->length=partition_exchange_data->send_buffer.size();
				((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->suggested_next_exchange=partition_exchange_data->next_exchange;
								
				//cout << "\t\tTo Partition " << i << ": " << ((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->length << "," << ((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->suggested_next_exchange << endl;
			}
		}
	}
	
	void Send_Receive()
	{
		for(int i=0;i<num_partitions;i++)
		{
			Exchange_Data* partition_exchange_data=&exchange_information.partition_exchange_data[i];
			
			// one thread per exchange
			
			long process = AtomicIncrement(&partition_exchange_data->comm_lock);
			
			//cout << "\t\tstatus of exchange with " << i << ": " << (bool)(partition_exchange_data->current_exchange==iteration) << endl;
			
			if(process == 1)
			{
				if(partition_exchange_data->current_exchange==iteration)
				{
					//cout << "\t\tmid-check " << i << ":" << host_rank << endl;
					
					// only process exchanges which are happening
					
					//cout << "\t\t\t" << "send stats: " << ((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->length << "," << ((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->num_messages << "," << ((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->suggested_next_exchange << endl;
					
					// initiate sends in rank order for those ranks which are smaller than you and receives for those ranks larger than you, self send is just a memcpy
					
					if(i < host_rank)
					{
						//cout << "\t\t\t" << host_rank << " sending first to " << i << ": " << partition_exchange_data->send_buffer.size() << endl;
						
						send(rank_to_socket[i],partition_exchange_data->send_buffer.buffer(),partition_exchange_data->send_buffer.size(),0);
						
						recv(rank_to_socket[i],partition_exchange_data->recv_buffer.buffer(),Max_Message_Size,0);
					}
					else if(i == host_rank)
					{
						memcpy(partition_exchange_data->recv_buffer.buffer(),partition_exchange_data->send_buffer.buffer(),partition_exchange_data->send_buffer.size());
					}
					else
					{
						//cout << "\t\t\t" << host_rank << " receiving first from " << i << ": " << partition_exchange_data->send_buffer.size() << endl;
						
						recv(rank_to_socket[i],partition_exchange_data->recv_buffer.buffer(),Max_Message_Size,0);
						
						send(rank_to_socket[i],partition_exchange_data->send_buffer.buffer(),partition_exchange_data->send_buffer.size(),0);
					}
					
					// done with partition send buffer
					
					partition_exchange_data->send_buffer.clear();
					
					// process the header to update the exchange info with this partition, the two will agree on the minimum of their respective suggestions
										
					Head_Message_Base* head_msg=(Head_Message_Base*)partition_exchange_data->recv_buffer.buffer();
					
					//cout << "\t\t\t" << "received message: " << head_msg->length << "," << head_msg->num_messages << "," << head_msg->suggested_next_exchange << endl;
					
					if(head_msg->suggested_next_exchange < partition_exchange_data->next_exchange)
					{
						partition_exchange_data->next_next_exchange = partition_exchange_data->next_exchange;
						partition_exchange_data->next_exchange = head_msg->suggested_next_exchange;
					}
					else if(head_msg->suggested_next_exchange == partition_exchange_data->next_exchange)
					{
						// do nothing
					}
					else if(head_msg->suggested_next_exchange < partition_exchange_data->next_next_exchange)
					{
						partition_exchange_data->next_next_exchange = head_msg->suggested_next_exchange;
					}
					
					// there is no more need to know about the current_exchange so we can safely advance exchange
					
					partition_exchange_data->current_exchange = partition_exchange_data->next_exchange;
					partition_exchange_data->next_exchange = partition_exchange_data->next_next_exchange;
					partition_exchange_data->next_next_exchange = INT_MAX;
					
					for(int j=0;j<num_threads;j++)
					{
						// threads become as critical as the partition_exchange
						
						exchange_information.thread_local_exchange_data[j][i].next_exchange = partition_exchange_data->next_exchange;
					}
					
					//cout << "\t\t\t" << "status of exchange with " << i << ": " << partition_exchange_data->current_exchange << "," << partition_exchange_data->next_exchange << "," << partition_exchange_data->next_next_exchange << endl;
					
					if(head_msg->num_messages > 0)
					{
						// load up the process buffer
						
						Process_Data* process_data=&exchange_information.process_data;
						
						Byte_Balloon* process_buffer=&process_data->process_buffer;				
						
						while(AtomicExchange(&process_data->process_data_lock,1)) SLEEP(0); // lock the process buffer
							
							process_data->num_messages_total += head_msg->num_messages;
							
							// adjust the message to remove the head, copy
							
							char* mbuf=process_buffer->allocate(head_msg->length - sizeof(Head_Message_Base));
							memcpy( mbuf, (partition_exchange_data->recv_buffer.buffer()+sizeof(Head_Message_Base)), (head_msg->length-sizeof(Head_Message_Base)) );
							
							//cout << "test-: " << *(int*)(process_buffer->buffer()+sizeof(Message_Base)) << endl;
						
						process_data->process_data_lock=0; // unlock the process buffer
					}
					
					// partition recv buffer no longer needed
					
					partition_exchange_data->recv_buffer.clear();
				}
			}
			
			if(process == num_threads)
			{
				partition_exchange_data->comm_lock=0;
			}
		}
	}

	void Update_Global_Exchange_Schedule()
	{
		// quickly sync the partition exchange iteration before processing
		
		Exchange_Data* partition_exchange_data;
		
		for(int i=0;i<num_partitions;i++)
		{
			partition_exchange_data=&exchange_information.partition_exchange_data[i];
			
			if(partition_exchange_data->current_exchange < exchange_information.next_exchange)
			{
				exchange_information.next_exchange=partition_exchange_data->current_exchange;
			}
		}

		//cout << "\t\t\t" << "status of partition exchange: " << exchange_information.current_exchange << endl;
	}
	
	void Build_Parcels()
	{
		Process_Data* process_data=&exchange_information.process_data;
		
		//cout << "\t\t" << "total received: " << process_data->num_messages_total << endl;
		
		if(process_data->num_messages_total==0)
		{
			//nothing to do
			return;
		}
		
		// first determine roughly how many threads per message
		
		int msg_ratio=process_data->num_messages_total/num_threads;
		
		//cout << "\t\t" << "msg ratio: " << msg_ratio << endl;
		
		int min_parcels;
		
		if(msg_ratio==0)
		{
			// small case, more threads than messages; need num_messages parcels with 1 message each
			
			min_parcels=0;
		}
		else if(msg_ratio > 5)
		{
			// large case, at least 5x more messages than threads

			// target is to have 5 parcels per thread, first see if the number of messages will fit evenly into this number of parcels
			
			min_parcels=5*num_threads;
		}
		else
		{
			// mid case, base parcels on the ratio
			
			// target is to have msg_ratio parcels per thread, first see if the number of messages will fit evenly into this number of parcels
			
			min_parcels = msg_ratio*num_threads;			
		}
		
		
		if( (process_data->num_messages_total%min_parcels) == 0 )
		{
			// no extra parcel required, divides evenly
			
			process_data->num_parcels=min_parcels;
		}
		else
		{
			// one extra parcel required
		
			process_data->num_parcels=min_parcels + 1;
		}
		
		// rough messages per parcel
		
		process_data->num_messages_per_parcel = process_data->num_messages_total/min_parcels;
		
		
		
		//cout << "\t\t" << "num parcels: " << process_data->num_parcels << ", num_messages_per_parcel: " << process_data->num_messages_per_parcel << endl;
		
		int message_counter=0;
		
		for(int i=0;i<process_data->num_parcels;i++)
		{
			Processing_Parcel* parcel=(Processing_Parcel*)process_data->processing_parcels.allocate(sizeof(Processing_Parcel));
			
			parcel->parcel_lock=0;
			parcel->message_offset=message_counter;

			message_counter+=process_data->num_messages_per_parcel;
		}
	}
	
	void Process()
	{
		Process_Data* process_data=&exchange_information.process_data;

		//cout << "\t\t" << "num parcels: " << process_data->num_parcels << ", num_messages_per_parcel: " << process_data->num_messages_per_parcel << endl;
		
		// review all created parcels
		
		for(int i=0;i<process_data->num_parcels;i++)
		{
			Processing_Parcel* parcel = (Processing_Parcel*)&process_data->processing_parcels.buffer()[i*sizeof(Processing_Parcel)];
			
			long process = AtomicIncrement(&parcel->parcel_lock);
			
			if(process == 1)
			{
				// thread has reserved this parcel
				
				// first, stride to appropriate offset
				
				int my_message_offset=0;
				
				// start at the beginning (storage of .buffer() is safe here)
				
				char* current_position=process_data->process_buffer.buffer();
				
				//cout << "\t\t\t" << "target offset: " << parcel->message_offset << endl;
				
				for(my_message_offset=0;my_message_offset!=parcel->message_offset;my_message_offset++)
				{	
					//cout << "\t\t\t" << "offsetting: " << ((Message_Base*)current_position)->length << endl;
					
					current_position=current_position+((Message_Base*)current_position)->length;
				}
				
				// may process either a full or partial parcel
				
				int num_messages_to_process=min(process_data->num_messages_per_parcel,process_data->num_messages_total-my_message_offset);
				
				//cout << "\t\t\t" << "num_messages_to_process: " << num_messages_to_process << "; " <<  process_data->num_messages_total << "," << my_message_offset << endl;
				
				// next, begin deciphering and processing messages
				
				for(int i=0;i<num_messages_to_process;i++)
				{
					// locate the type singleton and submit the message

					(*((Communication_Object*)all_components[((Message_Base*)current_position)->type_index])->handler_register)(all_components[((Message_Base*)current_position)->type_index],current_position+sizeof(Message_Base));
					current_position+=((Message_Base*)current_position)->length;
				}
				
			}
			
			if(process == num_threads)
			{
				process_data->process_data_lock=0;
			}
		}
	}

	void Exchange()
	{
		bool exchange_this_iteration = (exchange_information.current_exchange == iteration);
		
		//cout << "entering exchange: " << exchange_this_iteration << endl;
		
		while(exchange_this_iteration)
		{
			while(AtomicExchange(&ipc_lock,1)) SLEEP(0); // lock the ipc engine
				
				// only the first thread gets in
				
				if(++ipc_threads_counter_A == 1)
				{
					//cout << "\tmerging messages" << endl;
					Merge_Messages();
					//cout << "\tmessages merged" << endl;
				}
				
				if(ipc_threads_counter_A==num_threads)
				{
					ipc_threads_counter_A=0;
				}
			
			ipc_lock=0; // unlock the ipc engine
			
			while(AtomicCompareExchange(&ipc_threads_counter_A,0,0)) SLEEP(0); // everyone spins until the counter is reset

			AtomicIncrement(&ipc_threads_counter_B); // have everyone go through the next turnstile
			
			//cout << "\tsending and receiving" << endl;
			
			Send_Receive();

			//cout << "\tdone sending and receiving" << endl;
			
			if(ipc_threads_counter_B==num_threads)
			{
				ipc_threads_counter_B=0;
			}
			
			while(AtomicCompareExchange(&ipc_threads_counter_B,0,0)) SLEEP(0); // everyone spins until the counter is reset
			
			
			
			// at this point all messages from all partitions are contained in the partition process buffer
			
			// first task is to construct reasonable processing parcels
			
			while(AtomicExchange(&ipc_lock,1)) SLEEP(0); // lock the ipc engine
			
				if(++ipc_threads_counter_A == 1)
				{
					//cout << "\tupdating exchange schedule" << endl;			
					Update_Global_Exchange_Schedule();
					//cout << "\tdone updating exchange schedule" << endl;
					
					//cout << "\tbuilding parcels" << endl;	
					Build_Parcels();
					//cout << "\tdone building parcels" << endl;
				}
				
				if(ipc_threads_counter_A==num_threads)
				{
					ipc_threads_counter_A=0;
				}
			
			ipc_lock=0; // unlock the ipc engine
			
			while(AtomicCompareExchange(&ipc_threads_counter_A,0,0)) SLEEP(0); // everyone spins until the counter is reset
			
			
			
			AtomicIncrement(&ipc_threads_counter_B); // have everyone go through the next turnstile

			// finally handle all of the messages
			
			//cout << "\tprocessing" << endl;
			
			Process();
			
			//cout << "\tdone processing" << endl;
			
			if(ipc_threads_counter_B==num_threads)
			{
				ipc_threads_counter_B=0;
				
				// clean up processing

				exchange_information.process_data.Reset();
			}
			
			while(AtomicCompareExchange(&ipc_threads_counter_B,0,0)) SLEEP(0); // everyone spins until the counter is reset
			
			exchange_information.current_exchange=exchange_information.next_exchange;
			exchange_information.next_exchange=INT_MAX;
			
			exchange_this_iteration = (exchange_information.current_exchange == iteration);
			
			//cout << "\t" << "exchange again? " << exchange_this_iteration << endl << endl << endl;
			
			break;
		}
	}
	
	string job_name;
	
	string hostname;
	string host_ip;
	bool is_head_node;

	volatile long ipc_lock;

	volatile long ipc_threads_counter_A;
	volatile long ipc_threads_counter_B;

	vector<string> all_names;
	vector<string> all_ips;
	
	unordered_map<int,string> rank_to_ip;
	unordered_map<int,int> rank_to_socket;
	

};
#endif

static Interprocess_Engine* interprocess_root_ptr;

