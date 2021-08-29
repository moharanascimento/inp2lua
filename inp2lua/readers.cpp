#include "readers.h"
#include <algorithm>


// function to compare strings
bool readers::stringContain(std::string s1, std::string s2)
{
 if (s1.find(s2) != std::string::npos)
  return true;
 return false;
}

// function to get a string between two strings
std::string readers::stringBetween(std::string line, std::string firstDelimiter, std::string lastDelimiter)
{
 unsigned first = line.find(firstDelimiter);
 unsigned last = line.find(lastDelimiter);
 std::string stringBetween = line.substr(first + firstDelimiter.length(), last - first - firstDelimiter.length());
 return stringBetween;
}

// function to separe string in subtrings
std::vector<std::string> split(std::string s, std::string delimiter)
{
 size_t pos_start = 0, pos_end, delim_len = delimiter.length();
 std::string token;
 std::vector<std::string> res;

 while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
 {
  token = s.substr(pos_start, pos_end - pos_start);
  pos_start = pos_end + delim_len;
  res.push_back(token);
 }

 res.push_back(s.substr(pos_start));
 return res;
}

// read and save list of nodes
std::vector<node> readers::listOfNodes(std::ifstream& file, std::string& line)
{
 std::vector <node> nodes;
 while (std::getline(file, line))
 {
  if (stringContain(line, "*"))
   return nodes;

  std::vector<std::string> coord = split(line, ",");
  nodes.emplace_back(coord[1], coord[2]);
 }
 return nodes;
}

// read and save list of elements
std::vector<element> readers::listOfElements(std::ifstream& file, std::string& line)
{
 std::vector <element> elements;
 std::vector<std::string> text = split(line, "=");
 std::string type = text[1];

 while (std::getline(file, line))
 {
  if (stringContain(line, "*"))
   return elements;

  std::vector<std::string> list = split(line, ",");
  std::vector<std::string> incidence;
  incidence.insert(incidence.begin(), list.begin() + 1, list.end());
  elements.emplace_back(list[0], incidence, type);
 }
 return elements;
}

// read and save sets atributtes
std::vector<setAtributtes> readers::listOfSets(std::ifstream& file, std::string& line)
{
 std::vector <setAtributtes> setAtributtes;
 std::vector<std::string> setNodes;
 std::vector<std::string> setElem;
 std::string name;

 std::vector<std::string> text = split(line, "=");
 name = text[1];
 std::getline(file, line);
 std::vector<std::string> listNd = split(line, ",");
 setNodes.insert(setNodes.begin(), listNd.begin(), listNd.end());
 std::getline(file, line);

 if (stringContain(line, "*Elset"))
 {
  std::getline(file, line);
  std::vector<std::string> listEl = split(line, ",");
  setElem.insert(setElem.begin(), listEl.begin(), listEl.end());
 }

 setAtributtes.emplace_back(setNodes, setElem, name);

 while (std::getline(file, line))
 {

  if (stringContain(line, "**"))
   return setAtributtes;

  if (stringContain(line, "*Nset"))
  {
   setNodes.clear();
   std::vector<std::string> text = split(line, "=");
   name = text[1];
   std::getline(file, line);
   std::vector<std::string> listNd = split(line, ",");
   setNodes.insert(setNodes.begin(), listNd.begin(), listNd.end());
   std::getline(file, line);
  }

  if (stringContain(line, "*Elset"))
  {
   setElem.clear();
   std::getline(file, line);
   std::vector<std::string> listEl = split(line, ",");
   setElem.insert(setElem.begin(), listEl.begin(), listEl.end());
  }

  setAtributtes.emplace_back(setNodes, setElem, name);
 }
 return setAtributtes;
}

// read and save sections atributtes
std::vector<sectionAtributtes> readers::listOfSections(std::ifstream& file, std::string& line)
{
 std::vector <sectionAtributtes> sectionAtributtes;
 std::string sectionName;
 std::string setName;
 std::string materialName;
 std::string thickness;

 std::vector<std::string> text = split(line, ": ");
 sectionName = text[1];
 std::getline(file, line);
 std::vector<std::string> names = split(line, "=");
 std::vector<std::string> name1 = split(names[1], ",");
 setName = name1[0];
 materialName = names[2];
 std::getline(file, line);
 std::vector<std::string> thick = split(line, ",");
 thickness = thick[0];

 sectionAtributtes.emplace_back(sectionName, setName, materialName, thickness);

 while (std::getline(file, line))
 {

  if (stringContain(line, "*End"))
   return sectionAtributtes;
  if (stringContain(line, "**"))
  {
   std::vector<std::string> text = split(line, ": ");
   sectionName = text[1];
   std::getline(file, line);
   std::vector<std::string> names = split(line, "=");
   std::vector<std::string> name1 = split(names[1], ",");
   setName = name1[0];
   materialName = names[2];
   std::getline(file, line);
   std::vector<std::string> thick = split(line, ",");
   thickness = thick[0];
  }
  sectionAtributtes.emplace_back(sectionName, setName, materialName, thickness);
 }
 return sectionAtributtes;
}

// read and save sets used in Load and BC
std::vector<setOfLoadAndBC> readers::listOfSetsOfLoadAndBC(std::ifstream& file, std::string& line)
{
 std::vector <setOfLoadAndBC> setOfLoadAndBC;

 std::string setNameBC;

 while (true)
 {

  std::vector<std::string> setNodesBC;

  if (stringContain(line, "*Nset"))
  {
   setNameBC = stringBetween(line, "nset=", ", instance");
   std::getline(file, line);
   std::vector<std::string> listNd = split(line, ",");
   std::vector<std::string> filtered;
   std::copy_if(listNd.begin(), listNd.end(), std::back_inserter(filtered), [](std::string s){return !s.empty(); });
   setNodesBC.insert(setNodesBC.begin(), filtered.begin(), filtered.end());
   std::getline(file, line);
   if (stringContain(line, "*Nset"))
   {
    setOfLoadAndBC.emplace_back(setNodesBC, setNameBC);
    continue;
   }

   if (stringContain(line, "internal"))
    return setOfLoadAndBC;

   if (stringContain(line, "*Elset"))
   {
    std::vector<std::string> setElemBC;
    std::getline(file, line);
    std::vector<std::string> listEl = split(line, ",");
    std::vector<std::string> filtered;
    std::copy_if(listEl.begin(), listEl.end(), std::back_inserter(filtered), [](std::string s){return !s.empty(); });
    setElemBC.insert(setElemBC.begin(), listEl.begin(), listEl.end());
    setOfLoadAndBC.emplace_back(setNodesBC, setElemBC, setNameBC);
   }
  }
  std::getline(file, line);
 }
 return setOfLoadAndBC;
}

// read and save surfaces used in Load and BC
std::vector<surfaceOfLoadAndBC> readers::listOfSurfaces(std::ifstream& file, std::string& line)
{
  std::vector <surfaceOfLoadAndBC> surfaces; 

 do
 {
  std::string surfName;
  std::string surfFace;
  std::vector<std::string> surfElem;

  if (stringContain(line, "*End"))
   return surfaces;

  if (stringContain(line, "*Elset"))
  {
   std::getline(file, line);
   std::vector<std::string> listEl = split(line, ",");
   std::copy_if(listEl.begin(), listEl.end(), std::back_inserter(surfElem), [](std::string s){return !s.empty(); });
   std::getline(file, line);
  }

  if (stringContain(line, "*Surface"))
  {
   std::vector<std::string> name = split(line, "name=");
   surfName = name[1];
   std::getline(file, line);
   std::vector<std::string> face = split(line, ",");
   std::vector<std::string> numberOfFace = split(face[1], "S");
   surfFace = numberOfFace[1];
   surfaces.emplace_back(surfName, surfFace, surfElem);
  }
 } while (std::getline(file, line));
 return surfaces;
}

// read and save list of materials
std::vector<material> readers::listOfMaterials(std::ifstream& file, std::string& line)
{
 std::vector <material> materials;
 std::string materialName;
 std::string youngModulus;
 std::string poisson;
 std::string yieldStress;
 std::string frictionMohr;
 std::string dilationMohr;
 std::string cohesionMohr;
 std::string frictionDrucker;
 std::string dilationDrucker;

 std::vector<std::string> text = split(line, "name=");
 materialName = text[1];
 std::getline(file, line);

 bool isDruckerPrager = false;
 bool isMohrCoulumb = false;
 bool isVonMises = false;

 if (stringContain(line, "*Drucker Prager"))
 {
  std::getline(file, line);
  std::vector<std::string> angles = split(line, ",");
  frictionDrucker = angles[0];
  dilationDrucker = angles[2];
  isDruckerPrager = true;
  std::getline(file, line);
  std::getline(file, line);
  std::getline(file, line);
 }

 if (stringContain(line, "*Elastic"))
 {
  std::getline(file, line);
  std::vector<std::string> parameters = split(line, ",");
  youngModulus = parameters[0];
  poisson = parameters[1];
  std::getline(file, line);
 }

 if (stringContain(line, "*Plastic"))
 {
  std::getline(file, line);
  std::vector<std::string> plastic = split(line, ",");
  yieldStress = plastic[0];
  isVonMises = true;
  std::getline(file, line);
 }

 if (stringContain(line, "*Mohr Coulomb"))
 {
  std::getline(file, line);
  std::vector<std::string> angles = split(line, ",");
  frictionMohr = angles[0];
  dilationMohr = angles[1];
  isMohrCoulumb = true;
  std::getline(file, line);
 }

 if (stringContain(line, "*Mohr Coulomb Hardening"))
 {
  std::getline(file, line);
  std::vector<std::string> angles = split(line, ",");
  cohesionMohr = angles[0];
  std::getline(file, line);
 }

 if (!isDruckerPrager && !isMohrCoulumb && !isVonMises)
 {
  materials.emplace_back(materialName, youngModulus, poisson);
 }

 else if (!isDruckerPrager && !isMohrCoulumb && isVonMises)
 {
  materials.emplace_back(materialName, youngModulus, poisson, yieldStress);
 }

 else if (isDruckerPrager && !isMohrCoulumb && !isVonMises)
 {
  materials.emplace_back(materialName, youngModulus, poisson, frictionDrucker, dilationDrucker);
 }

 else if (!isDruckerPrager && isMohrCoulumb && !isVonMises)
 {
  materials.emplace_back(materialName, youngModulus, poisson, frictionMohr, dilationMohr, cohesionMohr);
 }

 while (std::getline(file, line))
 {

  if (stringContain(line, "**"))
   return materials;

  if (stringContain(line, "*Material"))
  {
   std::vector<std::string> text = split(line, "name=");
   materialName = text[1];
   std::getline(file, line);
  }

  isDruckerPrager = false;
  isMohrCoulumb = false;

  if (stringContain(line, "*Drucker Prager"))
  {
   std::getline(file, line);
   std::vector<std::string> angles = split(line, ",");
   frictionDrucker = angles[0];
   dilationDrucker = angles[2];
   isDruckerPrager = true;
   std::getline(file, line);
   std::getline(file, line);
   std::getline(file, line);
  }

  if (stringContain(line, "*Elastic"))
  {
   std::getline(file, line);
   std::vector<std::string> parameters = split(line, ",");
   youngModulus = parameters[0];
   poisson = parameters[1];
   std::getline(file, line);
  }

  if (stringContain(line, "*Plastic"))
  {
   std::getline(file, line);
   std::vector<std::string> plastic = split(line, ",");
   yieldStress = plastic[0];
   isVonMises = true;
   std::getline(file, line);
  }

  if (stringContain(line, "*Mohr Coulomb"))
  {
   std::getline(file, line);
   std::vector<std::string> angles = split(line, ",");
   frictionMohr = angles[0];
   dilationMohr = angles[1];
   isMohrCoulumb = true;
   std::getline(file, line);
  }

  if (stringContain(line, "*Mohr Coulomb Hardening"))
  {
   std::getline(file, line);
   std::vector<std::string> angles = split(line, ",");
   frictionMohr = angles[0];
   dilationMohr = angles[1];
   std::getline(file, line);
  }

  if (!isDruckerPrager && !isMohrCoulumb)
  {
   materials.emplace_back(materialName, youngModulus, poisson);
  }

  else if (isDruckerPrager && !isMohrCoulumb)
  {
   materials.emplace_back(materialName, youngModulus, poisson, frictionDrucker, dilationDrucker);
  }

  else if (!isDruckerPrager && isMohrCoulumb)
  {
   materials.emplace_back(materialName, youngModulus, poisson, frictionMohr, dilationMohr, cohesionMohr);
  }

 }
 return materials;
}

// read and save list of boundary conditions
std::vector<boundaryConditions> readers::listOfBoundaryConditions(std::ifstream& file, std::string& line)
{
 std::vector <boundaryConditions> boundaryConditions;
 std::string bcName;
 std::string bcType;
 std::string bcSet;
 std::string bcDirection;
 std::string bcValue;
 std::string bcAxisymmetric;
 bool isAxisymmetric = false;
 bool isPrescribedDisplacement = false;
 bool isFixed = false;

 while (std::getline(file, line))
 {

  if (stringContain(line, "** LOADS"))
   return boundaryConditions;

  if (stringContain(line, "** Name"))
  {
   bcName = stringBetween(line, "** Name: ", " Type");
   std::vector<std::string> type = split(line, "Type: ");
   bcType = type[1];
   std::getline(file, line);
  }

  if (stringContain(line, "*Boundary"))
  {
   while (std::getline(file, line))
   {
    std::vector<std::string> datas = split(line, ",");
    bcSet = datas[0];

    if (stringContain(line, "**"))
    {
     break;
    }

    if (datas.size() == 3)
    {
     bcDirection = datas[1];
     bcValue = "0";
     isFixed = true;
    }

    if (datas.size() == 4)
    {
     bcDirection = datas[1];
     bcValue = datas[3];
     isPrescribedDisplacement = true;
    }

    if (datas.size() == 2)
    {
     bcAxisymmetric = datas[1];
    }

    if (isPrescribedDisplacement || isFixed)
    {
     boundaryConditions.emplace_back(bcName, bcType, bcSet, bcDirection, bcValue);
    }

    else if (isAxisymmetric)
    {
     boundaryConditions.emplace_back(bcName, bcType, bcSet, bcAxisymmetric);
    }
   }
  }
 }
 return boundaryConditions;
}

// read and save list of loads
std::vector<load> readers::listOfLoads(std::ifstream& file, std::string& line)
{
 std::vector <load> loads;
 std::string loadName;
 std::string loadType;
 std::string loadSet;
 std::string loadDirection;
 std::string loadValue;
 std::string loadSurface;

 do
 {
  if (stringContain(line, "** OUTPUT"))
   return loads;

  if (stringContain(line, "** Name"))
  {
   loadName = stringBetween(line, "** Name: ", " Type");
   std::vector<std::string> type = split(line, "Type: ");
   loadType = type[1];
   std::getline(file, line);

   if (stringContain(line, "*Cload"))
   {

    std::getline(file, line);
    std::vector<std::string> text2 = split(line, ",");
    loadSet = text2[0];
    loadDirection = text2[1];
    loadValue = text2[2];
    loads.emplace_back(loadName, loadType, loadSet, loadDirection, loadValue);
    std::getline(file, line);
    if (stringContain(line, "** Name"))
     continue;
    std::vector<std::string> text3 = split(line, ",");
    loadSet = text3[0];
    loadDirection = text3[1];
    loadValue = text3[2];
    loads.emplace_back(loadName, loadType, loadSet, loadDirection, loadValue);
   }

   else if (stringContain(line, "*Dsload"))
   {
    std::getline(file, line);
    std::vector<std::string> text = split(line, ",");
    loadSurface = text[0];
    loadValue = text[2];
    loads.emplace_back(loadName, loadType, loadSurface, loadValue);
   }
  }
 } while (std::getline(file, line));
 return loads;
}