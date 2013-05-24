#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <errno.h>
#include "Io/Utils.h"
#include <map>
#include <stdlib.h> 
#include "DataStructures.h"
#include <exception>
#include <omp.h>
#include <time.h>
#include <sstream> 

inline void split(const std::string &s, char delim, std::vector<std::string> &result)
{
	result.clear();
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        result.push_back(item);
    }
}


int read_from_file(std::string file_path, polaris::Matrix& result)
{
	using namespace std;
	string line;
	ifstream  fh (file_path);
	int nlinks;
	std::vector<std::string> tok_line;
	int  link_path [5000];
	if (fh.is_open() && fh != NULL)
	{
		//skip the headers
		getline(fh,line);
		getline(fh,line);
	}
	else
		return errno;
	int count = 0;
	while (fh.good())
	{
		count +=1;
		if (count % 5000 == 0)
			cout << count << "\n";
		//if (count > 10000)
		//{
		//	fh.close();
		//	return 0;
		//}
		getline(fh, line);
		split(line, ',', tok_line);
		nlinks = atoi(tok_line[7].c_str());
		for (int i=0; i<nlinks; i++)
		{
			getline(fh, line);
			split(line, ',', tok_line);
			link_path[i] = atoi(tok_line[1].c_str());
		}
		for (int i=0; i<nlinks; i++)
		{
			for (int j=i; j<nlinks; j++)
			{
				result(link_path[i],link_path[j]) += 1;
			}
		}
	}
	fh.close();
	return 0;
}


polaris::Matrix GetLinkTravelProb(const std::string& db_path_prefix)
{
	using namespace std;
	int nlinks = polaris::CountRows(db_path_prefix, "Link");
	cout << "nlinks: " << nlinks << "\n";
	polaris::Matrix mat(nlinks,nlinks);
	map<int, int> link2ind;
	try 
	{
		unique_ptr<odb::database> db = open_sqlite_database(db_path_prefix);
		typedef odb::query<polaris::io::Link> query;
		typedef odb::result<polaris::io::Link> result;
		odb::transaction t(db->begin());
		result r(db->query<polaris::io::Link>(query::true_expr));	
		int index = 0;
		int rn;
		srand (10);
		clock_t now = clock();
		for (result::iterator i (r.begin()); i!=r.end(); ++i)
		{
			link2ind[i->getLink()] = index;
			index++;
			for (int i = 0; i < nlinks; i++)
			{
				rn = rand() % 10 + 1;
				//mat(index, i) = rn;
				if (rn > 9)
				{
					mat(index, i) = rn;
				}
			}
		}
		clock_t later = clock();
		printf("%.3f for loop\n",(double)(later-now)/(double)CLOCKS_PER_SEC);
		t.commit();
	}
	catch (exception& e)
	{
		std::cout << e.what() << "\n";
	}
	cout << "size = " << mat(1).size() << "\n";
	cout << "mat(2,3) = " << mat(2,3) << "\n";
	return mat;
}