
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

// PRE-PROCESSING DIRECTIVES 
#define WIN32_LEAN_AND_MEAN

// INCLUDES 
#include <windows.h>
#include <windowsx.h>

// MY CLASS INCLUDES
#include "graphicsclass.h"
#include "Sound.h"
#include "inputclass.h"
#include "Utils.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	GraphicsClass* m_Graphics;
	Sound* m_Sound;
	InputClass* m_Input;
};


// FUNCTION PROTOTYPES
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// GLOBALS
static SystemClass* ApplicationHandle = 0;


#endif