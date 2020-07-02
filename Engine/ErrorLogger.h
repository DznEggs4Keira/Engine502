#pragma once

#include <Windows.h>

#include "StringConverter.h"

class ErrorLogger
{
public:
	static void Log(std::string);
	static void Log(HRESULT, std::string);
};

