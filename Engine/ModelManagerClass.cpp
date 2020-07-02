#include "ModelManagerClass.h"

ModelManagerClass::ModelManagerClass()
{
	m_SkySphere = nullptr;
	m_Terrain = nullptr;
	m_Water = nullptr;
	m_TerrWater = nullptr; 
}

ModelManagerClass::ModelManagerClass(const ModelManagerClass &)
{
}


ModelManagerClass::~ModelManagerClass()
{
	// Release the water object.
	SAFE_DELETE(m_Water)

	//Release the terrain object.
	SAFE_DELETE(m_Terrain)

	// Release the sky dome object.
	SAFE_DELETE(m_SkySphere)
	
	//Release the assimp objects
	AssimpModelClass* p;
	SAFE_DELETE_VECTOR(p, m_Tree)
	SAFE_DELETE_VECTOR(p, m_Polyhedron)

	//Release the terrain object.
	SAFE_DELETE(m_TerrWater)
}

bool ModelManagerClass::Initialize(ID3D11Device* device , ID3D11DeviceContext* deviceContext, HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	m_pDevice = device;
	m_pDeviceContext = deviceContext;

	// Create the Water object.
	m_TerrWater = new TerrainClass;
	if (!m_TerrWater)
	{
		return false;
	}

	//Initialise the Water object
	if (FAILED(result = m_TerrWater->Initialize(m_pDevice, "../Engine/data/Textures/Heightmap_Island.bmp", 
		L"../Engine/data/Textures/seamelessWater.dds", "../Engine/data/Textures/colorm01.bmp")))
	{
		ErrorLogger::Log(result, "Could not Initialize Water");
		return false;
	}

	// Create the Terrain object.
	m_Terrain = new TerrainClass;
	if (!m_Terrain)
	{
		return false;
	}

	//Initialise the Terrain object
	if (FAILED(result = m_Terrain->Initialize(m_pDevice, "../Engine/data/Textures/Heightmap_Billow.bmp", 
		L"../Engine/data/Textures/dirt01.dds", "../Engine/data/Textures/colorm01.bmp")))
	{
		ErrorLogger::Log(result, "Could not Initialize Terrain");
		return false;
	}

	//Creat Tree Object
	AssimpModelClass *m_pTree = new AssimpModelClass();
	if (!m_pTree)
	{
		return false;
	}

	//Initialise Tree
	if (FAILED(result = m_pTree->Initialize(m_pDevice, "../Engine/data/Models/TiltedTree.obj", L"../Engine/data/Textures/bark.dds")))
	{
		ErrorLogger::Log(result, "Could not initialize the Tree test object.");
		return false;
	}

	//Add Tree to Tree Vector of Meshes
	m_Tree.push_back(m_pTree);

	// Repeat for Polyhedron
	AssimpModelClass *m_pPolyhedron = new AssimpModelClass();
	if (!m_pPolyhedron)
	{
		return false;
	}
	if (FAILED(result = m_pPolyhedron->Initialize(m_pDevice, "../Engine/data/Models/polyhedronBall.obj", L"../Engine/data/Textures/seafloor.dds")))
	{
		ErrorLogger::Log(result, "Could not initialize the Polyhedron test object.");
		return false;
	}

	m_Polyhedron.push_back(m_pPolyhedron);

	// Create the sky dome object.
	m_SkySphere = new skySphere;
	if (!m_SkySphere)
	{
		return false;
	}

	// Initialize the sky dome object.
	if (FAILED(result = m_SkySphere->Initialize(m_pDevice, L"../Engine/data/Textures/DysDesert.dds")))
	{
		ErrorLogger::Log(result, "Could not initialize the sky dome object.");
		return false;
	}

	// Create the water object.
	m_Water = new WaterClass;
	if (!m_Water)
	{
		return false;
	}

	// Initialize the water object.
	if (FAILED(result = m_Water->Initialize(m_pDevice, L"../Engine/data/Textures/waternormal.dds", 3.0f, 30.0f)))
	{
		ErrorLogger::Log(result, "Could not initialize the water object.");
		return false;
	}
	return true;
}

void ModelManagerClass::RenderAssimpModel(AssimpModelClass* p)
{
	//Try rendering the assimp model
	for (int i = 0; i < p->m_Meshes.size(); i++)
	{
		p->m_Meshes[i]->Render(m_pDeviceContext);
	}
}

