#pragma once
#ifdef _WIN32
#include <windows.h>
#include <Psapi.h>
#else
#include <unistd.h>
#endif

#include "fmtlib.h"

class ProcHelper
{
public:
	static bool isProcAlive(uint32_t pid)
	{
#ifdef _WIN32
		DWORD aProcesses[2048], cbNeeded, cProcesses;
		unsigned int i;
		if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
			return false;

		cProcesses = cbNeeded / sizeof(DWORD);
		for(i = 0; i < cProcesses; i++)
		{
			if (aProcesses[i] == pid)
				return true;
		}

		return false;
#else
		//linux下在只需要检查/proc/[PID]是否存在即可
		const char* path = fmtutil::format("/proc/{}", pid);
		return access(path, 0) == 0;
#endif
	}
};