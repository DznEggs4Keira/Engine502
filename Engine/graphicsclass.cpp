
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = nullptr;
	m_Camera = nullptr;
	m_Light = nullptr;

	m_pModelManager = nullptr;

	m_pShaderManager = nullptr;
	m_RefractionTexture = nullptr;
	m_ReflectionTexture = nullptr;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
	// Release the water shader object.
	SAFE_DELETE(m_pShaderManager)

	SAFE_DELETE(m_pModelManager)

	// Release the reflection render to texture object.
	SAFE_DELETE(m_ReflectionTexture)

	// Release the refraction render to texture object.
	SAFE_DELETE(m_RefractionTexture)

	// Release the light object.
	SAFE_DELETE(m_Light)

	// Release the camera object.
	SAFE_DELETE(m_Camera)

	// Release the D3D object.
	SAFE_DELETE(m_D3D)

}


bool GraphicsClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;
	float cameraX = 50.0f,
		  cameraY = 1.0f,
		  cameraZ = 25.0f;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	if(FAILED(result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR)))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the Shader Manager object.
	m_pShaderManager = new ShaderManagerClass;
	if (!m_pShaderManager)
	{
		return false;
	}

	// Initialize theShader Manager object.
	if(FAILED(result = m_pShaderManager->Initialize(m_D3D->GetDevice(), hwnd)))
	{
		MessageBox(hwnd, L"Could not initialize the shader manager object.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(cameraX, cameraY, cameraZ);

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	//m_Light->SetTerrainAmbientColor(0.83f, 0.70f, 0.25f, 1.0f);
	m_Light->SetAmbientColor(0.83f, 0.70f, 0.25f, 1.0f);
	m_Light->SetDiffuseColor(0.83f, 0.70f, 0.25f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Light->SetSpecularPower(60.0f);

	//Setup render to textures for the refraction and reflection of the scene.

	// Create the refraction render to texture object.
	m_RefractionTexture = new RenderTextureClass;
	if (!m_RefractionTexture)
	{
		return false;
	}

	// Initialize the refraction render to texture object.
	if(FAILED(result = m_RefractionTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR)))
	{
		MessageBox(hwnd, L"Could not initialize the refraction render to texture object.", L"Error", MB_OK);
		return false;
	}

	// Create the reflection render to texture object.
	m_ReflectionTexture = new RenderTextureClass;
	if (!m_ReflectionTexture)
	{
		return false;
	}

	// Initialize the reflection render to texture object.
	if(FAILED(result = m_ReflectionTexture->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, SCREEN_DEPTH, SCREEN_NEAR)))
	{
		MessageBox(hwnd, L"Could not initialize the reflection render to texture object.", L"Error", MB_OK);
		return false;
	}

	m_pModelManager = new ModelManagerClass;
	if (!m_pModelManager)
	{
		return false;
	}

	result = m_pModelManager->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hinstance, hwnd, screenWidth, screenHeight);
	if (FAILED(result))
	{
		MessageBox(hwnd, L"Could not initialize the Models.", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool GraphicsClass::Frame()
{
	static float rotation = 0.0f;
	static float delta = 0.0f;

	// Update the rotation variable each frame. --- for cube model
	rotation += (float)XM_PI * 0.05f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	// Update the delta variable each frame. (keep this between 0 and 1) --- for cube model
	delta += 0.001;
	if (delta > 1.0f)
	{
		delta -= 1.0f;
	}

	m_pModelManager->Frame();

	// Render the refraction of the scene to a texture.
	RenderRefractionToTexture();

	// Render the reflection of the scene to a texture.
	RenderReflectionToTexture(rotation, delta);

	// Render the graphics.
	if (!Render(rotation, delta))
	{
		return false;
	}

	return true;
}

void GraphicsClass::MoveCamera(XMFLOAT3 pos, XMFLOAT3 rot)
{
	// Set the position of the camera.
	m_Camera->SetPosition(pos.x, pos.y, pos.z);
	m_Camera->SetRotation(rot.x, rot.y, rot.z);
}

void GraphicsClass::RenderRefractionToTexture()
{
	XMFLOAT4 clipPlane;
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;


	// Setup a clipping plane based on the height of the water to clip everything above it to create a refraction.
	clipPlane = XMFLOAT4(0.0f, -1.0f, 0.0f, m_pModelManager->m_Water->GetWaterHeight() + 0.1f);

	// Set the render target to be the refraction render to texture.
	m_RefractionTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the refraction render to texture.
	m_RefractionTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the matrices from the camera and d3d objects.
	worldMatrix = m_D3D->GetWorldMatrix();
	viewMatrix = m_Camera->GetViewMatrix();
	projectionMatrix = m_D3D->GetProjectionMatrix();

	// Render the terrain using the reflection shader and the refraction clip plane to produce the refraction effect.
	m_pModelManager->RenderTerrain();

	m_pShaderManager->RenderReflectionShader(m_D3D->GetDeviceContext(), m_pModelManager->m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_pModelManager->m_Terrain->GetTexture(),
		clipPlane);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3D->ResetViewport();

	return;
}

void GraphicsClass::RenderReflectionToTexture(float rotation, float deltavalue)
{
	XMFLOAT4 clipPlane;
	XMMATRIX reflectionViewMatrix, worldMatrix, projectionMatrix;
	XMMATRIX modelTranslateMatrix, modelWorldMatrix, modelScaleMatrix;
	XMFLOAT3 cameraPosition;


	//add new variables which control in what axis the cube rotates
	float rotationX = rotation * 0.02f;
	float rotationY = rotation * 0.03f;
	float rotationZ = rotation * 0.4f;

	// Setup a clipping plane based on the height of the water to clip everything below it.
	clipPlane = XMFLOAT4(0.0f, 1.0f, 0.0f, -m_pModelManager->m_Water->GetWaterHeight());

	// Set the render target to be the reflection render to texture.
	m_ReflectionTexture->SetRenderTarget(m_D3D->GetDeviceContext());

	// Clear the reflection render to texture.
	m_ReflectionTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Use the camera to render the reflection and create a reflection view matrix.
	m_Camera->RenderReflection(m_pModelManager->m_Water->GetWaterHeight());

	// Get the camera reflection view matrix instead of the normal view matrix.
	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();

	// Get the world and projection matrices from the d3d object.
	worldMatrix = m_D3D->GetWorldMatrix();
	projectionMatrix = m_D3D->GetProjectionMatrix();

	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	// Invert the Y coordinate of the camera around the water plane height for the reflected camera position.
	cameraPosition.y = -cameraPosition.y + (m_pModelManager->m_Water->GetWaterHeight() * 2.0f);

	// Translate the sky dome and sky plane to be centered around the reflected camera position.
	worldMatrix = XMMatrixTranslation(cameraPosition.x, cameraPosition.y, cameraPosition.z);

	// Turn off back face culling and the Z buffer.
	m_D3D->TurnOffCulling();
	m_D3D->TurnZBufferOff();

	// Render the sky dome using the reflection view matrix.
	m_pModelManager->RenderSkySphere();
	m_pShaderManager->RenderSkySphereShader(m_D3D->GetDeviceContext(), m_pModelManager->m_SkySphere->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix,
		m_pModelManager->m_SkySphere->GetApexColor(), m_pModelManager->m_SkySphere->GetCenterColor(), m_pModelManager->m_SkySphere->GetTexture());

	// Enable back face culling.
	m_D3D->TurnOnCulling();
	m_D3D->TurnZBufferOn();

	// Reset the world matrix.
	worldMatrix = m_D3D->GetWorldMatrix();

	// Render the terrain using the reflection view matrix and reflection clip plane.
	m_pModelManager->RenderTerrain();
	m_pShaderManager->RenderReflectionShader(m_D3D->GetDeviceContext(), m_pModelManager->m_Terrain->GetIndexCount(), worldMatrix, reflectionViewMatrix, projectionMatrix,
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_pModelManager->m_Terrain->GetTexture(), clipPlane);

	// Reset the world matrix.
	worldMatrix = m_D3D->GetWorldMatrix();

	// Rotate the world matrix by the rotation value so that the sphere will spin.
	modelWorldMatrix = XMMatrixRotationRollPitchYaw(rotationX, rotationY, rotationZ);
	modelScaleMatrix = XMMatrixTranslation(0.05f, 0.05f, 0.05f);
	modelTranslateMatrix = XMMatrixTranslation(50.0f, 10.0f, 50.0f);
	worldMatrix = XMMatrixMultiply(XMMatrixMultiply(modelWorldMatrix, modelScaleMatrix), modelTranslateMatrix);

	for each(AssimpModelClass* p in m_pModelManager->m_Polyhedron)
	{
		m_pModelManager->RenderAssimpModel(p);

		// Try rendering the assimp model using the light shader.
		m_pShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), p->indices.size(), worldMatrix, reflectionViewMatrix, projectionMatrix,
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), deltavalue, p->GetTexture());
	}

	// Reset the world matrix.
	worldMatrix = m_D3D->GetWorldMatrix();

	modelTranslateMatrix = XMMatrixTranslation(100.0f, 20.0f, 100.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, modelTranslateMatrix);

	for each (AssimpModelClass* p in m_pModelManager->m_Tree)
	{
		m_pModelManager->RenderAssimpModel(p);

		// Try rendering the assimp model using the light shader.
		m_pShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), p->indices.size(), worldMatrix, reflectionViewMatrix, projectionMatrix,
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), deltavalue, p->GetTexture());
	}

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3D->ResetViewport();

	return;
}

bool GraphicsClass::Render(float rotation, float deltavalue)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, reflectionViewMatrix;
	XMMATRIX modelWorldMatrix, modelTranslateMatrix, modelScaleMatrix;

	//for sky sphere
	XMFLOAT3 cameraPosition;

	bool result;

	//add new variables which control in what axis the cube rotates
	float rotationX = rotation * 0.02f;
	float rotationY = rotation * 0.03f;
	float rotationZ = rotation * 0.4f;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Generate the reflection matrix based on the camera's position and the height of the water.
	m_Camera->RenderReflection(m_pModelManager->m_Water->GetWaterHeight());

	// Get the world, view, and projection matrices from the camera and d3d objects.
	viewMatrix = m_Camera->GetViewMatrix();
	worldMatrix = m_D3D->GetWorldMatrix();
	modelWorldMatrix = m_D3D->GetWorldMatrix();
	projectionMatrix = m_D3D->GetProjectionMatrix();
	reflectionViewMatrix = m_Camera->GetReflectionViewMatrix();

	//Here is where we get the position of the camera and then translate our sky dome to be centered around the camera position.
	//SKY SPHERE BARF
	// Get the position of the camera.
	cameraPosition = m_Camera->GetPosition();

	//modelScaleMatrix = XMMatrixScaling(500.0f, 100.0f, 500.0f);
	modelScaleMatrix = XMMatrixScaling(100.0f, 100.0f, 100.0f);
	// Translate the sky sphere to be centered around the camera position.
	modelTranslateMatrix = XMMatrixTranslation(cameraPosition.x, cameraPosition.y, cameraPosition.z);
	worldMatrix = XMMatrixMultiply(modelScaleMatrix, modelTranslateMatrix);

	// Turn off back face culling.
	m_D3D->TurnOffCulling();
	// Turn off the Z buffer.
	m_D3D->TurnZBufferOff();

	//Render the Sphere
	m_pModelManager->RenderSkySphere();

	// Render the sky sphere using the sky sphere shader.
	result = m_pShaderManager->RenderSkySphereShader(m_D3D->GetDeviceContext(), m_pModelManager->m_SkySphere->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_pModelManager->m_SkySphere->GetApexColor(), m_pModelManager->m_SkySphere->GetCenterColor(), m_pModelManager->m_SkySphere->GetTexture());

	//check if sky sphere rendered successfully
	if (!result)
	{
		return false;
	}

	// Turn back face culling back on.
	m_D3D->TurnOnCulling();
	// Turn the Z buffer back on.
	m_D3D->TurnZBufferOn();

	// Reset the world matrix.
	worldMatrix = m_D3D->GetWorldMatrix();

	modelTranslateMatrix = XMMatrixTranslation(0.0f, -10.0f, 0.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, modelTranslateMatrix);

	// Render the terrain buffers.
	m_pModelManager->RenderTerrain();

	// Render the terrain using the terrain shader.
	result = m_pShaderManager->RenderTerrainShader(m_D3D->GetDeviceContext(), m_pModelManager->m_Terrain->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
		m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Light->GetDirection(), m_pModelManager->m_Terrain->GetTexture());
	
	//check if terrain rendered successfully
	if (!result)
	{
		return false;
	}

	// Reset the world matrix.
	worldMatrix = m_D3D->GetWorldMatrix();

	// Translate to the location of the water and render it.

	modelTranslateMatrix = XMMatrixTranslation(40.0f, m_pModelManager->m_Water->GetWaterHeight(), 50.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, modelTranslateMatrix);

	//Render water
	m_pModelManager->RenderWater();

	//using water shader
	result = m_pShaderManager->RenderWaterShader(m_D3D->GetDeviceContext(), m_pModelManager->m_Water->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, reflectionViewMatrix,
		m_RefractionTexture->GetShaderResourceView(), m_ReflectionTexture->GetShaderResourceView(), m_pModelManager->m_Water->GetTexture(),
		m_Camera->GetPosition(), m_pModelManager->m_Water->GetNormalMapTiling(), m_pModelManager->m_Water->GetWaterTranslation(), m_pModelManager->m_Water->GetReflectRefractScale(),
		m_pModelManager->m_Water->GetRefractionTint(), m_Light->GetDirection(), m_pModelManager->m_Water->GetSpecularShininess());

	//check if water rendered successfully
	if (!result)
	{
		return false;
	}

	// Reset the world matrix.
	worldMatrix = m_D3D->GetWorldMatrix();

	// Rotate the world matrix by the rotation value so that the sphere will spin.
	modelWorldMatrix = XMMatrixRotationRollPitchYaw(rotationX, rotationY, rotationZ);
	modelScaleMatrix = XMMatrixTranslation(0.05f, 0.05f, 0.05f);
	modelTranslateMatrix = XMMatrixTranslation(50.0f, 10.0f, 50.0f);
	worldMatrix = XMMatrixMultiply(XMMatrixMultiply(modelWorldMatrix, modelScaleMatrix), modelTranslateMatrix);

	for each(AssimpModelClass* p in m_pModelManager->m_Polyhedron)
	{
		m_pModelManager->RenderAssimpModel(p);

		// Try rendering the assimp model using the light shader.
		m_pShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), p->indices.size(), worldMatrix, viewMatrix, projectionMatrix,
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), deltavalue, p->GetTexture());
	}

	// Reset the world matrix.
	worldMatrix = m_D3D->GetWorldMatrix();

	modelTranslateMatrix = XMMatrixTranslation(100.0f, 20.0f, 100.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, modelTranslateMatrix);

	for each (AssimpModelClass* p in m_pModelManager->m_Tree)
	{
		m_pModelManager->RenderAssimpModel(p);

		// Try rendering the assimp model using the light shader.
		m_pShaderManager->RenderLightShader(m_D3D->GetDeviceContext(), p->indices.size(), worldMatrix, viewMatrix, projectionMatrix,
			m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(),
			m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower(), deltavalue, p->GetTexture());
	}

	// Present the rendered scene to the screen.
	m_D3D->EndScene();

	return true;
}