#include "printers_solution.h"
#include "utils.h"
#include <unordered_set>

void printersSolution::printHeaderSolution(std::ofstream& out)
{
 if (out.is_open())
 {
  out << "-------------------------------------------------------------" << std::endl;
  out << "--  Numerical Solvers" << std::endl;
  out << "-------------------------------------------------------------" << std::endl;
  out << "NumericalSolver {" << std::endl;
  out << "    id          = 'solver'," << std::endl;
  out << "    typeName    = 'ArmadilloSolver'," << std::endl;
  out << "    description = 'Direct matrix solver'," << std::endl;
  out << "}" << std::endl << std::endl;
 }
}

void printersSolution::printPhysical(std::ofstream& out, std::vector<step> steps, std::vector<element> elements, std::vector<sectionAtributtes> sections, std::vector<material> materials, std::vector<setAtributtes> sets, std::vector<boundaryConditions> bc, std::vector<load> loads)
{
 if (out.is_open())
 {
  out << "-------------------------------------------------------------" << std::endl;
  out << "--  Physical Methods" << std::endl;
  out << "-------------------------------------------------------------" << std::endl;


  std::string gemaElementType;
  std::string gemaElementName;
  std::string gemaPlane;
  std::vector<std::string> elementGroup;
  std::unordered_set<std::string> materialsGroup;
  bool hasInitialStep = false;

  for (material m : materials)
  {
   std::string materialName1 = m.materialName;
   std::string materialName;
   int materialPosition = m.id;
   std::string gemaElementType;
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
      e.elementType == "CPE8" || e.elementType == "CPE8R" ||
      e.elementType == "CPE8H" ||
      e.elementType == "CPE8IH" || e.elementType == "CPE8RH")
     {
      gemaElementType = "Quadrilateral";
      gemaPlane = "PlaneStrain";
     }
     else if (e.elementType == "CPS4H" || e.elementType == "CPS4I" ||
              e.elementType == "CPS4IH" || e.elementType == "CPS4RH" ||
              e.elementType == "CPS4" || e.elementType == "CPS4R" ||
              e.elementType == "CPS8H" || e.elementType == "CPS8RH" ||
              e.elementType == "CPS8" || e.elementType == "CPS8R")
     {
      gemaElementType = "Quadrilateral";
      gemaPlane = "PlaneStress";
     }
     else if (e.elementType == "CPE3" || e.elementType == "CPE3H" || e.elementType == "CPE6" || e.elementType == "CPE6H" || e.elementType == "CPE6M" || e.elementType == "CPE6MH")
     {
      gemaElementType = "Triangular";
      gemaPlane = "PlaneStrain";
     }
     else if (e.elementType == "CPS3" || e.elementType == "CPS6" || e.elementType == "CPS6M")
     {
      gemaElementType = "Triangular";
      gemaPlane = "PlaneStress";
     }
     else
     {
      gemaElementType = "Triangular";
     }
     break;
    }
   }
   elementGroup.push_back("'" + materialName + "_" +gemaElementType + "',");

   switch (m.type)
   {
   case (materialType::ELASTIC) :
    materialsGroup.insert("'elastic',");
    break;
   case (materialType::MOHRCOULOMB) :
    materialsGroup.insert("'mohrCoulomb',"); 
    break;
   case (materialType::DRUCKERPRAGER) :
    materialsGroup.insert("'druckerPrager',"); 
    break;
   case (materialType::VONMISES) :
    materialsGroup.insert("'vonMises',");
    break;
   }
  }

  for (boundaryConditions bcd : bc)
  {
   if (bcd.bcStep == "Initial")
   {
    hasInitialStep = true;
    break;
   }
  }


  for (step stp : steps)
  {
   bool hasLoad = false;
   bool hasBC = false;

   for (load li : loads)
   {
    if (li.loadStep == stp.stepName)
     hasLoad = true;
   }

   for (boundaryConditions bch : bc)
   {
    if (bch.bcStep == stp.stepName)
     hasBC = true;
   }

   if (hasLoad || hasBC)
   {

    out << "PhysicalMethod {" << std::endl;
    out << "    id       = '" << stp.stepName << "'," << std::endl;
    out << "    typeName = " << "'MechanicalFemPhysics." << gemaPlane << "'," << std::endl;
    out << "    type     = 'fem'," << std::endl;
    out << "    mesh     = 'mesh'," << std::endl;
    out << "    ruleSet  = 1," << std::endl << std::endl;

    out << "    elementGroups      = { ";

    for (std::string s : elementGroup)
    {
     out << s;
    }

    out << " }," << std::endl;

    out << "    materials          = { ";

    for (std::string mt : materialsGroup)
    {
     out << mt;
    }

    out << " }," << std::endl;

    out << "    boundaryConditions = { ";

    if (hasInitialStep)
    {
     out << "'dispInitial',";
     hasInitialStep = false;
    }

    for (boundaryConditions bcd : bc)
    {
     if (stp.stepName == bcd.bcStep)
     {
      out << "'disp" << bcd.bcStep << "',";
      break;
     }
    }

    bool hasDuplicatePressure = false;
    bool hasDuplicateCload = false;

    for (load ld : loads)
    {
     if (ld.loadStep == stp.stepName)
     {
      if (ld.loadType == "Pressure" && !hasDuplicatePressure)
      {
       out << "'pEdges" << stp.stepName << "',";
       hasDuplicatePressure = true;
      }
      else if (ld.loadType == "Concentrated force" && !hasDuplicateCload)
      {
       out << "'cload" << stp.stepName << "',";
       hasDuplicateCload = true;
      }
     }
    }

    out << " }," << std::endl;

    out << "    properties         = { material = 'Mec_material' }" << std::endl;
    out << "}" << std::endl << std::endl;
   }
  }
 }
}

void printersSolution::printDoFile(std::ofstream& out)
{
 if (out.is_open())
 {
  out << "dofile('$SIMULATIONDIR/AuxData.lua')" << std::endl << std::endl;
 }
}

void printersSolution::printSolver(std::ofstream& out, std::vector<step> steps)
{
 if (out.is_open())
 {
  out << "-------------------------------------------------------------" << std::endl;
  out << "--  Solver options" << std::endl;
  out << "-------------------------------------------------------------" << std::endl;

  for (step s : steps)
  {
   out << "local solverOpitions" << s.stepName << " = {" << std::endl;
   out << "  type               = 'transient nonlinear'" << std::endl;
   out << "  timeMax            = " << s.stepTotal << "," << std::endl;
   out << "  timeInitIncrement  = " << s.stepInit << "," << std::endl;
   out << "  timeMinIncrement   = " << s.stepMin << "," << std::endl;
   out << "  timeMaxIncrement   = " << s.stepMax << "," << std::endl;
   out << "  iterationsMax      = 100," << std::endl;
   out << "  eulerTheta         = 1.000E+00," << std::endl;
   out << "  attemptMax         = 10,  " << std::endl;
   out << "  tolerance          ={mechanic =1.000E-05,}," << std::endl;
   out << "  newtonRaphsonMode      = 'full'," << std::endl;
   out << "  Increment_Time_Factor = 2," << std::endl;
   out << "  Frequency = 1," << std::endl;
   out << "}" << std::endl << std::endl;
  }
 }
}

void printersSolution::printProcess(std::ofstream& out, std::vector<load> loads)
{
 if (out.is_open())
 {
  out << "-------------------------------------------------------------" << std::endl;
  out << "--  Process execution script" << std::endl;
  out << "-------------------------------------------------------------" << std::endl;

  out << "function ProcessScript()" << std::endl;
  out << "   os.execute('if not exist out mkdir out')" << std::endl;

  std::string stepName;

  for (load l : loads)
  {
   stepName = l.loadStep;
   break;
  }

  out << "   io.print('------" << stepName << "-------')" << std::endl;
  out << "   local file = io.prepareMeshFile('mesh', '$SIMULATIONDIR/out/$SIMULATIONNAME', 'nf', {'u',}, {'S', 'E',}, {allstates = true, split = true, saveDisplacements = true})" << std::endl;
  out << "   -- Create the solver model - " << stepName << std::endl;
  out << "   local solver = fem.init({'" << stepName << "',}, 'solver', solverOptions" << stepName << ")" << std::endl;
  out << "   local wfileSta = io.open(translatePath('$SIMULATIONDIR/out/$SIMULATIONNAME.sta'), \"w + \")" << std::endl;
  out << "   wfileSta:write( ' SUMMARY OF JOB INFORMATION:', '\\n')" << std::endl;
  out << "   wfileSta:write( ' INC     ATT  EQUIL   TOTAL     INC OF', '\\n')" << std::endl;
  out << "   wfileSta:write( '              ITERS   TIME     TIME/LPF', '\\n')" << std::endl;
  out << "   local countP = 0" << std::endl;
  out << "   local frequency = solverOptions" << stepName << ".Frequency    -- print each frequency the increments " << std::endl;
  out << "   local dt        = solverOptions" << stepName << ".timeInitIncrement" << std::endl;
  out << "   local FinalTime = solverOptions" << stepName << ".timeMax" << std::endl;
  out << "   local Time      = 0" << std::endl;
  out << "   local LastStep  = false" << std::endl;
  out << "   local countAttemp = 0" << std::endl;
  out << "   local iterMax   = solverOptions" << stepName << ".iterationsMax" << std::endl;
  out << "   io.addResultToMeshFile(file, 0)" << std::endl;
  out << std::endl;
  out << "   while (Time < FinalTime) do  " << std::endl;
  out << "      -- Adjust time to guarantee that the last iteration will be on the " << std::endl;
  out << "      -- requested final simulation time   " << std::endl;
  out << "      if (Time + dt > FinalTime) then dt   = FinalTime - Time end        " << std::endl;
  out << "      io.print(tr('DTime= %1 s') :num(dt))          " << std::endl;
  out << "      local newdt,err,conv,niter = fem.step(solver, dt, true)" << std::endl;
  out << "      wfileSta:write(string.format('%s%5d%s%2d%s%5d%s%11.4e%s%11.4e\\n',' ', countP,' ',countAttemp,' ',niter,' ',Time,' ', dt  )) " << std::endl;
  out << "      if(conv)then" << std::endl;
  out << "         Time = Time +  dt  " << std::endl;
  out << "         io.print(tr('Time Step = %1 s') :num(Time)) " << std::endl;
  out << "         countAttemp = 0" << std::endl;
  out << "         countP = countP + 1" << std::endl;
  out << "         if( countP % frequency == 0 or Time==FinalTime)then" << std::endl;
  out << "            io.addResultToMeshFile(file, Time)" << std::endl;
  out << "         end                         " << std::endl;
  out << "         dt = adaptativeTime(dt,err,conv,niter,countAttemp,solverOptions" << stepName << ")  " << std::endl;
  out << "      else" << std::endl;
  out << "         setCurrentTime(Time)             " << std::endl;
  out << "         countAttemp = countAttemp + 1" << std::endl;
  out << "         dt = adaptativeTime(dt,err,conv,iterMax,countAttemp,solverOptions" << stepName << ")" << std::endl;
  out << "      end" << std::endl;
  out << "   end" << std::endl;
  out << "   io.closeMeshFile(file)" << std::endl;
  out << "   io.close(wfileSta)" << std::endl;
  out << " end" << std::endl;
  out << std::endl;
 }
}