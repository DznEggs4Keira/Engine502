
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

// MY CLASS INCLUDES
//Main
#include "d3dclass.h"
#include "cameraclass.h"
#include "lightclass.h"
#include "inputclass.h"

//Movement
#include "Movement.h"
#include "FrameTime.h"

//Models
#include "waterclass.h"
#include "terrainclass.h"
#include "skySphere.h"
#include "modelclass.h"
#include "AssimpModelClass.h"

//Shader
#include "ShaderManagerClass.h"
#include "rendertextureclass.h"



// GLOBALS 
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	bool HandleInput(float);

	void RenderRefractionToTexture();
	void RenderReflectionToTexture(float);

	bool Render(float, float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	LightClass* m_Light;
	InputClass* m_Input;

	Movement* m_Movement;
	FrameTime* m_Timer;

	std::vector<AssimpModelClass*> m_AssimpModel;
	ModelClass* m_Model;
	ModelClass* m_Tree;
	TerrainClass* m_Terrain;
	skySphere* m_SkySphere;
	WaterClass* m_Water;

	//one pointer for the shader manager class which will overwrite all the other seperate shader pointers 
	ShaderManagerClass* m_ShaderManager;

	RenderTextureClass *m_RefractionTexture, *m_ReflectionTexture;
};

#endif