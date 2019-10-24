#include "ModelManagerClass.h"

ModelManagerClass::ModelManagerClass()
{
	m_SkySphere = nullptr;
	m_Terrain = nullptr;
	m_Water = nullptr;
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

	// Release Assimp Models
	for each (AssimpModelClass* p in m_Tree)
	{
		delete p;
		p = 0;
	}

	for each (AssimpModelClass* p in m_Polyhedron)
	{
		delete p;
		p = 0;
	}
}

bool ModelManagerClass::Initialize(ID3D11Device* device , ID3D11DeviceContext* deviceContext, HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	m_pDevice = device;
	m_pDeviceContext = deviceContext;

	// Create the Terrain object.
	m_Terrain = new TerrainClass;
	if (!m_Terrain)
	{
		return false;
	}

	//Initialise the Terrain object
	if (FAILED(result = m_Terrain->Initialize(m_pDevice, "../Engine/data/Textures/Heightmap_Rocky.bmp", L"../Engine/data/Textures/grass.dds", "../Engine/data/Textures/colorm01.bmp")))
	{
		MessageBox(hwnd, L"Could not Initialize Terrain", L"Error", MB_OK);
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
		MessageBox(hwnd, L"Could not initialize the Polyhedron test object.", L"Error", MB_OK);
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
		MessageBox(hwnd, L"Could not initialize the Polyhedron test object.", L"Error", MB_OK);
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
	if (FAILED(result = m_SkySphere->Initialize(m_pDevice, L"../Engine/data/Textures/Skyrim.dds")))
	{
		MessageBox(hwnd, L"Could not initialize the sky dome object.", L"Error", MB_OK);
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
		MessageBox(hwnd, L"Could not initialize the water object.", L"Error", MB_OK);
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

