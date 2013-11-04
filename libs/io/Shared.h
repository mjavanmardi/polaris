#pragma once

#include <vector>
#include <string>
#include <map>
#include <odb/core.hxx>
#include <memory>

using std::shared_ptr;

#pragma db value(std::string) default("")
#pragma db value(int) default(0)
#pragma db value(double) default(0.0)
#pragma db value(float) default(0.0)