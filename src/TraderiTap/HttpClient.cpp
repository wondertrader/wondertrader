#include "httpclient.h"

#include "../Share/StrUtil.hpp"

#define CURL_STATICLIB
#include <curl/curl.h>
#include <string>
#include <stdint.h>

#ifdef _DEBUG
#pragma comment(lib, "libcurl-mdd.lib")
#else
#pragma comment(lib, "libcurl-md.lib")
#endif
#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wldap32.lib")

CHttpClient::CHttpClient(void) : 
m_bDebug(false)
{

}

CHttpClient::~CHttpClient(void)
{

}

static int OnDebug(CURL *, curl_infotype itype, char * pData, size_t size, void *)
{
	if(itype == CURLINFO_TEXT)
	{
		//printf("[TEXT]%s\n", pData);
	}
	else if(itype == CURLINFO_HEADER_IN)
	{
		printf("[HEADER_IN]%s\n", pData);
	}
	else if(itype == CURLINFO_HEADER_OUT)
	{
		printf("[HEADER_OUT]%s\n", pData);
	}
	else if(itype == CURLINFO_DATA_IN)
	{
		printf("[DATA_IN]%s\n", pData);
	}
	else if(itype == CURLINFO_DATA_OUT)
	{
		printf("[DATA_OUT]%s\n", pData);
	}
	return 0;
}

static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
	std::string* str = dynamic_cast<std::string*>((std::string *)lpVoid);
	if( NULL == str || NULL == buffer )
	{
		return -1;
	}

    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);
	return nmemb;
}

int CHttpClient::Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}
	if(m_bDebug)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}
	std::string strHeader;
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)&strHeader);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 120);
	//curl_easy_setopt(curl, CURLOPT_ENCODING, "gzip");
	res = curl_easy_perform(curl);

	/*
	if(res == CURLE_OK)
	{
		bool isGZipped = false;
		StrUtil::trim(strHeader, "\r\n");
		StrUtil::toLowerCase(strHeader);
		const StringVector& ayHeaders = StrUtil::split(strHeader, "\r\n");
		StringVector::const_iterator cit = ayHeaders.begin();
		for(; cit != ayHeaders.end(); cit++)
		{
			const std::string& item = *cit;
			if(StrUtil::startsWith(item, "content-encoding", true) && item.find("gzip")!=std::string::npos)
			{
				isGZipped = true;
			}
		}
		if(isGZipped && strResponse.size()>0)
		{
			uint32_t oldSize = strResponse.size();
			CGZIP2A g2a((LPGZIP)strResponse.data(), strResponse.size());	
			strResponse = g2a.psz;		

			StatHelper::append(oldSize, g2a.Length);
		}
		else
		{
			StatHelper::append(strResponse.size(), strResponse.size());
		}
	}
	*/
	curl_easy_cleanup(curl);

	return res;
}

int CHttpClient::Get(const std::string & strUrl, std::string & strResponse)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}
	if(m_bDebug)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}

	std::string strHeader;
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void *)&strHeader);
	/**
	* 当多个线程都使用超时处理的时候,同时主线程中有sleep或是wait等操作。
	* 如果不设置这个选项,libcurl将会发信号打断这个wait从而导致程序退出。
	*/
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 30);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30);
	res = curl_easy_perform(curl);

	/*
	if(res == CURLE_OK)
	{
		bool isGZipped = false;
		StrUtil::trim(strHeader, "\r\n");
		StrUtil::toLowerCase(strHeader);
		const StringVector& ayHeaders = StrUtil::split(strHeader, "\r\n");
		StringVector::const_iterator cit = ayHeaders.begin();
		for(; cit != ayHeaders.end(); cit++)
		{
			const std::string& item = *cit;
			if(StrUtil::startsWith(item, "Content-Encoding", true) && item.find("gzip")!=std::string::npos)
			{
				isGZipped = true;
			}
		}
		if(isGZipped)
		{
			uint32_t oldSize = strResponse.size();
			CGZIP2A g2a((LPGZIP)strResponse.data(), strResponse.size());	
			strResponse = g2a.psz;

			StatHelper::append(oldSize, g2a.Length);
		}
		else
		{
			StatHelper::append(strResponse.size(), strResponse.size());
		}
	}
	*/
	curl_easy_cleanup(curl);
	return res;
}

int CHttpClient::Posts(const std::string & strUrl, const std::string & strPost, std::string & strResponse, const char * pCaPath)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}
	if(m_bDebug)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strPost.c_str());
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	if(NULL == pCaPath)
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
	}
	else
	{
		//缺省情况就是PEM,所以无需设置,另外支持DER
		//curl_easy_setopt(curl,CURLOPT_SSLCERTTYPE,"PEM");
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
	}
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return res;
}

int CHttpClient::Gets(const std::string & strUrl, std::string & strResponse, const char * pCaPath)
{
	CURLcode res;
	CURL* curl = curl_easy_init();
	if(NULL == curl)
	{
		return CURLE_FAILED_INIT;
	}
	if(m_bDebug)
	{
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, OnDebug);
	}
	curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
	if(NULL == pCaPath)
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
	}
	else
	{
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, true);
		curl_easy_setopt(curl, CURLOPT_CAINFO, pCaPath);
	}
	curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3);
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
	res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	return res;
}

///////////////////////////////////////////////////////////////////////////////////////////////

void CHttpClient::SetDebug(bool bDebug)
{
	m_bDebug = bDebug;
}


const char* CHttpClient::GetErrorMessage(int ecode)
{
	switch(ecode)
	{
	case CURLE_OK:
		return "none";
		break;
	case CURLE_UNSUPPORTED_PROTOCOL:
		return "unsupported protocol";
		break;
	case CURLE_COULDNT_CONNECT:
		return "could not connect";
		break;
	case CURLE_OPERATION_TIMEDOUT:
		return "operation timeout";
		break;
	default:
		return "unkown error";
		break;
	}
}