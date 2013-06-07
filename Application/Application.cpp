#include "Polaris_PCH.h"


int main()
{

	// write test
	File_IO::Binary_File_Writer bw;
	bw.Open("test.bin");

	int num_ints_to_write = 5;
	bw.Write_Value<int>(num_ints_to_write);
	
	int ints[5] = {1,2,3,4,12};
	bw.WriteArray<int>(ints,num_ints_to_write);

	bw.Close();


	// read_test
	File_IO::Binary_File_Reader br;
	br.Open("test.bin");

	int num_to_read;
	br.Read_Value<int>(num_to_read);

	int *ints_in = new int[5];
	br.Read_Array<int>(ints_in, num_to_read);

	cout << num_to_read<<endl;
	
	for (int i = 0; i < num_to_read; ++i)
	{
		cout << ints_in[i]<<endl;
	}

	int test;
	cin >> test;
}