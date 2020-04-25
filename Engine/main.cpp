
#include "systemclass.h"


int WINAPI WinMain(HINSTANCE hInstance, 
					HINSTANCE hPrevInstance,
					PSTR pScmdline, int iCmdshow)
{

	// Create the system object.
	SystemClass* System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	// Initialize and run the system object.
	if (System->Initialize())
	{
		System->Run();
	}

	// Shutdown and release the system object.
	delete System;
	System = nullptr;

	return 0;
}