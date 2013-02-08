#pragma once
#include "Interprocess_Structures.h"

typedef void (*Communication_Handler)(void*,char*);

///============================================================================
/// Communication_Object - foundational object for communication
/// contains one register which controls which function is used for parsing
///============================================================================

struct Communication_Object
{
	__forceinline void Swap_Communication_Handler(Communication_Handler new_communication_handler)
	{
		communication_handler_register=new_communication_handler;
	}
	
	template<typename ComponentType>
	void Load_Register(Communication_Handler communication_handler)
	{
		communication_handler_register=communication_handler;
	}

	Communication_Handler communication_handler_register;
};

#ifndef INTERPROCESS
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
	
	///============================================================================
	/// Initialize - establish socket connections among all partitions
	///============================================================================

	void Initialize()
	{
		//cout << "hello world!" << endl;
		
		identify_job(job_name);
		
		//cout << "job name is: " << job_name << endl;
		
		
		
		identify_self(hostname,is_head_node);
		
		//cout << "hostname is: " << hostname << endl;
		//cout << "is_head_node is: " << is_head_node << endl;
		
		
		
		identify_others(job_name,all_names);
		
		vector<string>::iterator itr;
		//for(itr=all_names.begin();itr!=all_names.end();itr++) cout << "one hostname is: " << (*itr) << endl;
		
		
		
		node_names_to_ip_addresses(all_names,all_ips,hostname,host_ip);
		
		//cout << "host_ip is: " << host_ip << endl;
		//for(itr=all_ips.begin();itr!=all_ips.end();itr++) cout << "one ip is: " << (*itr) << endl;
		
		
		determine_ranks(all_ips,rank_to_ip,host_ip,_host_rank);
		//cout << "_host_rank is: " << _host_rank << endl;
		
		//unordered_map<int,string>::iterator mitr;
		
		//for(mitr=rank_to_ip.begin();mitr!=rank_to_ip.end();mitr++)
		//{
		//	cout << "one (rank,ip) combo is: (" << mitr->first << "," << mitr->second << ")" << endl;
		//}
		
		if(is_head_node)
		{
			stringstream s;
			
			unordered_map<int,string>::iterator mitr;
			//int k=0;
			for(mitr=rank_to_ip.begin();mitr!=rank_to_ip.end();mitr++)
			{
				if(mitr->first!=_host_rank)
				{
					SLEEP(.01);
					//mitr->second
					s << "ssh " << all_names[mitr->first] << " 'cd POLARIS && export PBS_JOBID=" << job_name.c_str() << " && export NOT_HEAD_NODE=1 && module load compiler/gcc471 && ./Core > Out_" << mitr->first << ".txt &' ";
					
					//cout << "opening child process: " << s.str() << endl;
					
					//FILE* from_cluster;
					
					popen(s.str().c_str(),"r");
					
					s.str("");
					
					//char buffer[255];for(int i=0;i<255;i++) buffer[i]=0;
					
					//fgets(buffer,255,from_cluster);
					
					//cout << "cluster response: " << buffer << endl;
				}
			}
		}
		
		//cout << "done!" << endl;
		
		//SLEEP(1000);
		
		connect_all_to_all();
		
		//msg_all_to_all();
		
		_num_partitions=rank_to_ip.size();
		
		//cout << "Number of partitions: " << _num_partitions << endl;
		
		//cout << "Initializing exchange data" << endl;
		
		for(int i=0;i<_num_threads;i++)
		{
			exchange_information.thread_local_exchange_data[i]=new Exchange_Data[_num_partitions];
		}
		
		exchange_information.partition_exchange_data=new Exchange_Data[_num_partitions];
		
		// set an initial max message size for the exchange
		
		for(int i=0;i<_num_partitions;i++)
		{
			exchange_information.partition_exchange_data[i].recv_buffer.allocate(_Max_Message_Size);
		}
		
		//cout << "Done initializing exchange data" << endl;
	}
	
	///============================================================================
	/// identify_job - query cluster to identify personal job information
	///============================================================================

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
	
	///============================================================================
	/// identify_self - query cluster for personal hostname
	///============================================================================

	void identify_self(string& host_name,bool& is_headnode)
	{
		char buffer[255];for(int i=0;i<255;i++) buffer[i]=0;
		
		FILE* from_cluster;
		
		from_cluster=popen("hostname", "r");
		
		fgets(buffer,255,from_cluster);	
		
		pclose(from_cluster);
		
		parse_node_name(host_name,buffer);
		
		
		
		for(int i=0;i<255;i++) buffer[i]=0;
		
		from_cluster=popen("echo $NOT_HEAD_NODE", "r");
		
		fgets(buffer,255,from_cluster);
		
		pclose(from_cluster);
		
		is_headnode=(buffer[0]=='\n');
	}
	
	///============================================================================
	/// identify_others - query cluster for other nodes involved in this job
	///============================================================================

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
	
	///============================================================================
	/// identify_others - query cluster to convert names to ip addresses
	///============================================================================

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
			//s << "-ib | grep Address | tail -1 | awk '{ print $2 }'";
			
			from_cluster=popen(s.str().c_str(),"r");
			
			fgets(buffer,255,from_cluster);
			
			pclose(from_cluster);
			
			string str(buffer);
			str.resize(str.size()-1);
			
			if((*itr)==host_name) hostip=str;
			
			allips.push_back(str);
		}
	}
	
	///============================================================================
	/// identify_others - compute ranks for self and all others
	///============================================================================

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
	
	///============================================================================
	/// identify_others - take char buffer with node list, convert to string
	///============================================================================

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
	
	///============================================================================
	/// connect_all_to_all - establish connection with all identified nodes
	///============================================================================

	void connect_all_to_all()
	{
		int listening_socket=socket(AF_INET,SOCK_STREAM,0);
		
		//cout << "listener: " << listening_socket << endl;
		
		if(listening_socket==-1)
		{
			cout << "listener creation error: " << strerror(errno) << endl;
		}
		
		sockaddr_in server_socket_address;
		server_socket_address.sin_family = AF_INET;
		server_socket_address.sin_addr.s_addr=INADDR_ANY;
		server_socket_address.sin_port = htons(40111);
		
		int bound=bind(listening_socket,(struct sockaddr*)&server_socket_address,sizeof(server_socket_address));
		
		//cout << "bound: " << bound << endl;

		if(bound==-1)
		{
			cout << "binding error: " << strerror(errno) << endl;
		}
		
		int listening=listen(listening_socket,120);
		
		//SLEEP(1);
		
		//cout << "listening: " << listening << endl;	
		
		
		if(listening==-1)
		{
			cout << "listening error: " << strerror(errno) << endl;
		}
		
		for(int i=0;i<rank_to_ip.size();i++)
		{
			if(i>_host_rank)
			{
				int connecting_socket=socket(AF_INET,SOCK_STREAM,0);
				
				//cout << "connecter socket: " << connecting_socket << endl;
				
				if(connecting_socket==-1)
				{
					cout << "connecter creation error: " << strerror(errno) << endl;
				}
		
				sockaddr_in server_socket_address;
				
				server_socket_address.sin_family = AF_INET;
				inet_aton(rank_to_ip[i].c_str(),(in_addr*)&server_socket_address.sin_addr.s_addr);
				server_socket_address.sin_port = htons(40111);
				
				//cout << "connecting from: " << rank_to_ip[_host_rank].c_str() << " to " << rank_to_ip[i].c_str() << endl;
				
				//SLEEP(1);
				
				int connected=connect(connecting_socket,(struct sockaddr*) &server_socket_address, sizeof(server_socket_address) );
				
				while(connected != 0)
				{
					SLEEP(.5);
					connected=connect(connecting_socket,(struct sockaddr*) &server_socket_address, sizeof(server_socket_address) );
					//cout << rank_to_ip[_host_rank].c_str() << " to " << rank_to_ip[i].c_str() << " connection failed, retrying: " << strerror(errno) << endl;
				}
				
				//cout << "connected: " << connecting_socket << endl;
				
				rank_to_socket[i]=connecting_socket;
			}
			if(i<_host_rank)
			{
				//cout << rank_to_ip[_host_rank].c_str() << " waiting for connection from: " << rank_to_ip[i].c_str() << endl;	

				int accepting_socket=accept(listening_socket,NULL,NULL);
				
				//cout << "accepting: " << accepting_socket << endl;
				
				rank_to_socket[i]=accepting_socket;
			}
		}
		
		unordered_map<int,int>::iterator mitr;
		
		//for(mitr=rank_to_socket.begin();mitr!=rank_to_socket.end();mitr++)
		//{
		//	cout << "one (rank,socket) combo is: (" << mitr->first << "," << mitr->second << ")" << endl;
		//}
	}
	
	//void msg_all_to_all()
	//{
	//	char msg[255];
	//	for(int i=0;i<255;i++) msg[i]='\0';
	//	stringstream s;
	//	
	//	for(int sender=0;sender<rank_to_ip.size();sender++)
	//	{
	//		for(int receiver=0;receiver<rank_to_ip.size();receiver++)
	//		{
	//			if(receiver!=_host_rank)
	//			{
	//				cout << "sending to " << receiver << endl;
	//				
	//				s << "Hello from " << _host_rank << "\0";
	//				
	//				send(rank_to_socket[receiver],s.str().c_str(),s.str().size(),0);
	//				
	//				cout << "sent" << endl;
	//				
	//				s.str("");
	//			}
	//		}
	//		
	//		if(true)
	//		{
	//		}
	//		else
	//		{
	//			cout << "receiving from " << sender << endl;
	//			
	//			recv(rank_to_socket[sender],msg,255,0);
	//			
	//			cout << msg << endl;			
	//		}
	//	}
	//}
	
	///============================================================================
	/// Merge_Messages - merge messages from all threads into one per partition
	///============================================================================

	void Merge_Messages()
	{
		// first, merge all of the messages by partition, use one thread for now in order to not confuse the FSB
		
		for(int i=0;i<_num_partitions;i++)
		{
			Exchange_Data* partition_exchange_data=&exchange_information.partition_exchange_data[i];
			
			// consolidate message buffers should the exchange be happening this _iteration
			
			if(partition_exchange_data->current_exchange==_iteration)
			{
				//the head of the partition message buffer is reserved to contain the msg header
				
				char* hbuf=partition_exchange_data->send_buffer.allocate(sizeof(Head_Message_Base));
				
				((Head_Message_Base*)hbuf)->num_messages=0;
				
				for(int j=0;j<_num_threads;j++)
				{
					Exchange_Data* thread_exchange_data=&exchange_information.thread_local_exchange_data[j][i];
					
					//cout << "\t\t\tThread stats " << j << ": " << thread_exchange_data->num_messages << endl;
					
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
					
					//cout << thread_exchange_data->send_buffer.size()  << "," << thread_exchange_data->send_buffer.max() << endl;
					thread_exchange_data->send_buffer.clear();
					thread_exchange_data->num_messages=0;
				}
				
				//the head of the partition message buffer gets length and suggested next exchange information
				
				((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->current_iteration=_iteration;
				((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->length=partition_exchange_data->send_buffer.size();
				((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->suggested_next_exchange=partition_exchange_data->next_exchange;
				((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->suggested_exchange_interval=partition_exchange_data->exchange_interval;
				
				//cout << "\t\tTo Partition " << i << ": " << ((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->length << "," << ((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->suggested_next_exchange << "," << ((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->suggested_exchange_interval << endl;
			}
		}
	}
	
	///============================================================================
	/// Send_Receive - actually exchange messages
	///============================================================================

	void Send_Receive()
	{
		for(int i=0;i<_num_partitions;i++)
		{
			Exchange_Data* partition_exchange_data=&exchange_information.partition_exchange_data[i];
			
			// one thread per exchange
			
			int process = AtomicIncrement(&partition_exchange_data->comm_lock);
			
			//cout << "\t\tstatus of exchange with " << i << ": " << (bool)(partition_exchange_data->current_exchange==_iteration) << endl;
			
			if(process == 1)
			{
				if(partition_exchange_data->current_exchange==_iteration)
				{
					//cout << "\t\tmid-check " << i << ":" << _host_rank << endl;
					
					// only process exchanges which are happening
					
					//cout << "\t\t\t" << "send stats to " << i << ": " << ((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->length << "," << ((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->num_messages << "," << ((Head_Message_Base*)partition_exchange_data->send_buffer.buffer())->suggested_next_exchange << endl;
					
					// initiate sends in rank order for those ranks which are smaller than you and receives for those ranks larger than you, self send is just a memcpy
					
					if(i < _host_rank)
					{
						//cout << "\t\t\t" << _host_rank << " sending first to " << i << ": " << partition_exchange_data->send_buffer.size() << endl;
						
						int err=send(rank_to_socket[i],partition_exchange_data->send_buffer.buffer(),partition_exchange_data->send_buffer.size(),0);
						
						if(err==-1)
						{
							cout << "Send Error: " << strerror(errno) << endl;
							exit(0);
						}
						
						int recvd=recv(rank_to_socket[i],partition_exchange_data->recv_buffer.buffer(),_Max_Message_Size,0);
						
						if(recvd==-1)
						{
							cout << "Receive Error: " << strerror(errno) << endl;
							exit(0);
						}
						
						Head_Message_Base* head_msg=(Head_Message_Base*)partition_exchange_data->recv_buffer.buffer();
						
						int len_so_far=recvd;
						int msg_len=head_msg->length;
						
						if(head_msg->current_iteration!=_iteration)
						{
							cout << "Receive Failure: " << len_so_far << "," << msg_len << "," << head_msg->current_iteration << "," << _iteration << "," << i << "," << head_msg->suggested_next_exchange << endl;
							exit(0);
						}
						
						//cout << "\t\t\t" << "received initial message: " << head_msg->length << ", len_so_far " << recvd << endl;
						
						while(len_so_far<msg_len)
						{
							recvd=recv(rank_to_socket[i],partition_exchange_data->recv_buffer.buffer()+len_so_far,_Max_Message_Size,0);
							
							if(recvd!=-1)
							{
								len_so_far+=recvd;
							}
							else
							{
								cout << "Receive Error: " << strerror(errno) << endl;
							}
						}
					}
					else if(i == _host_rank)
					{
						memcpy(partition_exchange_data->recv_buffer.buffer(),partition_exchange_data->send_buffer.buffer(),partition_exchange_data->send_buffer.size());
					}
					else
					{
						//cout << "\t\t\t" << _host_rank << " receiving first from " << i << ": " << partition_exchange_data->send_buffer.size() << endl;
						
						int recvd=recv(rank_to_socket[i],partition_exchange_data->recv_buffer.buffer(),_Max_Message_Size,0);
						
						if(recvd==-1)
						{
							cout << "Receive Error: " << strerror(errno) << endl;
							exit(0);
						}
						
						Head_Message_Base* head_msg=(Head_Message_Base*)partition_exchange_data->recv_buffer.buffer();
						
						int len_so_far=recvd;
						int msg_len=head_msg->length;
						
						if(head_msg->current_iteration!=_iteration)
						{
							cout << "Receive Failure: " << len_so_far << "," << msg_len << "," << head_msg->current_iteration << "," << _iteration << "," << i << "," << head_msg->suggested_next_exchange << endl;
							exit(0);
						}
						
						//cout << "\t\t\t" << "received initial message: " << head_msg->length << ", len_so_far " << recvd << endl;

						while(len_so_far<msg_len)
						{
							recvd=recv(rank_to_socket[i],partition_exchange_data->recv_buffer.buffer()+len_so_far,_Max_Message_Size,0);
							
							if(recvd!=-1)
							{
								len_so_far+=recvd;
							}
							else
							{
								cout << "Receive Error: " << strerror(errno) << endl;
								exit(0);
							}							
						}
						
						int err=send(rank_to_socket[i],partition_exchange_data->send_buffer.buffer(),partition_exchange_data->send_buffer.size(),0);
												
						if(err==-1)
						{
							cout << "Send Error: " << strerror(errno) << endl;
							exit(0);
						}
						
					}
					
					// done with partition send buffer
					
					partition_exchange_data->send_buffer.clear();
					
					// process the header to update the exchange info with this partition, the two will agree on the minimum of their respective suggestions
					
					Head_Message_Base* head_msg=(Head_Message_Base*)partition_exchange_data->recv_buffer.buffer();
					
					//cout << "\t\t\t" << "received message from " << i << ": " << head_msg->length << "," << head_msg->num_messages << "," << head_msg->suggested_next_exchange << endl;
					
					if(head_msg->suggested_exchange_interval < partition_exchange_data->exchange_interval)
					{
						partition_exchange_data->exchange_interval=head_msg->suggested_exchange_interval;
					}
					
					// decide when the interval would imply the next exchange, assume constancy from 0
					
					int interval_exchange = _iteration + (partition_exchange_data->exchange_interval - _iteration%partition_exchange_data->exchange_interval);
					
					// run the update check for this exchange interval

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
					
					
					int interval_next_exchange = partition_exchange_data->exchange_interval + _iteration + (partition_exchange_data->exchange_interval - _iteration%partition_exchange_data->exchange_interval);
					
					// run the update check for this next exchange interval
					
					if(interval_next_exchange < partition_exchange_data->next_exchange)
					{
						partition_exchange_data->next_next_exchange = partition_exchange_data->next_exchange;
						partition_exchange_data->next_exchange = interval_next_exchange;
					}
					else if(interval_next_exchange == partition_exchange_data->next_exchange)
					{
						// do nothing
					}
					else if(interval_next_exchange < partition_exchange_data->next_next_exchange)
					{
						partition_exchange_data->next_next_exchange = interval_next_exchange;
					}
					
					
					// run the update check for the suggested exchange
					
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
					
					for(int j=0;j<_num_threads;j++)
					{
						// threads become as critical as the partition_exchange
						
						exchange_information.thread_local_exchange_data[j][i].current_exchange = partition_exchange_data->current_exchange;
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

						process_data->process_data_lock=0; // unlock the process buffer
					}
					
					// partition recv buffer no longer needed
					
					partition_exchange_data->recv_buffer.clear();
				}
			}
			
			if(process == _num_threads)
			{
				partition_exchange_data->comm_lock=0;
			}
		}
	}
	
	///============================================================================
	/// Update_Global_Exchange_Schedule - collate scheduling data from all parts.
	///============================================================================

	void Update_Global_Exchange_Schedule()
	{
		// quickly sync the partition exchange _iteration before processing
		
		Exchange_Data* partition_exchange_data;
		
		for(int i=0;i<_num_partitions;i++)
		{
			partition_exchange_data=&exchange_information.partition_exchange_data[i];
			
			//cout << partition_exchange_data->current_exchange << "," << exchange_information.next_exchange << endl;
			
			if(partition_exchange_data->current_exchange < exchange_information.next_exchange)
			{
				exchange_information.next_exchange=partition_exchange_data->current_exchange;
			}
		}

		//cout << "\t\t\t" << "status of partition exchange: " << exchange_information.current_exchange << "," << exchange_information.next_exchange  << endl;
	}
	
	///============================================================================
	/// Build_Parcels - create parcels appropriate to num messages / threads
	///============================================================================

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
		
		int msg_ratio=process_data->num_messages_total/_num_threads;
		
		//cout << "\t\t" << "msg ratio: " << msg_ratio << endl;
		
		if(msg_ratio==0)
		{
			// small case, more threads than messages; need num_messages parcels with 1 message each
			
			process_data->num_parcels=1;
		}
		else if(msg_ratio > 5)
		{
			// large case, at least 5x more messages than threads

			// target is to have 5 parcels per thread, first see if the number of messages will fit evenly into this number of parcels
			
			process_data->num_parcels=5*_num_threads;
		}
		else
		{
			// mid case, base parcels on the ratio
			
			// target is to have msg_ratio parcels per thread, first see if the number of messages will fit evenly into this number of parcels
			
			process_data->num_parcels = msg_ratio*_num_threads;			
		}
		
		int num_full_parcels;
		if( (process_data->num_messages_total%process_data->num_parcels) == 0 )
		{
			// no extra messages per parcel required, divides evenly
			
			process_data->num_messages_per_parcel = process_data->num_messages_total/process_data->num_parcels;
			num_full_parcels=process_data->num_parcels;
		}
		else
		{
			// will have to take a partial load in some of the parcels
			
			process_data->num_messages_per_parcel = process_data->num_messages_total/process_data->num_parcels + 1;
			num_full_parcels=process_data->num_messages_total%process_data->num_parcels;
		}
		
		
		//cout << "\t\t" << "num messages: " << process_data->num_messages_total << ", num parcels: " << process_data->num_parcels << ", num_messages_per_parcel: " << process_data->num_messages_per_parcel << ", num_full_parcels: " << num_full_parcels << endl;
		
		int message_counter=0;
		
		for(int i=0;i<process_data->num_parcels;i++)
		{
			Processing_Parcel* parcel=(Processing_Parcel*)process_data->processing_parcels.allocate(sizeof(Processing_Parcel));
			
			parcel->parcel_lock=0;
			
			if(num_full_parcels>0)
			{
				parcel->num_messages=process_data->num_messages_per_parcel;
				--num_full_parcels;
			}
			else
			{
				parcel->num_messages=process_data->num_messages_per_parcel - 1;
			}
			
			parcel->message_offset=message_counter;

			message_counter+=parcel->num_messages;
		}
	}
	
	///============================================================================
	/// Process - submit messages to specified user parse functions
	///============================================================================

	void Process()
	{
		Process_Data* process_data=&exchange_information.process_data;

		//cout << "\t\t" << "num parcels: " << process_data->num_parcels << ", num_messages_per_parcel: " << process_data->num_messages_per_parcel << endl;
		
		// review all created parcels
		
		for(int i=0;i<process_data->num_parcels;i++)
		{
			Processing_Parcel* parcel = (Processing_Parcel*)&process_data->processing_parcels.buffer()[i*sizeof(Processing_Parcel)];
			
			int process = AtomicIncrement(&parcel->parcel_lock);
			
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
				
				//int num_messages_to_process=min(process_data->num_messages_per_parcel,process_data->num_messages_total-my_message_offset);
				//int num_messages_to_process=parcel->num_messages;
				
				//cout << "\t\t\t" << "num_messages_to_process: " << num_messages_to_process << "; " <<  process_data->num_messages_total << "," << my_message_offset << endl;
				
				// next, begin deciphering and processing messages
				
				//start_timer(sub_process_timer_b[_thread_id]);
				
				for(int i=0;i<parcel->num_messages;i++)
				{
					// locate the type singleton and submit the message

					(*((Communication_Object*)_all_components[((Message_Base*)current_position)->type_index])->communication_handler_register)(_all_components[((Message_Base*)current_position)->type_index],current_position+sizeof(Message_Base));
					//cout << "\t\t\t" << "incrementing: " << ((Message_Base*)current_position)->length << "," << ((Message_Base*)current_position)->type_index << endl;
					current_position+=((Message_Base*)current_position)->length;
				}
				
				//end_timer(sub_process_timer_b[_thread_id],sub_process_time_b[_thread_id]);
			}
			
			if(process == _num_threads)
			{
				process_data->process_data_lock=0;
			}
		}
	}
	
	///============================================================================
	/// Exchange - overall interprocess exchange function
	///============================================================================

	void Exchange()
	{
		bool exchange_this_iteration = (exchange_information.current_exchange == _iteration);
		
		//cout << "entering exchange: " << _iteration << " : " << exchange_information.current_exchange << endl;
		
		while(exchange_this_iteration)
		{
			//long process=AtomicIncrement(&ipc_threads_counter_A);
			
			while(AtomicExchange(&ipc_lock,1)) SLEEP(0); // lock the ipc engine
				
				// only the first thread gets in
				
				if(++ipc_threads_counter_A == 1)
				{
					//cout << "\tmerging messages" << endl;
					Merge_Messages();
				}
				
				if(ipc_threads_counter_A==_num_threads)
				{
					ipc_threads_counter_A=0;
					//cout << "\tmessages merged" << endl;
				}
			
			ipc_lock=0; // unlock the ipc engine
			
			while(AtomicCompareExchange(&ipc_threads_counter_A,0,0)) SLEEP(0); // everyone spins until the counter is reset
			
			//cout << "\tsending and receiving" << endl;
			
			Send_Receive();
			
			
			
			
			if(AtomicIncrement(&ipc_threads_counter_B)==_num_threads) // have everyone go through the next turnstile
			{
				//cout << "\tdone sending and receiving" << endl;
			
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
				
				if(ipc_threads_counter_A==_num_threads)
				{
					ipc_threads_counter_A=0;
				}
			
			ipc_lock=0; // unlock the ipc engine
			
			while(AtomicCompareExchange(&ipc_threads_counter_A,0,0)) SLEEP(0); // everyone spins until the counter is reset
			
			// finally handle all of the messages
			
			//cout << "\tprocessing" << endl;
			
			Process();
			
			//while(AtomicExchange(&stdout_lock,1)) SLEEP(0);
			//cout << _thread_id << " done processing" << endl;
			//stdout_lock=0;
		
			//cout << "\tdone processing" << endl;
			
			if(AtomicIncrement(&ipc_threads_counter_B)==_num_threads) // have everyone go through the next turnstile
			{
				//cout << "\t\t\t" << "status of partition exchange B: " << exchange_information.current_exchange << "," << exchange_information.next_exchange  << endl;
			
				// clean up processing
				
				exchange_information.process_data.Reset();
				
				exchange_information.current_exchange=exchange_information.next_exchange;
				exchange_information.next_exchange=INT_MAX;
				
				ipc_threads_counter_B=0;
				
				//cout << "\t" << "done, next exchange: " << exchange_information.current_exchange << endl;
			}
			
			while(AtomicCompareExchange(&ipc_threads_counter_B,0,0)) SLEEP(0); // everyone spins until the counter is reset
			
			exchange_this_iteration = (exchange_information.current_exchange == _iteration);
			
			break;
		}
	}
	
	string job_name;
	
	string hostname;
	string host_ip;
	bool is_head_node;

	volatile unsigned int ipc_lock;

	volatile unsigned int ipc_threads_counter_A;
	volatile unsigned int ipc_threads_counter_B;

	vector<string> all_names;
	vector<string> all_ips;
	
	unordered_map<int,string> rank_to_ip;
	unordered_map<int,int> rank_to_socket;
	

};
#endif

static Interprocess_Engine* interprocess_root_ptr;

