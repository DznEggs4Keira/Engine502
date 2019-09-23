////////////////////////////////////////////////////////////////////////////////
// Filename: shadermanagerclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _SHADERMANAGERCLASS_H_
#define _SHADERMANAGERCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"

//All the single ladies (shaders)
#include "lightshaderclass.h"
#include "reflectionshaderclass.h"
#include "skySphereShader.h"
#include "terrainshaderclass.h"
#include "watershaderclass.h"

class ShaderManagerClass
{
public:
	ShaderManagerClass();
	ShaderManagerClass(const ShaderManagerClass&);
	~ShaderManagerClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	//seperate render functions for each shader
	bool RenderLightShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, XMFLOAT3, XMFLOAT4, XMFLOAT4,
		XMFLOAT3, XMFLOAT4, float, float deltavalue, ID3D11ShaderResourceView*);
	
	bool RenderReflectionShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, XMFLOAT4, XMFLOAT4,
		XMFLOAT3, ID3D11ShaderResourceView*, XMFLOAT4);

	bool RenderSkySphereShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, XMFLOAT4, XMFLOAT4, ID3D11ShaderResourceView*);

	bool RenderTerrainShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, XMFLOAT4, XMFLOAT4, XMFLOAT3, ID3D11ShaderResourceView*);

	bool RenderWaterShader(ID3D11DeviceContext*, int, XMMATRIX, XMMATRIX, XMMATRIX, XMMATRIX, ID3D11ShaderResourceView*, ID3D11ShaderResourceView*,
		ID3D11ShaderResourceView*, XMFLOAT3, XMFLOAT2, float, float, XMFLOAT4, XMFLOAT3, float);

private:
	LightShaderClass* m_LightShader;
	ReflectionShaderClass* m_ReflectionShader;
	skySphereShader* m_SkySphereShader;
	TerrainShaderClass* m_TerrainShader;
	WaterShaderClass* m_WaterShader;

};

#endif
