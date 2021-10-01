#ifndef OBJECTS_INP_2_LUA
#define OBJECTS_INP_2_LUA
#include <string>
#include <vector>

class node
{
public:
 node(std::string x, std::string y);
 std::string x;
 std::string y;
};

enum class elementType{ CPE4, CPE4R, CPE8, CPE8R, CPE3, CPE6, CPS4, CPS4R, CPS8, CPS8R, CPS3, CPS6, UNDEF};

class element
{
public:
 element(std::string id, std::vector<std::string> incidence, std::string elementType);
 std::string id;
 std::vector<std::string> incidence;
 std::string elementType;

};

class setAtributtes
{
public:
 setAtributtes(std::vector<std::string> setNodes, std::vector<std::string> setElem, std::string name);
 std::vector<std::string> setNodes;
 std::vector<std::string> setElem;
 std::string name;
};

class sectionAtributtes
{
public:
 sectionAtributtes(std::string sectionName, std::string setName, std::string materialName, std::string thickness, int id);
 std::string sectionName;
 std::string setName;
 std::string materialName;
 int id;
 std::string thickness;
};

class setOfLoadAndBC
{
public:
 setOfLoadAndBC(std::vector<std::string> setNodesBC, std::string setNameBC);
 setOfLoadAndBC(std::vector<std::string> setNodesBC, std::vector<std::string> setElemBC, std::string setNameBC);
 std::vector<std::string> setNodesBC;
 std::vector<std::string> setElemBC;
 std::string setNameBC;
};

class surfaceOfLoadAndBC
{
public:
 surfaceOfLoadAndBC(std::string surfName, std::string surfFace, std::vector<std::string> surfElem);
 std::string surfName;
 std::string surfFace;
 std::vector<std::string> surfElem;
};

enum class materialType{ELASTIC, VONMISES, MOHRCOULOMB, DRUCKERPRAGER};

class material
{
public:
 materialType type;

 material(std::string materialName, std::string youngModulus, std::string poisson, int id);
 material(std::string materialName, std::string youngModulus, std::string poisson, std::string yieldStress, int id);
 material(std::string materialName, std::string youngModulus, std::string poisson, std::string frictionMohr, std::string dilationMohr, std::string cohesionMohr, int id);
 material(std::string materialName, std::string youngModulus, std::string poisson, std::string frictionDrucker, std::string dilationDrucker, int id); 
 
 std::string materialName;
 std::string youngModulus;
 std::string poisson;
 std::string yieldStress;
 std::string frictionMohr;
 std::string dilationMohr;
 std::string cohesionMohr;
 std::string frictionDrucker;
 std::string dilationDrucker;
 int id;
};

class boundaryConditions
{
public:
 boundaryConditions(std::string bcName, std::string bcType, std::string bcSet, std::string bcDirection, std::string bcValue);
 boundaryConditions(std::string bcName, std::string bcType, std::string bcSet, std::string bcAxisymmetric);

 std::string bcName;
 std::string bcType;
 std::string bcSet;
 std::string bcDirection;
 std::string bcValue;
 std::string bcAxisymmetric;
};

class load
{
public:
 load(std::string loadName, std::string loadType, std::string loadSet, std::string loadDirection1, std::string loadValue1, std::string loadDirection2, std::string loadValue2);
 load(std::string loadName, std::string loadType, std::string loadSurface, std::string loadValue); 
 
 std::string loadName;
 std::string loadType;
 std::string loadSet;
 std::string loadSurface;
 std::string loadDirection1;
 std::string loadValue;
 std::string loadValue1;
 std::string loadDirection2;
 std::string loadValue2;
};

#endif