
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

// PRE-PROCESSING DIRECTIVES 
#define DIRECTINPUT_VERSION 0x0800

// INCLUDES 
#include "d3dclass.h"
#include <dinput.h>

#include <Windows.h>
#include "Utils.h"

// My Includes
#include "FrameTime.h"
#include "Movement.h"


/* The input class is called in graphics AND system class
 * this is so because:
 * Keyboard: uses the GetAsyncState() function to return what key was pressed
 * and then it is processed in the graphics class.
 * Mouse: uses Raw input which is checked in the windows message handler 
 * which is present in the system class, therefore this will be called there too*/

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	bool Frame();

	XMFLOAT3 GetRot() { return pMovement->GetRotation(); }
	XMFLOAT3 GetPos() { return pMovement->GetPosition(); }

private:

	bool IsEscapePressed();

	//keyboard movement with W A S D
	bool IsKeyPressedW();
	bool IsKeyPressedA();
	bool IsKeyPressedS();
	bool IsKeyPressedD();

	XMFLOAT3 GetMouseMovement();

	bool ReadMouse();
	void ProcessInput();

	bool HandleInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_mouse;

	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;

	FrameTime* pTimer;
	Movement* pMovement;
};

#endif