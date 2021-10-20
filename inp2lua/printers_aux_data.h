#ifndef PRINTERS_AUx_INP_2_LUA
#define PRINTERS_AUX_INP_2_LUA
#include <string>
#include <fstream>
#include <vector>
#include "objects.h"

class printersAuxData
{
public:
 // funtion to printer the lua file
 static void printAuxData(std::ofstream& out);
};

#endif