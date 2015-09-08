#pragma once

std::vector<int> jsonToVectInt(Json::Value root, std::string name);
std::map<int,double> jsonToMapIntDouble(Json::Value root, std::string name);
std::vector<double> jsonToVectorDouble(Json::Value root, std::string name);
std::vector<int> jsonToVectorInt(Json::Value root, std::string name);

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