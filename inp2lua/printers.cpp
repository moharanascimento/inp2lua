#include "printers.h"
#include "utils.h"

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

void printers::printMatProperties(std::ofstream& out, std::vector<material> materials)
{
 if (out.is_open())
 {
  out << "PropertySet {" << std::endl;
  out << "    id          = 'MatProp'," << std::endl;
  out << "    typeName    = 'GemaPropertySet'," << std::endl;
  out << "    description = 'Material properties'," << std::endl;
  out << "    properties  = {" << std::endl;
  out << "        {id = 'E'  , description = 'Elastic modulus'           , unit = 'kPa'}," << std::endl;
  out << "        {id = 'nu' , description = 'Poisson ratio'             , unit = ''   }," << std::endl;

  bool hasMohrCoulomb = false;
  bool hasDruckerPrager = false;
  bool hasVonMises = false;

  for (material m : materials)
  {
   if (!hasMohrCoulomb && m.type == materialType::MOHRCOULOMB)
    hasMohrCoulomb = true;
   if (!hasDruckerPrager && m.type == materialType::DRUCKERPRAGER)
    hasDruckerPrager = true;
   if (!hasVonMises && m.type == materialType::VONMISES)
    hasVonMises = true;
  }

  if (hasMohrCoulomb || hasDruckerPrager)
  {
   out << "        {id = 'Coh', description = 'Cohesion'                  , unit = 'kPa'}," << std::endl;
   out << "        {id = 'Phi', description = 'Angle of internal friction', unit = 'degree'}," << std::endl;
   out << "        {id = 'Psi', description = 'Angle of dilation'         , unit = 'degree'}," << std::endl;
  }
  if (hasVonMises)
  {
   out << "		{id = 'Sy0' , description = 'Yield stress'			     , unit = 'kPa'}," << std::endl;
  }
  out << "        {id = 'Mec_material', description = 'Mechanical material type', constMap = constants.MechanicalFemPhysics.materialModels}," << std::endl;
  out << "    }," << std::endl;
  out << "    values = {" << std::endl;
 }

 for (material m : materials)
 {
  switch (m.type)
  {
  case (materialType::ELASTIC) :
   out << "        -- " << m.materialName << std::endl;
   out << "        {E = " << m.youngModulus << ", nu = " << m.poisson << ", Mec_material = 'elastic'}," << std::endl;
   break;
  case (materialType::MOHRCOULOMB) :
   out << "        -- " << m.materialName << std::endl;
   out << "        {E = " << m.youngModulus << ", nu = " << m.poisson << ", Coh = " << m.cohesionMohr << ", Phi = " << m.frictionMohr << ", Psi = " << m.dilationMohr << ", Mec_material = 'mohrCoulomb'}," << std::endl;
   break;
  case (materialType::DRUCKERPRAGER) :
   out << "        -- " << m.materialName << std::endl;
   out << "        {E = " << m.youngModulus << ", nu = " << m.poisson << ", Coh = " << "1" << ", Phi = " << m.frictionDrucker << ", Psi = " << m.dilationDrucker << ", Mec_material = 'druckerPrager'}," << std::endl;
   break;
  case (materialType::VONMISES) :
   out << "        -- " << m.materialName << std::endl;
   out << "        {E = " << m.youngModulus << ", nu = " << m.poisson << ", Sy0 = " << m.yieldStress << ", Mec_material = 'vonMises'}," << std::endl;
   break;
  }
 }

 out << "    }" << std::endl;
 out << "}" << std::endl << std::endl;
}

void printers::printPropertySet(std::ofstream& out, std::vector<sectionAtributtes> sections)
{
 if (out.is_open())
 {
  out << "PropertySet {" << std::endl;
  out << "   id          = 'SecProp'," << std::endl;
  out << "   typeName    = 'GemaPropertySet'," << std::endl;
  out << "   description = 'Section properties'," << std::endl;
  out << "   properties  = {" << std::endl;
  out << "       {id = 'h',  description = 'Element thickness', unit = 'm'}," << std::endl;
  out << "   }," << std::endl;
  out << "   values = {" << std::endl;

  for (sectionAtributtes s : sections)
  {
   out << "        -- " << s.materialName << std::endl;
   out << "       { h =    " << s.thickness << " }," << std::endl;
  }
  out << "    }" << std::endl;
  out << "}" << std::endl << std::endl;
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

void printers::printNodes(std::ofstream& out, std::vector<node> nodes)
{
 if (out.is_open())
 {
  out << "local mesh_nodes = {" << std::endl;
  for (node n : nodes)
  {
   out << "\t {" << n.x << " , " << n.y << "}," << std::endl;
  }
  out << "}" << std::endl << std::endl;
 }
}

void printers::printElementGroups(std::ofstream& out, std::vector<element> elements,
 std::vector<sectionAtributtes> sections, std::vector<setAtributtes> sets)
{
 if (out.is_open())
 {
  for (sectionAtributtes s : sections)
  {
   setAtributtes* sa = nullptr; //pointer init
   for (setAtributtes setAtt : sets)
   {
    if (s.setName == setAtt.name)
    {
     sa = new setAtributtes(setAtt);
     break;
    }
   }
   std::vector<int> setElementsId;
   for (std::string elemIdString : sa->setElem)
   {
    int elemIdInt = utils::string2int(elemIdString);
    setElementsId.push_back(elemIdInt);
   }

   std::string gemaElementType;
   element e = elements[setElementsId.front() - 1];

   if (e.elementType == "CPE4" || e.elementType == "CPE4R" ||
    e.elementType == "CPE4H" || e.elementType == "CPE4I" ||
    e.elementType == "CPE4IH" || e.elementType == "CPE4RH" ||
    e.elementType == "CPS4H" || e.elementType == "CPS4I" ||
    e.elementType == "CPS4IH" || e.elementType == "CPS4RH" ||
    e.elementType == "CPS4" || e.elementType == "CPS4R" ||
    e.elementType == "CPE8" || e.elementType == "CPE8R" ||
    e.elementType == "CPE8H" || e.elementType == "CP84I" ||
    e.elementType == "CPE8IH" || e.elementType == "CPE8RH" ||
    e.elementType == "CPS8H" || e.elementType == "CPS8RH" ||
    e.elementType == "CPS8" || e.elementType == "CPS8R")
   {
    gemaElementType = "Quadrilateral";
   }
   else
   {
    gemaElementType = "Triangular";
   }

   out << "local " << s.materialName << "_" << gemaElementType << "_elements = {" << std::endl;
   for (int idElem : setElementsId)
   {
    element e = elements[idElem - 1];
    out << "    {";
    for (std::string i : e.incidence)
    {
     out << i << ", ";
    }
    out << "}," << std::endl;
   }
   out << "}" << std::endl << std::endl;
  }
 }
}

void printers::printMeshElements(std::ofstream& out, std::vector<element> elements, std::vector<sectionAtributtes> sections, std::vector<material> materials, std::vector<setAtributtes> sets)
{
 if (out.is_open())
 {
  out << "local mesh_elements = {" << std::endl;

  for (material m : materials)
  {
   std::string materialName1 = m.materialName;
   std::string materialName;
   int materialPosition = m.id;
   std::string gemaElementType;
   std::string gemaElementName;
   int sectionPosition;

   for (sectionAtributtes s : sections)
   {
    std::string sectionMaterialName = s.materialName;
    sectionPosition = s.id;

    if (materialName1 == sectionMaterialName)
    {
     materialName = materialName1;

     setAtributtes* sa = nullptr; //pointer init
     for (setAtributtes setAtt : sets)
     {
      if (s.setName == setAtt.name)
      {
       sa = new setAtributtes(setAtt);
       break;
      }
     }
     std::vector<int> setElementsId;
     for (std::string elemIdString : sa->setElem)
     {
      int elemIdInt = utils::string2int(elemIdString);
      setElementsId.push_back(elemIdInt);
     }

     element e = elements[setElementsId.front() - 1];

     if (e.elementType == "CPE4" || e.elementType == "CPE4R" ||
      e.elementType == "CPE4H" || e.elementType == "CPE4I" ||
      e.elementType == "CPE4IH" || e.elementType == "CPE4RH" ||
      e.elementType == "CPS4H" || e.elementType == "CPS4I" ||
      e.elementType == "CPS4IH" || e.elementType == "CPS4RH" ||
      e.elementType == "CPS4" || e.elementType == "CPS4R")
     {
      gemaElementType = "Quadrilateral";
      gemaElementName = "'quad4'";
     }
     else if (e.elementType == "CPE8" || e.elementType == "CPE8R" ||
      e.elementType == "CPE8H" || e.elementType == "CP84I" ||
      e.elementType == "CPE8IH" || e.elementType == "CPE8RH" ||
      e.elementType == "CPS8H" || e.elementType == "CPS8RH" ||
      e.elementType == "CPS8" || e.elementType == "CPS8R")
     {
      gemaElementType = "Quadrilateral";
      gemaElementName = "'quad8'";
     }
     else if (e.elementType == "CPE3" || e.elementType == "CPE3H" | e.elementType == "CPS3")
     {
      gemaElementType = "Triangular";
      gemaElementName = "'tri3'";
     }
     else
     {
      gemaElementType = "Triangular";
      gemaElementName = "'tri6'";
     }
     break;
    }
   }

   if (!materialName.empty())
    out << "    {cellType = " << gemaElementName << ", cellGroup = '" << materialName << "_" << gemaElementType << "', cellList = " << materialName << "_" << gemaElementType << "_elements, MatProp =" << materialPosition << ", SecProp =" << sectionPosition << "}," << " -- " << materialName << std::endl;
  }
  out << "}" << std::endl << std::endl;
 }
}

void printers::printBCEdges(std::ofstream& out, std::vector<load> loads, std::vector<surfaceOfLoadAndBC> surfaces)
{
 if (out.is_open())
 {
  if (surfaces.empty())
   return;

  out << "local bc_edges = {" << std::endl;

  for (surfaceOfLoadAndBC s : surfaces)
  {
   std::string surfaceName = s.surfName;
   std::string surfaceFace = s.surfFace;

   for (load l : loads)
   {
    if (l.loadType == "Pressure" && l.loadSurface == surfaceName)
    {

     out << "    {id = '" << l.loadName << "'," << std::endl;
     out << "     cellList = {" << std::endl;

     for (std::string e : s.surfElem)
     {
      out << "        {" << e << " , " << surfaceFace << "}," << std::endl;
     }

     out << "    }}," << std::endl;
    }
   }
  }
  out << "}" << std::endl;
 }
}
