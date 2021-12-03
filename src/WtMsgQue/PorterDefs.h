#pragma once

#ifdef _WIN32
#	define PORTER_FLAG _cdecl
#else
#	define PORTER_FLAG __attribute__((_cdecl))
#endif

typedef unsigned long		WtUInt32;
typedef unsigned long long	WtUInt64;
typedef const char*			WtString;

typedef void(PORTER_FLAG *FuncMQCallback)(WtUInt32 id, const char* topic, const char* data, WtUInt32 dataLen);
typedef void(PORTER_FLAG *FuncLogCallback)(WtUInt32 id, const char* message, bool bServer);

#ifndef EXPORT_FLAG
#ifdef _MSC_VER
#	define EXPORT_FLAG __declspec(dllexport)
#else
#	define EXPORT_FLAG __attribute__((__visibility__("default")))
#endif
#endif
