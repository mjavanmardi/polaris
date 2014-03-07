//*********************************************************
//	Record.h - Functions for Recording Video
//*********************************************************

#pragma once
#include "Antares_Layer_Implementation.h"

#define STREAMING

//---------------------------------------------------------
//	Initialize_Recorder
//--------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::Initialize_Recorder()
{
	_recording=false;
	_recording_index=0;
	_frame_counter=0;

	// Create a pipe for the child process's STDOUT. 
	//if ( ! CreatePipe(&out_read, &out_write, &security_attributes, 0) )  cout << "Stdout failed!" << endl;


	//WaitForSingleObject( process_info.hProcess, INFINITE );
}

//---------------------------------------------------------
//	Start_Recording
//--------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::Start_Recording()
{
	_frame_counter = 0;

	++_recording_index;

#ifdef STREAMING
		SECURITY_ATTRIBUTES security_attributes; 

		// Set the bInheritHandle flag so pipe handles are inherited. 
	
		security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES); 
		security_attributes.bInheritHandle = TRUE; 
		security_attributes.lpSecurityDescriptor = NULL; 

		// Create a pipe for the child process's STDIN. 
		if (! CreatePipe(&in_read, &in_write, &security_attributes, 0))  cout << "Stdin failed!" << endl; 

		ZeroMemory( &start_info, sizeof(start_info) );
		start_info.cb = sizeof(STARTUPINFOA);
		start_info.hStdError = GetStdHandle(STD_ERROR_HANDLE);
		start_info.hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);
		start_info.hStdInput = in_read;
		start_info.dwFlags |= STARTF_USESTDHANDLES;

		ZeroMemory( &process_info, sizeof(process_info) );

		stringstream s;
		s << "ffmpeg.exe -y -pix_fmt rgb24 -f rawvideo -framerate 30 -s ";
		s << _panel_width << "x" << _panel_height;
		//s << " -r 30";
		s << " -i - -an -r 60 -vcodec mpeg4 -vb 75M clip_" << _recording_index << ".wmv " << '\0' << '\0';
		//s << " -i - -an -r 30 my_movie_" << _recording_index << ".mpeg " << '\0' << '\0';
		//s << " -i - -an -r 30 -vb 10240000 -bufsize 10240000 -minrate 10000000 -maxrate 11000000 my_movie_" << _recording_index << ".mpeg " << '\0' << '\0';

		//char ffmpeg_arguments[] = 930x472.mpeg";

		char* ffmpeg_arguments = new char[s.str().size()];

		memcpy(ffmpeg_arguments,s.str().c_str(),s.str().size());

		BOOL ffmpeg_process = CreateProcessA(NULL,ffmpeg_arguments,NULL,NULL,TRUE,0,NULL,NULL,&start_info,&process_info);

		delete[] ffmpeg_arguments;
#endif

	_recording = true;
}

//---------------------------------------------------------
//	Stop_Recording
//--------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::Stop_Recording()
{
	_recording = false;

	CloseHandle(in_read);
	CloseHandle(in_write);

	TerminateProcess(process_info.hProcess,0);

	CloseHandle(process_info.hProcess);
	CloseHandle(process_info.hThread);
}

//---------------------------------------------------------
//	Record_Frame
//--------------------------------------------------------

template<typename MasterType,typename InheritanceList>
void Canvas_Implementation<MasterType,InheritanceList>::Record_Frame()
{
	//cout << "Recording: " << _iteration << "," << _sub_iteration << endl;

	glReadBuffer(GL_BACK);
	int width = _panel_width;
	int height = _panel_height;

	//unsigned char* pixels = new unsigned char[width*height*3];
	unsigned char* pixels = (unsigned char*) malloc(width*height*3);

	glReadPixels(0,0,width,height,GL_RGB,GL_UNSIGNED_BYTE,pixels);

	for(int i=0;i<height/2;i++)
	{
		for(int j=0;j<width*3;j++)
		{
			unsigned char top_pixel;
			unsigned char bottom_pixel;

			top_pixel = pixels[width*i*3+j];
			bottom_pixel = pixels[(height-i-1)*width*3+j];

			pixels[width*i*3+j] = bottom_pixel;
			pixels[(height-i-1)*width*3+j] = top_pixel;
		}
	}

#ifdef STREAMING
		//cout << gluErrorString(glGetError()) << endl;

		DWORD dwWrite, dwWritten;
		BOOL bSuccess = FALSE;

		dwWrite = width*height*3;
		bSuccess = WriteFile(in_write,pixels,dwWrite,&dwWritten,NULL);

		free(pixels);
#else
		wxImage output_frame(width,height,pixels);

		stringstream s;

		s << "Frame_" << _frame_counter << ".png";

		output_frame.SaveFile(s.str(),wxBITMAP_TYPE_PNG);
		 
		++_frame_counter;
#endif
	
}
