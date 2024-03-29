#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "objects.h"
#include "printers_model.h"
#include "printers_lua.h"
#include "printers_solution.h"
#include "printers_aux_data.h"
#include "readers.h"

void readFile(std::string filePath)
{
 std::string line;
 std::ifstream file(filePath);
 if (file.is_open())
 {
  std::vector <node> nodes;
  std::vector <element> elements;
  std::vector <setAtributtes> sets;
  std::vector <sectionAtributtes> sections;
  std::vector <setOfLoadAndBC> setLBC;
  std::vector <surfaceOfLoadAndBC> surface;
  std::vector <material> materials;
  std::vector <step> steps;
  std::vector <boundaryConditions> boundaryConditions;
  std::vector <load> loads;
  while (std::getline(file, line))
  {
   std::cout << line << std::endl;
   if (readers::stringContain(line, "*Node"))
   {
    std::cout << "reading nodes" << std::endl;
    nodes = readers::listOfNodes(file, line);
   }
   if (readers::stringContain(line, "*Element"))
   {
    std::cout << "reading elements" << std::endl;
    elements = readers::listOfElements(file, line);
   }
   if (readers::stringContain(line, "*Nset"))
   {
    std::cout << "reading sets" << std::endl;
    sets = readers::listOfSets(file, line);
   }
   if (readers::stringContain(line, "** Section"))
   {
    std::cout << "reading sections" << std::endl;
    sections = readers::listOfSections(file, line);
   }
   if (readers::stringContain(line, "*End Instance"))
   {
    std::cout << "reading sets of loads and bc" << std::endl;
    setLBC = readers::listOfSetsOfLoadAndBC(file, line);
   }
   if (readers::stringContain(line, "*Elset, elset=_"))
   {
    std::cout << "reading surfaces" << std::endl;
    surface = readers::listOfSurfaces(file, line);
   }
   if (readers::stringContain(line, "*Material"))
   {
    std::cout << "reading materials" << std::endl;
    materials = readers::listOfMaterials(file, line);
   }
   if (readers::stringContain(line, "** STEP"))
   {
    std::cout << "reading steps" << std::endl;
    std::vector<step> auxSteps = readers::listOfSteps(file, line);
    steps.insert(steps.end(), auxSteps.begin(), auxSteps.end());
   }
   if (readers::stringContain(line, "** BOUNDARY"))
   {
    std::cout << "reading bc" << std::endl;
    auto auxBond = readers::listOfBoundaryConditions(file, line);
    boundaryConditions.insert(boundaryConditions.end(), auxBond.begin(), auxBond.end());
   }
   if (readers::stringContain(line, "** LOADS"))
   {
    std::cout << "reading loads" << std::endl;
    auto auxLoads = readers::listOfLoads(file, line);
    loads.insert(loads.end(), auxLoads.begin(), auxLoads.end());
   }
  } 
  file.close();

  // write the model file
  std::ofstream fileOut("Modelo_model.lua");

  if (fileOut.is_open())
  {
   printersModel::printHeader(fileOut);
   printersModel::printMatProperties(fileOut, materials, sets, sections);
   printersModel::printPropertySet(fileOut, sections);
   printersModel::printMeshDefinition(fileOut, nodes.size(), elements.size());
   printersModel::printNodes(fileOut, nodes);
   printersModel::printElementGroups(fileOut, elements, sections, sets);
   printersModel::printMeshElements(fileOut, elements, sections, materials, sets);
   printersModel::printBCEdges(fileOut, loads, surface);
   printersModel::printMesh(fileOut, elements, surface, loads);
   printersModel::printBoundaryConditions(fileOut, loads,setLBC, boundaryConditions, steps);
   printersModel::printSignature(fileOut);
   fileOut.close();
  }

  // write the lua file
  std::ofstream fileOutLua("Modelo.lua");
  if (fileOutLua.is_open())
  {
   printersLua::printHeaderLua(fileOutLua);
   printersModel::printSignature(fileOutLua);
   fileOutLua.close();
  }

  // write the solution file
  std::ofstream fileOutSolution("Modelo_solution.lua");
  if (fileOutSolution.is_open())
  {
   printersSolution::printHeaderSolution(fileOutSolution);
   printersSolution::printPhysical(fileOutSolution, steps, elements, sections, materials, sets, boundaryConditions, loads);
   printersSolution::printDoFile(fileOutSolution);
   printersSolution::printSolver(fileOutSolution, steps);
   printersSolution::printProcess(fileOutSolution, loads, steps);
   printersModel::printSignature(fileOutSolution);
   fileOutSolution.close();
  }

  // write the aux data file
  std::ofstream fileOutAux("AuxData.lua");
  if (fileOutAux.is_open())
  {
   printersAuxData::printAuxData(fileOutAux);
   fileOutAux.close();
  }

 }
}



int main()
{
 readFile("Job-3.inp");
}