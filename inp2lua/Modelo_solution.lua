-------------------------------------------------------------
--  Numerical Solvers
-------------------------------------------------------------
NumericalSolver {
    id          = 'solver',
    typeName    = 'ArmadilloSolver',
    description = 'Direct matrix solver',
}

-------------------------------------------------------------
--  Physical Methods
-------------------------------------------------------------
PhysicalMethod {
    id       = 'Step2',
    typeName = 'MechanicalFemPhysics.PlaneStress',
    type     = 'fem',
    mesh     = 'mesh',
    ruleSet  = 1,

    elementGroups      = { 'Material1_Quadrilateral','Material2_Quadrilateral', },
    materials          = { 'mohrCoulomb','elastic', },
    boundaryConditions = { 'dispStep2','cloadStep2','pEdgesStep2', },
    properties         = { material = 'Mec_material' }
}

dofile('$SIMULATIONDIR/AuxData.lua')

-------------------------------------------------------------
--  Solver options
-------------------------------------------------------------
local solverOptionsStep1 = {
  type               = 'transient nonlinear',
  timeMax            =  1.,
  timeInitIncrement  = 1.,
  timeMinIncrement   =  1e-05,
  timeMaxIncrement   =  1.,
  iterationsMax      = 100,
  eulerTheta         = 1.000E+00,
  attemptMax         = 10,  
  tolerance          ={mechanic =1.000E-03,},
  newtonRaphsonMode      = 'full',
  Increment_Time_Factor = 2,
  Frequency = 1,
}

local solverOptionsStep2 = {
  type               = 'transient nonlinear',
  timeMax            =  1.,
  timeInitIncrement  = 1.,
  timeMinIncrement   =  1e-05,
  timeMaxIncrement   =  1.,
  iterationsMax      = 100,
  eulerTheta         = 1.000E+00,
  attemptMax         = 10,  
  tolerance          ={mechanic =1.000E-03,},
  newtonRaphsonMode      = 'full',
  Increment_Time_Factor = 2,
  Frequency = 1,
}

-------------------------------------------------------------
--  Process execution script
-------------------------------------------------------------
function ProcessScript()
   os.execute('if not exist out mkdir out')
   io.print('------Step2-------')
   local file = io.prepareMeshFile('mesh', '$SIMULATIONDIR/out/$SIMULATIONNAME', 'nf', {'u',}, {'S', 'E',}, {allstates = true, split = true, saveDisplacements = true})
   -- Create the solver model - Step2
   local solver = fem.init({'Step2',}, 'solver', solverOptionsStep2)
   local wfileSta = io.open(translatePath('$SIMULATIONDIR/out/$SIMULATIONNAME.sta'), "w+")
   wfileSta:write( ' SUMMARY OF JOB INFORMATION:', '\n')
   wfileSta:write( ' INC     ATT  EQUIL   TOTAL     INC OF', '\n')
   wfileSta:write( '              ITERS   TIME     TIME/LPF', '\n')
   local countP = 0
   local frequency = solverOptionsStep2.Frequency    -- print each frequency the increments 
   local dt        = solverOptionsStep2.timeInitIncrement
   local FinalTime = solverOptionsStep2.timeMax
   local Time      = 0
   local LastStep  = false
   local countAttemp = 0
   local iterMax   = solverOptionsStep2.iterationsMax
   io.addResultToMeshFile(file, 0)

   while (Time < FinalTime) do  
      -- Adjust time to guarantee that the last iteration will be on the 
      -- requested final simulation time   
      if (Time + dt > FinalTime) then dt   = FinalTime - Time end        
      io.print(tr('DTime= %1 s') :num(dt))          
      local newdt,err,conv,niter = fem.step(solver, dt, true)
      wfileSta:write(string.format('%s%5d%s%2d%s%5d%s%11.4e%s%11.4e\n',' ', countP,' ',countAttemp,' ',niter,' ',Time,' ', dt  )) 
      if(conv)then
         Time = Time +  dt  
         io.print(tr('Time Step = %1 s') :num(Time)) 
         countAttemp = 0
         countP = countP + 1
         if( countP % frequency == 0 or Time==FinalTime)then
            io.addResultToMeshFile(file, Time)
         end                         
         dt = adaptativeTime(dt,err,conv,niter,countAttemp,solverOptionsStep2)  
      else
         setCurrentTime(Time)             
         countAttemp = countAttemp + 1
         dt = adaptativeTime(dt,err,conv,iterMax,countAttemp,solverOptionsStep2)
      end
   end
   io.closeMeshFile(file)
   io.close(wfileSta)
 end

-- Generated by Abaqus2GeMA v0.1.0