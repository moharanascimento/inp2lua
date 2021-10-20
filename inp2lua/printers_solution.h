#ifndef PRINTERS_SOLUTION_INP_2_LUA
#define PRINTERS_SOLUTION_INP_2_LUA
#include <string>
#include <fstream>
#include <vector>
#include "objects.h"

class printersSolution
{
public:
 // funtion to printer the solution file
 static void printHeaderSolution(std::ofstream& out);
 static void printPhysical(std::ofstream& out, std::vector<step> steps, 
                           std::vector<element> elements, std::vector<sectionAtributtes> sections, 
                           std::vector<material> materials, std::vector<setAtributtes> sets, 
                           std::vector<boundaryConditions> bc, std::vector<load> loads);
 static void printDoFile(std::ofstream& out);
 static void printSolver(std::ofstream& out, std::vector<step> steps);
};

#endif