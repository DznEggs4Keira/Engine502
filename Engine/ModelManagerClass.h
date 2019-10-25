#ifndef _MODELMANAGERCLASS_H_
#define _MODELMANAGERCLASS_H_

//Models
#include "waterclass.h"
#include "terrainclass.h"
#include "skySphere.h"
#include "AssimpModelClass.h"

//Utils
#include "Utils.h"

class ModelManagerClass
{
public:
	ModelManagerClass();
	ModelManagerClass(const ModelManagerClass&);
	~ModelManagerClass();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, HINSTANCE, HWND, int, int);

	void ModelManagerClass::Frame() { m_Water->Frame(); }

	void ModelManagerClass::RenderWater() { m_Water->Render(m_pDeviceContext); }

	void ModelManagerClass::RenderTerrain()	{ m_Terrain->Render(m_pDeviceContext); }

	void ModelManagerClass::RenderTerrWater() { m_TerrWater->Render(m_pDeviceContext); }

	void ModelManagerClass::RenderSkySphere() { m_SkySphere->Render(m_pDeviceContext); }

	void ModelManagerClass::RenderAssimpModel(AssimpModelClass*);

public:
	std::vector<AssimpModelClass*> m_Tree, m_Polyhedron;
	WaterClass* m_Water;
	TerrainClass* m_Terrain;
	TerrainClass* m_TerrWater;
	skySphere* m_SkySphere;

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
};

#endif

