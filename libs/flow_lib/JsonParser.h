#pragma once

#include <fstream>

//Tools to read from Json Values

std::vector<int> jsonToVectInt(Json::Value root, std::string name);
std::map<int,double> jsonToMapIntDouble(Json::Value root, std::string name);
std::vector<double> jsonToVectorDouble(Json::Value root, std::string name);
std::vector<int> jsonToVectorInt(Json::Value root, std::string name);
bool jsonToBool(Json::Value root, std::string name);
int jsonToInt(Json::Value root, std::string name);
double jsonToDouble(Json::Value root, std::string name);

template<typename T>
void jsonToVector(Json::Value root, std::string name, std::vector<typename T> &value) //Converter for abstract type with constructor
{
	Json::Value JVect = root;
	if(JVect.isArray())
	{
		for(int i=0;i<JVect.size();i++)
		{
			T a(JVect[i]);
			value.push_back(a);
		}
	}
	else
		throw string(name+" information missing");
}

template<typename T>
void jsonToMapInt(Json::Value root, std::string name, std::map<int,T> &value)
{
	if(!root.isNull())
	{
		Json::Value jRoot = root;
		for(Json::Value::iterator it = root.begin();it != root.end();it++)
		{
			if(it.key().isString())
				value[stoi(it.key().asString())] = T(*it);
			else
			{
				if(it.key().isString())
					throw string("Problem with "+name+" values  information");
				else
					throw string("Problem with "+name+" keys information");
			}
		}
	}
	else
		throw string(name+" information missing");
}

std::string fileToString(const std::string& filename);