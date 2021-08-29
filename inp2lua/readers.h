#ifndef READERS_INP_2_LUA
#define READERS_INP_2_LUA
#include <string>
#include <fstream>
#include <vector>
#include "objects.h"

class readers
{
public:

 static std::vector<node> listOfNodes(std::ifstream& file, std::string& line);
 static bool stringContain(std::string s1, std::string s2);
 static std::string stringBetween(std::string line, std::string first, std::string last);
 static std::vector<element> listOfElements(std::ifstream& file, std::string& line);
 static std::vector<setAtributtes> listOfSets(std::ifstream& file, std::string& line);
 static std::vector<sectionAtributtes> listOfSections(std::ifstream& file, std::string& line);
 static std::vector<setOfLoadAndBC> listOfSetsOfLoadAndBC(std::ifstream& file, std::string& line);
 static std::vector<surfaceOfLoadAndBC> listOfSurfaces(std::ifstream& file, std::string& line);
 static std::vector<material> listOfMaterials(std::ifstream& file, std::string& line);
 static std::vector<boundaryConditions> listOfBoundaryConditions(std::ifstream& file, std::string& line);
 static std::vector<load> listOfLoads(std::ifstream& file, std::string& line);
};



#endif