
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_

// MY CLASS INCLUDES
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "inputclass.h"
#include "Movement.h"
#include "FrameTime.h"
#include "skySphere.h"
#include "skySphereShader.h"
#include "terrainclass.h"
#include "terrainshaderclass.h"

#include "rendertextureclass.h"
#include "reflectionshaderclass.h"
#include "waterclass.h"
#include "watershaderclass.h"

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
	ModelClass* m_Model;
	ModelClass* m_Tree;
	LightShaderClass* m_LightShader;
	LightShaderClass* m_TextureShader;
	LightClass* m_Light;
	InputClass* m_Input;
	Movement* m_Movement;
	FrameTime* m_Timer;
	skySphere* m_SkySphere;
	skySphereShader* m_SkySphereShader;
	TerrainClass* m_Terrain;
	TerrainShaderClass* m_TerrainShader;

	RenderTextureClass *m_RefractionTexture, *m_ReflectionTexture;
	ReflectionShaderClass* m_ReflectionShader;
	WaterClass* m_Water;
	WaterShaderClass* m_WaterShader;
};

#endif