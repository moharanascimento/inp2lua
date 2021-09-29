-------------------------------------------------------------
--  State variables
-------------------------------------------------------------
StateVar{id = 'u', dim = 2, description = 'Displacements in the X and Y directions', unit = 'm', format = '8.4f', groupName = 'mechanic'}

-------------------------------------------------------------
--  Cell properties
-------------------------------------------------------------
PropertySet {
    id          = 'MatProp',
    typeName    = 'GemaPropertySet',
    description = 'Material properties',
    properties  = {
        {id = 'E'  , description = 'Elastic modulus'           , unit = 'kPa'},
        {id = 'nu' , description = 'Poisson ratio'             , unit = ''   },
        {id = 'Coh', description = 'Cohesion'                  , unit = 'kPa'},
        {id = 'Phi', description = 'Angle of internal friction', unit = 'degree'},
        {id = 'Psi', description = 'Angle of dilation'         , unit = 'degree'},
        {id = 'Mec_material', description = 'Mechanical material type', constMap = constants.MechanicalFemPhysics.materialModels},
    },
    values = {
        -- Material-1
        {E = 3., nu =  0.1, Coh = 5., Phi = 30., Psi = 10., Mec_material = 'mohrCoulomb'},
        -- Material-2
        {E = 3., nu =  0.4, Mec_material = 'elastic'},
    }
}

PropertySet {
   id          = 'SecProp',
   typeName    = 'GemaPropertySet',
   description = 'Section properties',
   properties  = {
       {id = 'h',  description = 'Element thickness', unit = 'm'},
   },
   values = {
        -- Material-2
       { h =    10. },
        -- Material-1
       { h =    10. },
    }
}

-------------------------------------------------------------
--  Mesh definition
-------------------------------------------------------------
--  Number of nodes: 9
--  Number of elements: 4

local mesh_nodes = {
	 {          10. ,            5.},
	 {           0. ,            5.},
	 {           0. ,            0.},
	 {          10. ,            0.},
	 {          10. ,           10.},
	 {           0. ,           10.},
	 {           5. ,            5.},
	 {           5. ,            0.},
	 {           5. ,           10.},
}

local Material-2_Quadrilateral_elements = {
    { 1,  7,  8,  4, },
    { 7,  2,  3,  8, },
}

local Material-1_Quadrilateral_elements = {
    { 2,  7,  9,  6, },
    { 7,  1,  5,  9, },
}

local mesh_elements = {
    {cellType = 'quad4', cellGroup = 'Material-1_Quadrilateral', cellList = Material-1_Quadrilateral_elements, MatProp =1, SecProp =2}, -- Material-1
    {cellType = 'quad4', cellGroup = 'Material-2_Quadrilateral', cellList = Material-2_Quadrilateral_elements, MatProp =2, SecProp =1}, -- Material-2
}

