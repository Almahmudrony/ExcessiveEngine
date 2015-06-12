
#include "../Common/src/Sys.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <xlocbuf>
#include <codecvt>

Sys::DLLHandle Sys::LoadDLL(const wchar_t* path) 
{
	return LoadLibraryW(path);
}

bool Sys::unLoadDLL(DLLHandle h) 
{
	return FreeLibrary((HMODULE)h) ? true : false;
}

void* Sys::GetDLLProcAddress(DLLHandle h, const std::string& procName) 
{
	return (void*)GetProcAddress((HMODULE)h, procName.c_str());
}

std::wstring Sys::GetWorkDir() 
{
	// TODO make define or constant somewhere (128)
	wchar_t path[128];
	GetModuleFileNameW(0, path, 128);

	// Preserve only directory
	for (int i = 127; i > 0; i--)
		if (path[i] == '\\' && i < 127)
		{
			path[i + 1] = 0;
			break;
		}

	// TODO: lass�t, de legal�bb sz�p replace '\\' with '/'
	int idx = 0;
	while (path[idx] != '\0') {
		if (path[idx] == '\\')
			path[idx] = '/';
		idx++;
	}

	return path;
}

mm::ivec2 Sys::GetMousePos() 
{
	POINT p; GetCursorPos(&p);
	return mm::ivec2(p.x, p.y);
}

mm::ivec2 Sys::GetScreenSize()
{
	return{ GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
}

void Sys::messageBox(const std::wstring& msg)
{
	MessageBoxW(0, msg.c_str(), L"", MB_OK);
}