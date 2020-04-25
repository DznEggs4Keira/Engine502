
#ifndef _D3DCLASS_H_
#define _D3DCLASS_H_

// INCLUDES

#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>

#include <DirectXMath.h>

using namespace DirectX;

class D3DClass
{
public:
	D3DClass();
	D3DClass(const D3DClass&);
	~D3DClass();

	bool Initialize(int, int, bool, HWND, bool, float, float);
	
	void BeginScene(float, float, float, float);
	void EndScene();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();

	XMMATRIX GetProjectionMatrix();
	XMMATRIX GetWorldMatrix();
	XMMATRIX GetOrthoMatrix();

	void GetVideoCardInfo(char*, int&);

	//for handling back face culling
	void TurnOnCulling();
	void TurnOffCulling();

	//need the z buffers
	void TurnZBufferOn();
	void TurnZBufferOff();

	void SetBackBufferRenderTarget();
	void ResetViewport();

private:
	bool m_vsync_enabled;
	int m_videoCardMemory;
	char m_videoCardDescription[128];
	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;

	//adding a pointer for culling
	ID3D11RasterizerState* m_rasterStateNoCulling;
	ID3D11DepthStencilState* m_depthDisabledStencilState;

	XMMATRIX m_projectionMatrix;
	XMMATRIX m_worldMatrix;
	XMMATRIX m_orthoMatrix;

	ID3D11RenderTargetView* m_renderTargetView;
	D3D11_VIEWPORT m_viewport;
};

#endif