
#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

// INCLUDES
#include <d3d11.h>
#include <DirectXTex.h>

using namespace DirectX;

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, WCHAR*);
	bool LoadCubeTexture(ID3D11Device*, WCHAR*);

	ID3D11ShaderResourceView* GetTexture();

private:
	ID3D11ShaderResourceView* m_texture;
};

#endif