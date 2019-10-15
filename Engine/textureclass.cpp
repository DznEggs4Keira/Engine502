
#include "textureclass.h"


TextureClass::TextureClass()
{
	m_texture = 0;
}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
	// Release the texture resource.
	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
}


bool TextureClass::Initialize(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;

	// Load the texture in.

	TexMetadata imageMetadata;
	ScratchImage* pScratchImage = new ScratchImage();

	result = LoadFromDDSFile(filename, DDS_FLAGS_NONE, &imageMetadata, *pScratchImage);
	if (FAILED(result))
	{
		return false;
	}

	result = CreateShaderResourceView(device, pScratchImage->GetImages(), 
		pScratchImage->GetImageCount(), imageMetadata, &m_texture);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}


ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}

bool TextureClass::LoadCubeTexture(ID3D11Device* device, WCHAR* filename)
{
	HRESULT result;

	//D3DX11_IMAGE_LOAD_INFO loadSMInfo;
	//loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	TexMetadata imageMetadata;
	ScratchImage* pScratchImage = new ScratchImage();

	result = LoadFromDDSFile(filename, TEX_MISC_TEXTURECUBE, &imageMetadata, *pScratchImage);
	if (FAILED(result))
	{
		return false;
	}

	ID3D11Texture2D* SMTexture = 0;
	result = CreateTexture(device, pScratchImage->GetImages(),
		pScratchImage->GetImageCount(), imageMetadata, (ID3D11Resource**)&SMTexture);
	if (FAILED(result))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);

	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	result = device->CreateShaderResourceView(SMTexture, &SMViewDesc, &m_texture);

	if (FAILED(result))
	{
		return false;
	}

	return true;
}