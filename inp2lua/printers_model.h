#ifndef PRINTERS_MODEL_INP_2_LUA
#define PRINTERS_MODEL_INP_2_LUA
#include <string>
#include <fstream>
#include <vector>
#include "objects.h"

class printersModel
{
public:
 // funtion to printer the model file
 static void printHeader(std::ofstream& out);
 static void printMatProperties(std::ofstream& out, std::vector<material>, std::vector<setAtributtes>, std::vector<sectionAtributtes>);
 static void printNodes(std::ofstream& out, std::vector<node>);
 static void printMeshDefinition(std::ofstream& out, int nodesSize, int elementsSize);
 static void printPropertySet(std::ofstream& out, std::vector<sectionAtributtes>);
 static void printElementGroups(std::ofstream& out, std::vector<element> elements, std::vector<sectionAtributtes> sections, std::vector<setAtributtes> sets);
 static void printMeshElements(std::ofstream& out, std::vector<element> elements, std::vector<sectionAtributtes> sections, std::vector<material> materials, std::vector<setAtributtes> sets);
 static void printBCEdges(std::ofstream& out, std::vector<load> loads, std::vector<surfaceOfLoadAndBC> surfaces);
 static void printMesh(std::ofstream& out, std::vector<element> elements, std::vector<surfaceOfLoadAndBC> surfaces, std::vector<load> loads);
 static void printBoundaryConditions(std::ofstream& out, std::vector<load> loads, std::vector<setOfLoadAndBC> sets, std::vector<boundaryConditions> bc, std::vector<step> steps);
 static void printSignature(std::ofstream& out);
};



#endif