#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "objects.h"
#include "printers.h"
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
    nodes = readers::listOfNodes(file, line);
   }
   if (readers::stringContain(line, "*Element"))
   {
    elements = readers::listOfElements(file, line);
   }
   if (readers::stringContain(line, "*Nset"))
   {
    sets = readers::listOfSets(file, line);
   }
   if (readers::stringContain(line, "** Section"))
   {
    sections = readers::listOfSections(file, line);
   }
   if (readers::stringContain(line, "*Assembly"))
   {
    setLBC = readers::listOfSetsOfLoadAndBC(file, line);
   }
   if (readers::stringContain(line, "*Elset, elset=_"))
   {
    surface = readers::listOfSurfaces(file, line);
   }
   if (readers::stringContain(line, "*Material"))
   {
    materials = readers::listOfMaterials(file, line);
   }
   if (readers::stringContain(line, "** STEP"))
   {
    std::vector<step> auxSteps = readers::listOfSteps(file, line);
    steps.insert(steps.end(), auxSteps.begin(), auxSteps.end());
   }
   if (readers::stringContain(line, "** BOUNDARY"))
   {
    auto auxBond = readers::listOfBoundaryConditions(file, line);
    boundaryConditions.insert(boundaryConditions.end(), auxBond.begin(), auxBond.end());
   }
   if (readers::stringContain(line, "** LOADS"))
   {
    auto auxLoads = readers::listOfLoads(file, line);
    loads.insert(loads.end(), auxLoads.begin(), auxLoads.end());
   }
  } 
  file.close();
  std::ofstream fileOut("Modelo_model.lua");
  if (fileOut.is_open())
  {
   printers::printHeader(fileOut);
   printers::printMatProperties(fileOut, materials, sets, sections);
   printers::printPropertySet(fileOut, sections);
   printers::printMeshDefinition(fileOut, nodes.size(), elements.size());
   printers::printNodes(fileOut, nodes);
   printers::printElementGroups(fileOut, elements, sections, sets);
   printers::printMeshElements(fileOut, elements, sections, materials, sets);
   printers::printBCEdges(fileOut, loads, surface);
   printers::printMesh(fileOut, elements, surface, loads);
   printers::printBoundaryConditions(fileOut, loads,setLBC, boundaryConditions);
   printers::printSignature(fileOut);
   fileOut.close();
  }
 }
}

int main()
{
 readFile("Job-3.inp");
}