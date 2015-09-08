#include "json.h"
#include "JsonParser.h"

using namespace std;

std::vector<int> jsonToVectInt(Json::Value root, string name)
{
	vector<int> vect(0);
	if(root.isArray())
	{
		Json::Value jRoot = root;
		for(int i=0;i<jRoot.size();i++)
		{
			if(jRoot[i].isInt())
				vect.push_back(jRoot[i].asInt());
			else
				throw string("Problem with "+name+" information");
		}
	}
	else
		throw string(name+" information missing");
	return vect;
}

std::map<int,double> jsonToMapIntDouble(Json::Value root, string name)
{
	map<int,double> value;
	if(!root.isNull())
	{
		Json::Value jRoot = root;
		for(Json::Value::iterator it = root.begin();it != root.end();it++)
		{
			if(it.key().isString() && (*it).isDouble())
				value[stoi(it.key().asString())] = (*it).asDouble();
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

	return value;
}

std::vector<double> jsonToVectorDouble(Json::Value root, std::string name)
{
	vector<double> value;

	if(root.isArray())
	{
		Json::Value jRoot = root;
		for(int i=0;i<jRoot.size();i++)
		{
			if(jRoot[i].isDouble())
				value.push_back(jRoot[i].asDouble());
			else
				throw string("Problem with "+name+" information");
		}
	}
	else
		throw string(name+" information missing");

	return value;
}

std::vector<int> jsonToVectorInt(Json::Value root, std::string name)
{
	vector<int> value;

	if(root.isArray())
	{
		Json::Value jRoot = root;
		for(int i=0;i<jRoot.size();i++)
		{
			if(jRoot[i].isInt())
				value.push_back(jRoot[i].asInt());
			else
				throw string("Problem with "+name+" information");
		}
	}
	else
		throw string(name+" information missing");

	return value;
}

bool jsonToBool(Json::Value root, std::string name)
{
	bool value;
	if(root.isBool())
		value = root.asBool();
	else
		throw string(name+" information missing");
	return value;
}

int jsonToInt(Json::Value root, std::string name)
{
	int value;
	if(root.isInt())
		value = root.asInt();
	else
		throw string(name+" information missing");
	return value;
}

double jsonToDouble(Json::Value root, std::string name)
{
	double value;
	if(root.isDouble())
		value = root.asDouble();
	else
		throw string(name+" information missing");
	return value;
}