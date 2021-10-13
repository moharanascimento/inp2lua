#include "printers.h"
#include "utils.h"
#include <algorithm>

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

static void sortMaterials(std::vector<material>& materials, const std::vector<sectionAtributtes>& sections, const std::vector<setAtributtes>& sets)
{
 std::sort(materials.begin(), materials.end(), [sections,sets](material m1, material m2)
 {
  setAtributtes setM1 = sets[0];
  setAtributtes setM2 = sets[0];

  for (sectionAtributtes sec : sections)
  {
   if (sec.materialName == m1.materialName)
   {
    for (setAtributtes set : sets)
    {
     if (sec.setName == set.name)
      setM1 = set;
    }
   }
   if (sec.materialName == m2.materialName)
   {
    for (setAtributtes set : sets)
    {
     if (sec.setName == set.name)
      setM2 = set;
    }
   }
  }

  int initElemM1 = utils::string2int(setM1.setElem[0]);
  int initElemM2 = utils::string2int(setM2.setElem[0]);

  return initElemM1 < initElemM2;

 });

}


void printers::printMatProperties(std::ofstream& out, std::vector<material> materials, std::vector<setAtributtes> sets, std::vector<sectionAtributtes> sections)
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

  sortMaterials(materials, sections, sets);

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

  bool hasPressure = false;

  for (load p : loads)
  {
   if (p.loadType == "Pressure")
    hasPressure = true;
  }

  if (hasPressure)
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

  if (hasPressure)
  out << "}" << std::endl << std::endl;
 }
}

void printers::printMesh(std::ofstream& out, std::vector<element> elements, std::vector<surfaceOfLoadAndBC> surfaces, std::vector<load> loads)
{
 if (out.is_open())
 {
  out << "Mesh {" << std::endl;
  out << "    -- General mesh attributes" << std::endl;
  out << "    id          = 'mesh'," << std::endl;
  out << "    typeName    = 'GemaMesh.elem'," << std::endl;
  out << "    description = 'mesh discretization'," << std::endl;
  out << "    " << std::endl;
  out << "    -- Mesh dimensions" << std::endl;
  out << "    coordinateDim  =   2," << std::endl;
  out << "    coordinateUnit = 'm'," << std::endl;
  out << "    " << std::endl;
  out << "    -- State vars stored in this mesh (per node)" << std::endl;
  out << "    stateVars = { 'u',}," << std::endl;
  out << "    " << std::endl;
  out << "    -- Mesh node coordinates" << std::endl;
  out << "    nodeData = mesh_nodes," << std::endl;
  out << "    " << std::endl;
  out << "    -- Element properties" << std::endl;
  out << "    cellProperties = {'MatProp', 'SecProp'}," << std::endl;
  out << "    " << std::endl;
  out << "    -- Element data" << std::endl;
  out << "    cellData = mesh_elements," << std::endl;
  out << "    " << std::endl;
  out << "    -- IntegrationRules" << std::endl;
  out << "    elementRules = {" << std::endl;

  bool hasQuad4 = false;
  bool hasQuad4R = false;
  bool hasQuad8 = false;
  bool hasQuad8R = false;
  bool hasTri3 = false;
  bool hasTri6 = false;

  for (element e : elements)
  {

   if (e.elementType == "CPE4R" || e.elementType == "CPE4RH" ||
       e.elementType == "CPS4RH" ||e.elementType == "CPS4R")
   {
    hasQuad4R = true;
   }
   else if (e.elementType == "CPE4" || e.elementType == "CPS4H" ||
    e.elementType == "CPE4H" || e.elementType == "CPE4I" ||
    e.elementType == "CPE4IH" || e.elementType == "CPS4IH" ||
    e.elementType == "CPS4")
   {
    hasQuad4 = true;
   }
   else if (e.elementType == "CPE8R" || e.elementType == "CPE8RH" ||
            e.elementType == "CPS8RH" || e.elementType == "CPS8R")
   {
    hasQuad8 = true;
   }
   else if (e.elementType == "CPE8" || e.elementType == "CPE8H" || 
    e.elementType == "CPE8IH" || e.elementType == "CP84I" ||
    e.elementType == "CPS8H" || e.elementType == "CPS8")
   {
    hasQuad8 = true;
   }
   else if (e.elementType == "CPE3" || e.elementType == "CPE3H" | e.elementType == "CPS3")
   {
    hasTri3 = true;
   }
   else
   {
    hasTri6 = true;
   }
  }

  out << "        { ";

  if (hasQuad4)
   out << "quad4 = 2, ";
  if (hasQuad4R)
   out << "quad4 = 1, ";
  if (hasQuad8)
   out << "quad8 = 3, ";
  if (hasQuad8R)
   out << "quad8 = 2, ";
  if (hasTri3)
   out << "tri3 = 1, ";
  if (hasTri6)
   out << "tri6 = 3, ";

  out << "}," << std::endl;
  out << "    }," << std::endl;

  bool hasPressure = false;

  for (load p : loads)
  {
   if (p.loadType == "Pressure")
    hasPressure = true;
  }

  if (!hasPressure)
   out << "}" << std::endl <<std::endl;

  else
  {
   out << std::endl;
   out << "    -- Boundary data" << std::endl;
   out << "    boundaryEdgeData = bc_edges," << std::endl;
   out << "}" << std::endl << std::endl;
  }
 }
}

void printers::printBoundaryConditions(std::ofstream& out, std::vector<load> loads, std::vector<setOfLoadAndBC> sets, std::vector<boundaryConditions> bc)
{
 if (out.is_open())
 {
  out << "-------------------------------------------------------------" << std::endl;
  out << "--  Boundary conditions" << std::endl;
  out << "-------------------------------------------------------------" << std::endl;

  bool cloadTitle = false;
  bool pressureTitle = false;
  bool dispTitle = false;
  int pressureNumber = 0;
  int dispNumber = 0;

  for (load l : loads)
  {
   if (l.loadType == "Concentrated force")
   {
    if (!cloadTitle)
    {
     out << "BoundaryCondition {" << std::endl;
     out << "    id   = 'cload'," << std::endl;
     out << "    type = 'node concentrated forces'," << std::endl;
     out << "    mesh = 'mesh'," << std::endl;
     out << "    properties  = {" << std::endl;
     out << "        {id = 'f',  description = 'External force applied on the node', unit = 'kN', dim = 2}," << std::endl;
     out << "    }," << std::endl;
     out << "    nodeValues = {" << std::endl;
     cloadTitle = true;
    }

    for (setOfLoadAndBC set : sets)
    {
     if (set.setNameBC == l.loadSet)
     {
      for (std::string node : set.setNodesBC)
      {
       if (l.loadDirection2.empty() && l.loadDirection1 == " 1")
       {
        out << "        {" << node << ",";
        out << " { " << l.loadValue1 << ", 0} }, -- " << l.loadName << std::endl;
       }
       else if (l.loadDirection2.empty() && l.loadDirection1 == " 2")
       {
        out << "        {" << node << ",";
        out << " { 0," << l.loadValue1 << "} }, -- " << l.loadName << std::endl;
       }
       else if (l.loadDirection1 == " 1" && l.loadDirection2 == " 2")
       {
        out << "        {" << node << ",";
        out << " { " << l.loadValue1 << ", " << l.loadValue2 << "} }, -- " << l.loadName << std::endl;
       }
       else if (l.loadDirection1 == " 2" && l.loadDirection2 == " 1")
       {
        out << "        {" << node << ",";
        out << " { " << l.loadValue2 << ", " << l.loadValue1 << "} }, -- " << l.loadName << std::endl;
       }
      }
     }
    }
    out << "    }" << std::endl;
    out << "}" << std::endl << std::endl;
   }

   if (l.loadType == "Pressure")
   {
    if (!pressureTitle)
    {
     out << "BoundaryCondition {" << std::endl;
     out << "    id   = 'pEdges"<< pressureNumber << "'," << std::endl;
     out << "    type = 'pressure load'," << std::endl;
     out << "    mesh = 'mesh'," << std::endl;
     out << "    properties  = {" << std::endl;
     out << "        {id = 'pl',  description = 'Pressure loading on edges', unit = 'kPa'}," << std::endl;
     out << "    }," << std::endl;
     out << "    edgeValues = {" << std::endl;
     pressureTitle = true;
    }

    out << "        {'" << l.loadName << "' , " << l.loadValue << "}," << std::endl;
   }
  }

  if (pressureTitle)
  {
   out << "    }" << std::endl;
   out << "}" << std::endl << std::endl;
  }

  for (boundaryConditions boundCond : bc)
  {
   if (!dispTitle)
   {
    out << "BoundaryCondition {" << std::endl;
    out << "    id   = 'disp"<< dispNumber <<"'," << std::endl;
    out << "    type = 'node displacements'," << std::endl;
    out << "    mesh = 'mesh'," << std::endl;
    out << "    properties  = {" << std::endl;
    out << "        {id = 'ux',  description = 'Fixed node displacement in the X direction', unit = 'm', defVal = -9999}," << std::endl;
    out << "        {id = 'uy',  description = 'Fixed node displacement in the Y direction', unit = 'm', defVal = -9999}," << std::endl;
    out << "    }," << std::endl;
    out << "    nodeValues = {" << std::endl;
    dispTitle = true;
   }

   if (boundCond.bcType == "Displacement/Rotation")
   {
    for (setOfLoadAndBC set : sets)
    {
     if (set.setNameBC == boundCond.bcSet)
     {
      for (std::string node : set.setNodesBC)
      {
       if (boundCond.bcDirection2.empty() && boundCond.bcDirection1 == " 1")
       {
        out << "        { " << node << ", ";
        out << boundCond.bcValue1 << ", nil }, -- " << boundCond.bcName << std::endl;
       }
       else if (boundCond.bcDirection2.empty() && boundCond.bcDirection1 == " 2")
       {
        out << "        { " << node << ",";
        out << " nil, " << boundCond.bcValue1 << " }, -- " << boundCond.bcName << std::endl;
       }
       else if (boundCond.bcDirection1 == " 1" && boundCond.bcDirection2 == " 2")
       {
        out << "        { " << node << ", ";
        out << boundCond.bcValue1 << ", " << boundCond.bcValue2 << " }, -- " << boundCond.bcName << std::endl;
       }
       else if (boundCond.bcDirection1 == " 2" && boundCond.bcDirection2 == " 1")
       {
        out << "        { " << node << ", ";
        out << boundCond.bcValue2 << ", " << boundCond.bcValue1 << " }, -- " << boundCond.bcName << std::endl;
       }
      }
     }
    }
   }

   if (boundCond.bcType == "Symmetry/Antisymmetry/Encastre")
   {
    for (setOfLoadAndBC set : sets)
    {
     if (set.setNameBC == boundCond.bcSet)
     {
      for (std::string node : set.setNodesBC)
      {
       if (boundCond.bcAxisymmetric == " XSYMM")
       {
        out << "        { " << node << ", ";
        out << "0, nil }, -- " << boundCond.bcName << std::endl;
       }
       else if (boundCond.bcAxisymmetric == " YSYMM")
       {
        out << "        { " << node << ",";
        out << " nil, 0 }, -- " << boundCond.bcName << std::endl;
       }
       else if (boundCond.bcAxisymmetric == " ZASYMM")
       {
        out << "        { " << node << ", ";
        out << " nil, 0 }, -- " << boundCond.bcName << std::endl;
       }
       else if (boundCond.bcAxisymmetric == " PINNED")
       {
        out << "        { " << node << ", ";
        out << " nil, 0 }, -- " << boundCond.bcName << std::endl;
       }
       else if (boundCond.bcAxisymmetric == " ENCASTRE")
       {
        out << "        { " << node << ", ";
        out << " nil, 0 }, -- " << boundCond.bcName << std::endl;
       }
      }
     }
    }
   }
  }

  out << "    }" << std::endl;
  out << "}" << std::endl << std::endl;

 }
}

void printers::printSignature(std::ofstream& out)
{
 if (out.is_open())
  out << "-- Generated by Abaqus2GeMA v0.1.0";
}