#pragma once
#include "Core_Includes.h"

class Single_Publisher
{
public:
	Single_Publisher():topic_handle(NULL),topic_base(NULL),file_pointer(NULL){};

	bool Publish_Message(string& Message)
	{
		(*((int*)topic_base))=(int)(file_pointer-topic_base);

		(*((int*)file_pointer))=(int)Message.size();
		file_pointer+=sizeof(int);

		memcpy(file_pointer,Message.c_str(),Message.size());
		file_pointer+=sizeof(Message);
		
		cout << "Published: \"" << Message << "\"" << endl;

		return true;
	}

	bool Create_Topic(string& Topic_Name)
	{
		if(!topic_handle)
		{
			LARGE_INTEGER max_file_size;
			max_file_size.QuadPart=1<<30;
			HANDLE handle=CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,max_file_size.HighPart,max_file_size.LowPart,Topic_Name.c_str());
			
			if(!handle) return false;
			if(GetLastError()==ERROR_ALREADY_EXISTS) return false;
			
			topic_handle=handle;

			handle=MapViewOfFile(topic_handle,FILE_MAP_ALL_ACCESS,0,0,0);
			
			if(!handle) return false;

			topic_base=(BYTE*)handle;
			
			(*((int*)topic_base))=0;

			file_pointer=topic_base+sizeof(int);

			return true;
		}
		else
		{
			return false;
		}
	}

private:
	HANDLE topic_handle;
	
	BYTE* topic_base;
	BYTE* file_pointer;
};

class Subscriber
{
public:
	Subscriber():topic_handle(NULL){};

	bool Subscribe(string& Topic_Name)
	{
		if(!topic_handle)
		{
			LARGE_INTEGER file_size;
			file_size.QuadPart=1<<30;
			HANDLE handle=OpenFileMapping(FILE_MAP_READ,TRUE,Topic_Name.c_str());

			if(!handle) return false;
			if(GetLastError()==ERROR_ALREADY_EXISTS) return false;

			topic_handle=handle;

			handle=MapViewOfFile(topic_handle,FILE_MAP_READ,0,0,0);

			if(!handle) return false;

			topic_base=(BYTE*)handle;

			file_pointer=topic_base;

			return true;
		}
		else
		{
			return false;
		}
	}

	bool Read_Current_Message(int id)
	{
		while(true)
		{
			if(file_pointer!=(topic_base+(*((int*)topic_base))))
			{
				file_pointer=topic_base+(*((int*)topic_base));
				
				int msg_size=(*((int*)file_pointer));

				BYTE* f_ptr=file_pointer+sizeof(int);

				cout << id << " Read: \"";

				for(;f_ptr<file_pointer+msg_size+sizeof(int);f_ptr++)
				{
					cout << (*f_ptr);
				}

				cout << "\""<< endl;

				return true;
			}

			Sleep(10);
		}

		return true;
	}

private:
	HANDLE topic_handle;
	BYTE* topic_base;
	BYTE* file_pointer;
};