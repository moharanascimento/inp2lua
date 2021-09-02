#ifndef PRINTERS_INP_2_LUA
#define PRINTERS_INP_2_LUA
#include <string>
#include <fstream>
#include <vector>
#include "objects.h"

class printers
{
public:
 // funtion to 
 static void printHeader(std::ofstream& out);
 static void printMatProperties(std::ofstream& out, std::vector<material>);
 static void printNodes(std::ofstream& out, std::vector<node>);
 static void printMeshDefinition(std::ofstream& out, int nodesSize, int elementsSize);
 static void printPropertySet(std::ofstream& out, std::vector<sectionAtributtes>);
 static void printElementGroups(std::ofstream& out, std::vector<element> elements, std::vector<sectionAtributtes> sections, std::vector<setAtributtes> sets);
};



#endif