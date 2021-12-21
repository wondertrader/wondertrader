/*!
 * \file IniFile.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Ini文件辅助工具，仅适用于windows平台
 */
#pragma once
#include <string>
#include <vector>
#include <map>

typedef std::vector<std::string> StringVector;
typedef std::map<std::string, std::string>	StringMap;

#define MAX_BUFFER_SIZE  2048

#ifndef BOOL
#define BOOL int
#endif

class IniFile
{
public:
	static void			CleareSection(const char* lpszSec, const char* lpszFile)
	{
		WritePrivateProfileSectionA(lpszSec, NULL, lpszFile);
	}

	static BOOL			RemoveKey(const char* lpszSec, const char* lpszKey, const char* lpszFile)
	{
		if (lpszFile == NULL || lpszKey == NULL || lpszSec == NULL)
		{
			return FALSE;
		}

		return WritePrivateProfileStringA(lpszSec, lpszKey, NULL, lpszFile);
	}

	static std::string	ReadConfigString(const char* lpszSec, const char* lpszKey, const char* lpszDefault, const char* lpszFile)
	{
		std::string strRet = lpszDefault;
		if(lpszFile == NULL || lpszKey == NULL || lpszSec == NULL)
		{
			return strRet;
		}

		char buffer[MAX_BUFFER_SIZE];
		memset(buffer, 0, sizeof(char)*MAX_BUFFER_SIZE);
		GetPrivateProfileStringA(lpszSec, lpszKey, lpszDefault, buffer, MAX_BUFFER_SIZE-1, lpszFile);
		strRet = buffer;

		return strRet;
	}

	static int			ReadConfigInt(const char* lpszSec, const char* lpszKey, int nDefault, const char* lpszFile)
	{
		int nRet = nDefault;
		if(lpszFile == NULL || lpszKey == NULL || lpszSec == NULL)
		{
			return nRet;
		}

		nRet = GetPrivateProfileIntA(lpszSec, lpszKey, nDefault, lpszFile);

		return nRet;
	}

	static uint32_t		ReadConfigUInt(const char* lpszSec, const char* lpszKey, uint32_t nDefault, const char* lpszFile)
	{
		uint32_t nRet = nDefault;
		if (lpszFile == NULL || lpszKey == NULL || lpszSec == NULL)
		{
			return nRet;
		}

		nRet = (uint32_t)GetPrivateProfileIntA(lpszSec, lpszKey, nDefault, lpszFile);

		return nRet;
	}

	static int			ReadConfigSectionArray(StringVector &aySection, const char* lpszFile)
	{
		if(lpszFile == NULL)
			return 0;

		aySection.clear();

		char buffer[1024] = {0};
		::GetPrivateProfileSectionNamesA(buffer, 1024, lpszFile);

		char *p = buffer;
		int nCount = 0;
		while(strlen(p) > 0)
		{
			std::string strSection = p;
			aySection.push_back(strSection);
			nCount++;

			p += strlen(p)+1;
		}

		return nCount;
	}

	static std::string		ReadConfigSection(const char* lpszSec, const char* lpszFile)
	{
		if (lpszFile == NULL)
			return 0;

		char buffer[10240] = { 0 };
		::GetPrivateProfileSectionA(lpszSec, buffer, 10240, lpszFile);

		return buffer;
	}

	static int		ReadConfigSectionContent(StringMap& mapContent, const char* lpszSec, const char* lpszFile)
	{
		if (lpszFile == NULL)
			return 0;

		mapContent.clear();

		char buffer[10240] = { 0 };
		::GetPrivateProfileSectionA(lpszSec, buffer, 10240, lpszFile);
		char *p = buffer;
		int nCount = 0;
		while (strlen(p) > 0)
		{
			std::string strLine = p;

			int nPos = strLine.find("=");

			std::string strkey = strLine.substr(0, nPos);
			std::string strValue = strLine.substr(nPos + 1);
			mapContent[strkey] = strValue;

			nCount++;

			p += strlen(p) + 1;
		}

		return nCount;
	}

	static int			ReadConfigSectionKeyArray(StringVector &ayKey, const char* lpszSec, const char* lpszFile)
	{
		if(lpszFile == NULL)
			return 0;

		ayKey.clear();

		char buffer[10240] = {0};
		::GetPrivateProfileSectionA(lpszSec, buffer, 10240, lpszFile);
		char *p = buffer;
		int nCount = 0;
		while(strlen(p) > 0)
		{
			std::string strLine = p;

			int nPos = strLine.find("=");

			std::string strkey = strLine.substr(0,nPos);

			ayKey.push_back(strkey);

			nCount++;

			p += strlen(p)+1;
		}

		return nCount;
	}

	static int			ReadConfigSectionKeyValueArray(StringVector &ayKey, StringVector &ayValue, const char* lpszSec, const char* lpszFile)
	{
		if(lpszFile == NULL)
			return 0;

		ayKey.clear();
		ayValue.clear();

		char buffer[10240] = {0};
		::GetPrivateProfileSectionA(lpszSec, buffer, 10240, lpszFile);
		char *p = buffer;
		int nCount = 0;
		while(strlen(p) > 0)
		{
			std::string strLine = p;

			int nPos = strLine.find("=");

			std::string strkey = strLine.substr(0, nPos);
			std::string strValue = strLine.substr(nPos + 1);

			ayKey.push_back(strkey);
			ayValue.push_back(strValue);

			nCount++;

			p += strlen(p)+1;
		}

		return nCount;
	}

	static BOOL			WriteConfigString(const char* lpszSec, const char* lpszKey, const char* lpszValue, const char* lpszFile)
	{
		if(lpszFile == NULL || lpszKey == NULL || lpszSec == NULL)
		{
			return FALSE;
		}

		return WritePrivateProfileStringA(lpszSec, lpszKey, lpszValue, lpszFile);
	}

	static BOOL			WriteConfigInt(const char* lpszSec, const char* lpszKey, int nValue, const char* lpszFile)
	{
		if(lpszFile == NULL || lpszKey == NULL || lpszSec == NULL)
		{
			return FALSE;
		}

		char strValue[32] = {0};
		sprintf(strValue, "%d", nValue);

		return WritePrivateProfileStringA(lpszSec, lpszKey, strValue, lpszFile);
	}

	static BOOL			WriteConfigUInt(const char* lpszSec, const char* lpszKey, uint32_t nValue, const char* lpszFile)
	{
		if (lpszFile == NULL || lpszKey == NULL || lpszSec == NULL)
		{
			return FALSE;
		}

		char strValue[32] = { 0 };
		sprintf(strValue, "%u", nValue);

		return WritePrivateProfileStringA(lpszSec, lpszKey, strValue, lpszFile);
	}
};