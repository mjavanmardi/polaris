#include "tmc.h"
#include "read_data.h"
#include "DataStructures.h"

int main(int argc, char* argv[])
{
	//if (argc != 2) 
	//{
	//	std::cout << "Usage: " << argv[0] << "[gtest arguments] partial_path_to_database\n";
	//	return 1;
	//}
	//polaris::Matrix mat = GetLinkTravelProb(argv[1]);
	polaris::Matrix mat;
	read_from_file("C:\\Users\\vsokolov\\POLARIS_Demo\\Case_Data\\Normal_Day\\vehicle_trajecotry.csv", mat);
	cout << "size: " << mat(2501).size() << "\n";
	std::ofstream fh_out ("C:\\Users\\vsokolov\\POLARIS_Demo\\link_probability.csv");
	if (fh_out.is_open())
	{

		for (auto it = mat.M().begin(); it!=mat.M().end(); ++it)
		{
			for (auto it1 = it->second.begin(); it1 != it->second.end(); ++it1)
			{
				fh_out << it->first << ',' << it1->first << ',' << it1->second << "\n";
			}
		}
	}
	fh_out.close();
	return 0;
}