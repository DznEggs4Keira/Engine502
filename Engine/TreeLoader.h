
#ifndef _TREELOADER_H_
#define _TREELOADER_H_


// INCLUDES 
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>

//not good practice
using namespace std;

// MY CLASS INCLUDES 
#include "textureclass.h"


class TreeClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
	};

	struct InstanceType
	{
		D3DXVECTOR3 position;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	TreeClass();
	TreeClass(const TreeClass&);
	~TreeClass();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetVertexCount();
	int GetInstanceCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadModel(char*);
	void ReleaseModel();

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer *m_vertexBuffer, *m_instanceBuffer;
	int m_vertexCount, m_instanceCount;;
	ModelType* m_model;
	TextureClass* m_Texture;
};

#endif