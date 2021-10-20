#ifndef PRINTERS_LUA_INP_2_LUA
#define PRINTERS_LUA_INP_2_LUA
#include <string>
#include <fstream>
#include <vector>
#include "objects.h"

class printersLua
{
public:
 // funtion to printer the lua file
 static void printHeaderLua(std::ofstream& out);
};

#endif