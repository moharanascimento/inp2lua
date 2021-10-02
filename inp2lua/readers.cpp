#include "readers.h"
#include <algorithm>
#include "utils.h"


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
 unsigned last = line.find(lastDelimiter, first+firstDelimiter.size());
 std::string stringBetween = line.substr(first + firstDelimiter.size(), last - first - firstDelimiter.size());
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
  if (stringContain(line, "*Nset"))
   return elements;

  if (stringContain(line, "*Element"))
  {
   text = split(line, "=");
   type = text[1];
   continue;
  }

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

 while (std::getline(file, line))
 {

  if (stringContain(line, "*Solid"))
   break;

  setNodes.clear();

   std::vector<std::string> listNd = split(line, ",");
   setNodes.insert(setNodes.begin(), listNd.begin(), listNd.end());

   while (std::getline(file, line) && !stringContain(line, "*Elset"))
   {
    std::vector<std::string> listNd2 = split(line, ",");
    setNodes.insert(setNodes.end(), listNd2.begin(), listNd2.end());
   }

  if (stringContain(line, "*Elset"))
  {
   setElem.clear();
   std::vector<std::string> text = split(line, "elset=");
   if (stringContain(text[1], ", "))
   {
    name = stringBetween(line, "elset=", ", generate");

    while (std::getline(file, line))
    {
     if (stringContain(line, "**"))
      break;

     if (stringContain(line, "*Nset"))
      break;

     std::vector<std::string> listEl = split(line, ",");
     int number1 = utils::string2int(listEl[0]);
     int number2 = utils::string2int(listEl[1]);
     for (int i = number1; i <= number2; i++)
     {
      std::string str = utils::int2string(i);
      setElem.push_back(str);
     }
    }
   }
   else
   {
    name = text[1];
    while (std::getline(file, line))
    {
     if (stringContain(line, "**"))
      break;

     if (stringContain(line, "*Nset"))
      break;
     std::vector<std::string> listEl = split(line, ",");
     setElem.insert(setElem.begin(), listEl.begin(), listEl.end());
    }
   }
  }

  setAtributtes.emplace_back(setNodes, setElem, name);

  if (stringContain(line, "**"))
   break;
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
 int id = 1;

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

 sectionAtributtes.emplace_back(sectionName, setName, materialName, thickness, id++);

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
  sectionAtributtes.emplace_back(sectionName, setName, materialName, thickness, id++);
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

   std::vector<std::string> setElemBC;

   if (stringContain(line, "*Elset") && !stringContain(line, "internal"))
   {
    std::getline(file, line);
    std::vector<std::string> listEl = split(line, ",");
    std::vector<std::string> filtered;
    std::copy_if(listEl.begin(), listEl.end(), std::back_inserter(filtered), [](std::string s){return !s.empty(); });
    setElemBC.insert(setElemBC.begin(), filtered.begin(), filtered.end());
   }

   if (!setElemBC.empty())
    setOfLoadAndBC.emplace_back(setNodesBC, setElemBC, setNameBC);
   else
    setOfLoadAndBC.emplace_back(setNodesBC, setNameBC);
  }

  if (stringContain(line, "internal"))
   return setOfLoadAndBC;

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
   surfName = stringBetween(line, "elset=_", "_");
   std::vector<std::string> FaceEl = split(line, ", internal");
   surfFace = FaceEl[0].back();
   std::getline(file, line);
   std::vector<std::string> listEl = split(line, ",");
   std::copy_if(listEl.begin(), listEl.end(), std::back_inserter(surfElem), [](std::string s){return !s.empty(); });
   surfaces.emplace_back(surfName, surfFace, surfElem);
  // std::getline(file, line);
  }

  if (stringContain(line, "*Surface"))
  {
   continue;
 //  std::vector<std::string> name = split(line, "name=");
//   surfName = name[1];
  // std::getline(file, line);
 //  std::vector<std::string> face = split(line, ",");
 //  std::vector<std::string> numberOfFace = split(face[1], "S");
 //  surfFace = numberOfFace[1];
//   surfaces.emplace_back(surfName, surfFace, surfElem);
  }
 } while (std::getline(file, line));
 return surfaces;
}

// read and save list of materials
std::vector<material> readers::listOfMaterials(std::ifstream& file, std::string& line)
{
 std::vector <material> materials;
 std::string materialName;
 std::string materialName1;
 std::string youngModulus;
 std::string poisson;
 std::string yieldStress;
 std::string frictionMohr;
 std::string dilationMohr;
 std::string cohesionMohr;
 std::string frictionDrucker;
 std::string dilationDrucker;
 int id = 1;

 std::vector<std::string> text = split(line, "name=");
 materialName1 = text[1];
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
 }

 //adiconado dia 29
 if (stringContain(line, "*Material"))
 {
  std::vector<std::string> text = split(line, "name=");
  materialName = text[1];
 }
 //adicionado dia 29

 if (!isDruckerPrager && !isMohrCoulumb && !isVonMises)
 {
  materials.emplace_back(materialName1, youngModulus, poisson, id++);
 }

 else if (!isDruckerPrager && !isMohrCoulumb && isVonMises)
 {
  materials.emplace_back(materialName1, youngModulus, poisson, yieldStress, id++);
 }

 else if (isDruckerPrager && !isMohrCoulumb && !isVonMises)
 {
  materials.emplace_back(materialName1, youngModulus, poisson, frictionDrucker, dilationDrucker, id++);
 }

 else if (!isDruckerPrager && isMohrCoulumb && !isVonMises)
 {
  materials.emplace_back(materialName1, youngModulus, poisson, frictionMohr, dilationMohr, cohesionMohr, id++);
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
   materials.emplace_back(materialName, youngModulus, poisson, id++);
  }

  else if (isDruckerPrager && !isMohrCoulumb)
  {
   materials.emplace_back(materialName, youngModulus, poisson, frictionDrucker, dilationDrucker, id++);
  }

  else if (!isDruckerPrager && isMohrCoulumb)
  {
   materials.emplace_back(materialName, youngModulus, poisson, frictionMohr, dilationMohr, cohesionMohr, id++);
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
 std::string bcDirection1;
 std::string bcValue1;
 std::string bcDirection2;
 std::string bcValue2;
 std::string bcAxisymmetric;
 bool isAxisymmetric = false;
 //bool isPrescribedDisplacement = false;
 //bool isFixed = false;

 do
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
    std::getline(file, line);
    std::vector<std::string> datas = split(line, ",");
    bcSet = datas[0];

    if (datas.size() == 3)
    {
     bcDirection1 = datas[1];
     bcValue1 = "0";
     std::getline(file, line);

     if (stringContain(line, "** Name"))
     {
      boundaryConditions.emplace_back(bcName, bcType, bcSet, bcDirection1, bcValue1, bcDirection2, bcValue2);
      bcName = stringBetween(line, "** Name: ", " Type");
      std::vector<std::string> type = split(line, "Type: ");
      bcType = type[1];
      continue;
     }

     if (stringContain(line, "**"))
     {
      boundaryConditions.emplace_back(bcName,bcType,bcSet,bcDirection1,bcValue1,bcDirection2,bcValue2);
      continue;
     }
     std::vector<std::string> datas2 = split(line, ",");
     if (datas2.size() == 3)
     {
      bcDirection2 = datas2[1];
      bcValue2 = "0";
      boundaryConditions.emplace_back(bcName, bcType, bcSet, bcDirection1, bcValue1, bcDirection2, bcValue2);
     }
     if (datas2.size() == 4)
     {
      bcDirection2 = datas2[1];
      bcValue2 = datas2[3];
      boundaryConditions.emplace_back(bcName, bcType, bcSet, bcDirection1, bcValue1, bcDirection2, bcValue2);
     }
    }

    if (datas.size() == 4)
    {
     bcDirection1 = datas[1];
     bcValue1 = datas[3];
     std::getline(file, line);

     if (stringContain(line, "** Name"))
     {
      boundaryConditions.emplace_back(bcName, bcType, bcSet, bcDirection1, bcValue1, bcDirection2, bcValue2);
      bcName = stringBetween(line, "** Name: ", " Type");
      std::vector<std::string> type = split(line, "Type: ");
      bcType = type[1];
      continue;
     }

     if (stringContain(line, "**"))
     {
      boundaryConditions.emplace_back(bcName, bcType, bcSet, bcDirection1, bcValue1, bcDirection2, bcValue2);
      continue;
     }
     std::vector<std::string> datas2 = split(line, ",");
     if (datas2.size() == 3)
     {
      bcDirection2 = datas2[1];
      bcValue2 = "0";
      boundaryConditions.emplace_back(bcName, bcType, bcSet, bcDirection1, bcValue1, bcDirection2, bcValue2);
     }
     else if (datas2.size() == 4)
     {
      bcDirection2 = datas2[1];
      bcValue2 = datas2[3];
      boundaryConditions.emplace_back(bcName, bcType, bcSet, bcDirection1, bcValue1, bcDirection2, bcValue2);
     }
    }

    bcDirection1.clear();
    bcDirection2.clear();
    bcValue1.clear();
    bcValue2.clear();


    if (datas.size() == 2)
    {
     bcAxisymmetric = datas[1];
     boundaryConditions.emplace_back(bcName, bcType, bcSet, bcAxisymmetric);
    }
   }
 } while (std::getline(file, line));
 return boundaryConditions;
}

// read and save list of loads
std::vector<load> readers::listOfLoads(std::ifstream& file, std::string& line)
{
 std::vector <load> loads;
 std::string loadName;
 std::string loadType;
 std::string loadSet;
 std::string loadValue;
 std::string loadDirection1;
 std::string loadValue1;
 std::string loadDirection2;
 std::string loadValue2;
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
  }

  if (stringContain(line, "*Cload"))
  {
   std::getline(file, line);
   std::vector<std::string> text2 = split(line, ",");
   loadSet = text2[0];
   loadDirection1 = text2[1];
   loadValue1 = text2[2];
   std::getline(file, line);

    if (stringContain(line, "** Name"))
    {
     loads.emplace_back(loadName, loadType, loadSet, loadDirection1, loadValue1, loadDirection2, loadValue2);
     loadName = stringBetween(line, "** Name: ", " Type");
     std::vector<std::string> type = split(line, "Type: ");
     loadType = type[1];
     continue;
    }

    if (stringContain(line, "**"))
     loads.emplace_back(loadName, loadType, loadSet, loadDirection1, loadValue1, loadDirection2, loadValue2);
     continue;

    std::vector<std::string> text3 = split(line, ",");
    loadSet = text3[0];
    loadDirection2 = text3[1];
    loadValue2 = text3[2];
    loads.emplace_back(loadName, loadType, loadSet, loadDirection1, loadValue1, loadDirection2, loadValue2);
   }

   else if (stringContain(line, "*Dsload"))
   {
    std::getline(file, line);
    std::vector<std::string> text = split(line, ",");
    loadSurface = text[0];
    loadValue = text[2];
    loads.emplace_back(loadName, loadType, loadSurface, loadValue);
   }
 } while (std::getline(file, line));
 return loads;
}