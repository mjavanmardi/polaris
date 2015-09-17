#pragma once



#include "road.h"
#include "io/database.h"
#include "io/Io_db.h"

using namespace odb;
using namespace polaris::io;


double getCapacity(string type, double grade);

bool verifyLane(int lane, string lanes);

void importLanes(Road& R, bool q, int lane, result<polaris::io::Connect> connections, double grade);

std::pair<Road,Road> importRoad(polaris::io::Link db_itr, int ID, vector<int> link, vector<int> toNodeA, vector<int> toNodeB, vector<bool> dir, vector<string> lanes, vector<string> type);

void preprocessConnections(result<polaris::io::Connect> connections, vector<int>& link, vector<int>& toNodeA, vector<int>& toNodeB, vector<bool>& dir, vector<string>& lanes, vector<string>& type);

std::map<int, Road> openRoad(char *db_path);

Json::Value roadToJson(char *db_path);

std::map<int,Road> jsonToRoad(std::string fileName);