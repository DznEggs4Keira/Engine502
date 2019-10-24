
#include "inputclass.h"

InputClass::InputClass()
{
	m_directInput = nullptr;
	m_mouse = nullptr;

	pTimer = nullptr;
	pMovement = nullptr;
}


InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
	// release the mouse.
	m_mouse->Unacquire();
	SAFE_RELEASE(m_mouse)

	// release the main interface to direct input.
	SAFE_RELEASE(m_directInput)

	SAFE_DELETE(pTimer)
	SAFE_DELETE(pMovement)
}

#pragma region Initialize

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	// Store the screen size which will be used for positioning the mouse cursor.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Initialize the location of the mouse on the screen.
	m_mouseX = 0;
	m_mouseY = 0;

	// Initialize the main direct input interface.
	if (FAILED(result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_directInput, NULL)))
	{
		return false;
	}

	// Initialize the direct input interface for the mouse.
	if (FAILED(result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL)))
	{
		return false;
	}

	// Set the data format for the mouse using the pre-defined mouse data format.
	if (FAILED(result = m_mouse->SetDataFormat(&c_dfDIMouse)))
	{
		return false;
	}

	// Set the cooperative level of the mouse to share with other programs.
	if (FAILED(result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		return false;
	}

	// Acquire the mouse.
	if (FAILED(result = m_mouse->Acquire()))
	{
		return false;
	}

	//Initialise the Timer and Movement objects
	pTimer = new FrameTime;
	if (!pTimer)
	{
		return false;
	}

	if (FAILED(result = pTimer->Initialise()))
	{
		MessageBox(hwnd, L"Could not initialise the timer object", L"Error", MB_OK);
		return false;
	}

	pMovement = new Movement;
	if (!pMovement)
	{
		return false;
	}

	pMovement->SetPosition();

	return true;
}

#pragma endregion

#pragma region Getter

XMFLOAT3 InputClass::GetMouseMovement()
{
	return XMFLOAT3(m_mouseState.lX, m_mouseState.lY, m_mouseState.lZ);
}

#pragma endregion

#pragma region Frame

bool InputClass::Frame()
{	
	// Read the current state of the mouse.
	if (!ReadMouse())
	{
		return false;
	}

	// Process the changes in the mouse and keyboard.
	ProcessInput();

	// Update the system stats.
	pTimer->Frame();

	HandleInput();

	return true;
}

void InputClass::HandleInput()
{
	// Set the frame time for calculating the updated position.
	pMovement->SetFrameTime(pTimer->GetFPS());

	//Movement via keyboard being used
	pMovement->MoveForward(IsKeyPressedW());
	pMovement->MoveBackward(IsKeyPressedS());
	pMovement->MoveUpward(IsKeyPressedA());
	pMovement->MoveDownward(IsKeyPressedD());

	// Camera Rotation via Mouse
	XMFLOAT3 Rot, tempRot;

	Rot = GetMouseMovement();
	tempRot = XMFLOAT3(0.0f, 0.0f, 0.0f);

	tempRot.x += Rot.x * 1 / 10;
	tempRot.y += Rot.y * 1 / 10;
	pMovement->RotateCamera(tempRot);
}

bool InputClass::ReadMouse()
{
	HRESULT result;

	// Read the mouse device.
	if (FAILED(result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE),
		(LPVOID)&m_mouseState)))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

void InputClass::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }
}

#pragma endregion

#pragma region KeyPresses

bool InputClass::IsKeyPressedW()
{
	if (GetAsyncKeyState('W') & 0x8000)
	{
		return true;
	}

	return false;
}
bool InputClass::IsKeyPressedA()
{
	if (GetAsyncKeyState('A') & 0x8000)
	{
		return true;
	}

	return false;
}
bool InputClass::IsKeyPressedS()
{
	if (GetAsyncKeyState('S') & 0x8000)
	{
		return true;
	}

	return false;
}
bool InputClass::IsKeyPressedD()
{
	if (GetAsyncKeyState('D') & 0x8000)
	{
		return true;
	}

	return false;
}

#pragma endregion


