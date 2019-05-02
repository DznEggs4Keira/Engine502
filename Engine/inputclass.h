
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

// PRE-PROCESSING DIRECTIVES 
#define DIRECTINPUT_VERSION 0x0800

// LINKING 
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// INCLUDES 
#include "d3dclass.h"
#include <dinput.h>

class InputClass
{
public:
	InputClass();
	InputClass(const InputClass&);
	~InputClass();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

	bool IsEscapePressed();

	//keyboard movement with W A S D
	bool IsKeyPressedW();
	bool IsKeyPressedA();
	bool IsKeyPressedS();
	bool IsKeyPressedD();

	//to look up and down
	bool IsUpPressed();
	bool IsDownPressed();

	//to move up and down
	bool IsLeftPressed();
	bool IsRightPressed();

	void GetMouseLocation(int&, int&);

private:
	bool ReadKeyboard();
	bool ReadMouse();
	void ProcessInput();

private:
	IDirectInput8* m_directInput;
	IDirectInputDevice8* m_keyboard;
	IDirectInputDevice8* m_mouse;

	unsigned char m_keyboardState[256];
	DIMOUSESTATE m_mouseState;

	int m_screenWidth, m_screenHeight;
	int m_mouseX, m_mouseY;
};

#endif