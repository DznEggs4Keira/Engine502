#ifndef _MESHCLASS_H_
#define _MESHCLASS_H_

// Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

//My Class Includes

using namespace DirectX;

struct Vertex
{
	XMFLOAT3 position;
	XMFLOAT2 texture;
	XMFLOAT3 normal;
};

class MeshClass
{
public:
	MeshClass(ID3D11Device*, std::vector<Vertex>, std::vector<DWORD>);
	MeshClass(const MeshClass& Mesh);
	~MeshClass();
	
	//Initialize
	bool Initialize(ID3D11Device*, std::vector<Vertex>, std::vector<DWORD>);

	//Render
	void Render(ID3D11DeviceContext*);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
};

#endif