
#ifndef _SKYDOMECLASS_H_
#define _SKYDOMECLASS_H_


// INCLUDES
#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

//my includes
#include "textureclass.h"

using namespace DirectX;


class skySphere
{
private:
	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VertexType
	{
		XMFLOAT3 position;
	};

public:
	skySphere();
	skySphere(const skySphere&);
	~skySphere();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

	XMFLOAT4 GetApexColor();
	XMFLOAT4 GetCenterColor();

	//get texture for cube map
	ID3D11ShaderResourceView* GetTexture();

private:
	bool LoadSkySphere(char*);
	void ReleaseSkySphere();

	//load up and release textures for cube map
	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool InitializeBuffers(ID3D11Device*);
	void ReleaseBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ModelType* m_model;
	int m_vertexCount, m_indexCount;
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	XMFLOAT4 m_apexColor, m_centerColor;
	TextureClass* m_Texture;
};
#endif

