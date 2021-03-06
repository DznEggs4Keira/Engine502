#pragma once

#define SAFE_SHUTDOWN(p)			if (p) { p->Shutdown(); delete p; p = nullptr; }
#define SAFE_RELEASE(p)			if (p) { p->Release(); p = nullptr; }
#define SAFE_DELETE(p)			if (p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p)	if (p) { delete[] p; p = nullptr; }
#define SAFE_DELETE_VECTOR(p, o) for each(p in o) { delete p; p = nullptr; }
