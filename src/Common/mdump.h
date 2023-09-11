/*
* $Id: mdump.h 5561 2009-12-25 07:23:59Z wangmeng $
*
* this file is part of easyMule
* Copyright (C)2002-2008 VeryCD Dev Team ( strEmail.Format("%s@%s", "emuledev", "verycd.com") / http: * www.easymule.org )
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#pragma once
#include <Windows.h>
struct _EXCEPTION_POINTERS;

class CMiniDumper
{
public:
	static void Enable(LPCTSTR pszAppName, bool bShowErrors, LPCTSTR pszDumpPath = "");

private:
	static TCHAR m_szAppName[MAX_PATH];
	static TCHAR m_szDumpPath[MAX_PATH];

	static HMODULE GetDebugHelperDll(FARPROC* ppfnMiniDumpWriteDump, bool bShowErrors);
	static LONG WINAPI TopLevelFilter(struct _EXCEPTION_POINTERS* pExceptionInfo);
};

extern CMiniDumper theCrashDumper;
