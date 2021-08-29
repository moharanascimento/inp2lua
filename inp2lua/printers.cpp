#include "printers.h"

void printers::printHeader(std::ofstream& out)
{
 if (out.is_open())
 {
  out << "-------------------------------------------------------------" << std::endl;
  out << "--  State variables" << std::endl;
  out << "-------------------------------------------------------------" << std::endl;
  out << "StateVar{id = 'u', dim = 2, description = 'Displacements in the X and Y directions', unit = 'm', format = '8.4f', groupName = 'mechanic'}" << std::endl << std::endl;
  out << "-------------------------------------------------------------" << std::endl;
  out << "--  Cell properties" << std::endl;
  out << "-------------------------------------------------------------" << std::endl;
 }
}

void printers::printMatProperties(std::ofstream& out, std::vector<material> material)
{
 if (out.is_open())
 {
  out << "PropertySet {" << std::endl;
  out << "   id          = 'MatProp'," << std::endl;
  out << "    typeName    = 'GemaPropertySet'," << std::endl;
  out << "    description = 'Material properties'," << std::endl;
  out << "    properties  = {" << std::endl;

  
 }
}

void printers::printNodes(std::ofstream& out, std::vector<node> nodes)
{
 if (out.is_open())
 {
  out << "local mesh_nodes = {" << std::endl;
  for (node n : nodes)
  {
   out << "\t {" << n.x << " , " << n.y << "}," << std::endl;
  }
  out << "}" << std::endl;
 }
}

void printers::printMeshDefinition(std::ofstream& out, int nodesSize, int elementsSize)
{
 if (out.is_open())
 {
  out << "-------------------------------------------------------------" << std::endl;
  out << "--  Mesh definition" << std::endl;
  out << "-------------------------------------------------------------" << std::endl;
  out << "--  Number of nodes: " << nodesSize << std::endl;
  out << "--  Number of elements: " << elementsSize << std::endl << std::endl;
 }
}

//test
void printers::printTest(std::ofstream& out, std::vector<element> elements)
{
 if (out.is_open())
 {
  out << "local mesh_elements = {" << std::endl;
  for (element e : elements)
  {
   out << "\t {" << e.id << " , " << e.incidence[0] << " , " << e.incidence[1] << " , " << e.incidence[2] << " , " << e.incidence[3] << " , " << e.elementType << " }, " << std::endl;
  }
  out << "}" << std::endl;
 }
}
