#include "objects.h"
#include "vector"
#include "string"

node::node(std::string x, std::string y)
{
 this->x = x;
 this->y = y;
};

element::element(std::string id, std::vector<std::string> incidence, std::string elementType)
{
 this->id = id;
 this->incidence = incidence;
 this->elementType = elementType;
};

setAtributtes::setAtributtes(std::vector<std::string> setNodes, std::vector<std::string> setElem, std::string name)
{
 this->setNodes = setNodes;
 this->setElem = setElem;
 this->name = name;
}

sectionAtributtes::sectionAtributtes(std::string sectionName, std::string setName, std::string materialName, std::string thickness, int id)
{
 this->sectionName = sectionName;
 this->setName = setName;
 this->materialName = materialName;
 this->id = id;
 this->thickness = thickness;
}

setOfLoadAndBC::setOfLoadAndBC(std::vector<std::string> setNodesBC, std::vector<std::string> setElemBC, std::string setNameBC)
{
 this->setNodesBC = setNodesBC;
 this->setElemBC = setElemBC;
 this->setNameBC = setNameBC;
}

setOfLoadAndBC::setOfLoadAndBC(std::vector<std::string> setNodesBC, std::string setNameBC)
{
 this->setNodesBC = setNodesBC;
 this->setNameBC = setNameBC;
}
surfaceOfLoadAndBC::surfaceOfLoadAndBC(std::string surfName, std::string surfFace, std::vector<std::string> surfElem)
{
 this->surfName = surfName;
 this->surfFace = surfFace;
 this->surfElem = surfElem;
}

material::material(std::string materialName, std::string youngModulus, std::string poisson, int id)
{
 this->materialName = materialName;
 this->youngModulus = youngModulus;
 this->poisson = poisson;
 this->id = id;
 this-> type = materialType::ELASTIC;
}

material::material(std::string materialName, std::string youngModulus, std::string poisson, std::string yieldStress, int id)
{
 this->materialName = materialName;
 this->youngModulus = youngModulus;
 this->poisson = poisson;
 this->yieldStress = yieldStress;
 this->id = id;
 this->type = materialType::VONMISES;
}


material::material(std::string materialName, std::string youngModulus, std::string poisson, std::string frictionMohr, std::string dilationMohr, std::string cohesionMohr, int id)
{
 this->materialName = materialName;
 this->youngModulus = youngModulus;
 this->poisson = poisson;
 this->frictionMohr = frictionMohr;
 this->dilationMohr = dilationMohr;
 this->cohesionMohr = cohesionMohr;
 this->id = id;
 this->type = materialType::MOHRCOULOMB;
}

material::material(std::string materialName, std::string youngModulus, std::string poisson, std::string frictionDrucker, std::string dilationDrucker, int id)
{
 this->materialName = materialName;
 this->youngModulus = youngModulus;
 this->poisson = poisson;
 this->frictionDrucker = frictionDrucker;
 this->dilationDrucker = dilationDrucker;
 this->id = id;
 this->type = materialType::DRUCKERPRAGER;
}

boundaryConditions::boundaryConditions(std::string bcName, std::string bcType, std::string bcSet, std::string bcDirection1, std::string bcValue1, std::string bcDirection2, std::string bcValue2)
{
 this->bcName = bcName;
 this->bcType = bcType;
 this->bcSet = bcSet;
 this->bcDirection1 = bcDirection1;
 this->bcValue1 = bcValue1;
 this->bcDirection2 = bcDirection2;
 this->bcValue2 = bcValue2;
}

boundaryConditions::boundaryConditions(std::string bcName, std::string bcType, std::string bcSet, std::string bcAxisymmetric)
{
 this->bcName = bcName;
 this->bcType = bcType;
 this->bcSet = bcSet;
 this->bcAxisymmetric = bcAxisymmetric;
}

load::load(std::string loadName, std::string loadType, std::string loadSet, std::string loadDirection1, std::string loadValue1, std::string loadDirection2, std::string loadValue2)
{
 this->loadName = loadName;
 this->loadType = loadType;
 this->loadSet = loadSet;
 this->loadDirection1 = loadDirection1;
 this->loadValue1 = loadValue1;
 this->loadDirection2 = loadDirection2;
 this->loadValue2 = loadValue2;
}

load::load(std::string loadName, std::string loadType, std::string loadSurface, std::string loadValue)
{
 this->loadName = loadName;
 this->loadType = loadType;
 this->loadSurface = loadSurface;
 this->loadValue = loadValue;
}

struct matProperty
{
 bool isElastic;
 bool isMohrCoulomb;
 bool isDruckerPrager;
 bool isVonMises;

 std::string E;
 std::string v;
 std::string c;
 std::string phi;
 std::string psi;
 std::string sy;

};

struct secProperty
{
 std::string h;
};

struct edge
{
 std::string h;
};

struct mesh;