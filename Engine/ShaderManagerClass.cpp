////////////////////////////////////////////////////////////////////////////////
// Filename: shadermanagerclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "shadermanagerclass.h"


ShaderManagerClass::ShaderManagerClass()
{
	m_LightShader = 0;
	m_ReflectionShader = 0;
	m_SkySphereShader = 0;
	m_TerrainShader = 0;
	m_WaterShader = 0;
}

ShaderManagerClass::ShaderManagerClass(const ShaderManagerClass& other)
{
}

ShaderManagerClass::~ShaderManagerClass()
{
	//Release the Reflection Shader object.
	if (m_ReflectionShader)
	{
		m_ReflectionShader->Shutdown();
		delete m_ReflectionShader;
		m_ReflectionShader = 0;
	}

	// Release the water shader object.
	if (m_WaterShader)
	{
		m_WaterShader->Shutdown();
		delete m_WaterShader;
		m_WaterShader = 0;
	}

	// Release the terrain shader object.
	if (m_TerrainShader)
	{
		m_TerrainShader->Shutdown();
		delete m_TerrainShader;
		m_TerrainShader = 0;
	}

	// Release the sky sphere shader object.
	if (m_SkySphereShader)
	{
		m_SkySphereShader->Shutdown();
		delete m_SkySphereShader;
		m_SkySphereShader = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}
}

bool ShaderManagerClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the sky sphere shader object.
	m_SkySphereShader = new skySphereShader;
	if (!m_SkySphereShader)
	{
		return false;
	}

	// Initialize the sky sphere shader object.
	result = m_SkySphereShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sky sphere shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the Terrain shader object.
	m_TerrainShader = new TerrainShaderClass;
	if (!m_TerrainShader)
	{
		return false;
	}

	// Initialize the Terrain shader object.
	result = m_TerrainShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the terrain shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the water shader object.
	m_WaterShader = new WaterShaderClass;
	if (!m_WaterShader)
	{
		return false;
	}

	// Initialize the water shader object.
	result = m_WaterShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the water shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the Reflection shader object.
	m_ReflectionShader = new ReflectionShaderClass;
	if (!m_ReflectionShader)
	{
		return false;
	}

	// Initialize the Reflection shader object.
	result = m_ReflectionShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the Reflection shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool ShaderManagerClass::RenderLightShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, XMFLOAT3 lightDirection, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor,
	XMFLOAT3 cameraPosition, XMFLOAT4 specularColor, float specularPower, float deltavalue, ID3D11ShaderResourceView* texture)
{
	bool result;


	// Render the model using the texture shader.
	result = m_LightShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, lightDirection, ambientColor, diffuseColor,
		cameraPosition, specularColor, specularPower, deltavalue, texture);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderManagerClass::RenderReflectionShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection,
	ID3D11ShaderResourceView* texture, XMFLOAT4 clipPlane)
{
	bool result;


	// Render the model using the texture shader.
	result = m_ReflectionShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, ambientColor,
		diffuseColor, lightDirection, texture, clipPlane);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderManagerClass::RenderSkySphereShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, XMFLOAT4 apexColor, XMFLOAT4 centerColor, ID3D11ShaderResourceView* texture)
{
	bool result;


	// Render the model using the texture shader.
	result = m_SkySphereShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix,
		apexColor, centerColor, texture);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderManagerClass::RenderTerrainShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, XMFLOAT4 ambientColor, XMFLOAT4 diffuseColor, XMFLOAT3 lightDirection,
	ID3D11ShaderResourceView* texture)
{
	bool result;
	
	// Render the model using the texture shader.
	result = m_TerrainShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, 
		ambientColor, diffuseColor, lightDirection, texture);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ShaderManagerClass::RenderWaterShader(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX worldMatrix, XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix, XMMATRIX reflectionMatrix, ID3D11ShaderResourceView* refractionTexture,
	ID3D11ShaderResourceView* reflectionTexture, ID3D11ShaderResourceView* normalTexture, XMFLOAT3 cameraPosition,
	XMFLOAT2 normalMapTiling, float waterTranslation, float reflectRefractScale, XMFLOAT4 refractionTint,
	XMFLOAT3 lightDirection, float specularShininess)
{
	bool result;


	// Render the model using the texture shader.
	result = m_WaterShader->Render(deviceContext, indexCount, worldMatrix, viewMatrix, projectionMatrix, reflectionMatrix, 
		refractionTexture, reflectionTexture, normalTexture, cameraPosition, normalMapTiling,
		waterTranslation, reflectRefractScale, refractionTint, lightDirection, specularShininess);
	if (!result)
	{
		return false;
	}

	return true;
}
