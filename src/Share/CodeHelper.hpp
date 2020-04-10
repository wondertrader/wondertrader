/*!
 * \file CodeHelper.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 代码辅助类，封装到一起方便使用
 */
#pragma once
#include "StrUtil.hpp"
#include "WTSTypes.h"

#include <boost/xpressive/xpressive_dynamic.hpp>

USING_NS_OTP;

class CodeHelper
{
public:
	typedef struct _CodeInfo
	{
		char _code[MAX_INSTRUMENT_LENGTH];		//合约代码
		char _exchg[MAX_INSTRUMENT_LENGTH];		//交易所代码
		char _product[MAX_INSTRUMENT_LENGTH];	//品种代码

		ContractCategory	_category;		//合约类型
		union
		{
			bool	_hot;		//是否是主力合约
			bool	_exright;	//是否是复权代码，如SH600000Q
		};

		_CodeInfo()
		{
			memset(this, 0, sizeof(_CodeInfo));
			_category = CC_Future;
		}
	} CodeInfo;

public:
	static bool	isStdStkCode(const char* code)
	{
		using namespace boost::xpressive;
		/* 定义正则表达式 */
		cregex reg_stk = cregex::compile("^[A-Z]+.([A-Z]+.)?\\d{6}Q?$");
		return 	regex_match(code, reg_stk);
	}

	static std::string stdCodeToStdCommID(const char* stdCode)
	{
		if (isStdStkCode(stdCode))
			return stdStkCodeToStdCommID(stdCode);
		else
			return stdFutCodeToStdCommID(stdCode);
	}

	static std::string stdStkCodeToStdCommID(const char* stdCode)
	{
		//如果是SSE.600000格式的，默认为STK品种
		//如果是SSE.STK.600000格式的，就解析品种出来
		StringVector ay = StrUtil::split(stdCode, ".");
		std::string str = ay[0];
		str += ".";
		if (ay.size() == 2)
			str += "STK";
		else
			str += ay[1];
		return str;
	}

	/*
	 *	从基础合约代码提取基础品种代码
	 *	如ag1912 -> ag
	 */
	static std::string bscFutCodeToBscCommID(const char* code)
	{
		std::string strRet;
		int nLen = 0;
		while ('A' <= code[nLen] && code[nLen] <= 'z')
		{
			strRet += code[nLen];
			nLen++;
		}

		return strRet;
	}

	/*
	 *	标准合约代码转标准品种代码
	 *	如SHFE.ag.1912 -> SHFE.ag
	 */
	static std::string stdFutCodeToStdCommID(const char* stdCode)
	{
		StringVector ay = StrUtil::split(stdCode, ".");
		std::string str = ay[0];
		str += ".";
		str += ay[1];
		return str;
	}

	/*
	 *	基础合约代码转标准码
	 *	如ag1912转成全码
	 */
	static std::string bscFutCodeToStdCode(const char* code, const char* exchg, bool isComm = false)
	{
		std::string pid = code;
		if (!isComm)
			pid = bscFutCodeToBscCommID(code);

		std::string ret = StrUtil::printf("%s.%s", exchg, pid.c_str());
		if (!isComm)
		{
			ret += ".";

			char* s = (char*)code;
			s += pid.size();
			if(strlen(s) == 4)
			{
				ret += s;
			}
			else
			{
				if (s[0] == '9')
					ret += "1";
				else
					ret += "2";

				ret += s;
			}
		}
		return ret;
	}

	static std::string bscStkCodeToStdCode(const char* code, const char* exchg)
	{
		return StrUtil::printf("%s.%s", exchg, code);
	}

	/*
	 *	通过品种代码获取主力合约代码
	 */
	static std::string bscCodeToStdHotCode(const char* code, const char* exchg, bool isComm = false)
	{
		std::string pid = code;
		if (!isComm)
			pid = bscFutCodeToBscCommID(code);

		std::string ret = StrUtil::printf("%s.%s.HOT", exchg, pid.c_str());
		return ret;
	}

	static std::string stdCodeToStdHotCode(const char* stdCode)
	{
		StringVector ay = StrUtil::split(stdCode, ".");
		std::string stdHotCode = ay[0];
		stdHotCode += ".";
		stdHotCode += ay[1];
		stdHotCode += ".HOT";
		return stdHotCode;
	}


	static std::string stdFutCodeToBscCode(const char* stdCode)
	{
		StringVector ay = StrUtil::split(stdCode, ".");
		std::string exchg = ay[0];
		std::string bscCode = ay[1];
		if (exchg.compare("CZCE") == 0 && ay[2].size() == 4)
			bscCode += ay[2].substr(1);
		else
			bscCode += ay[2];
		return bscCode;
	}

	static std::string stdStkCodeToBscCode(const char* stdCode)
	{
		StringVector ay = StrUtil::split(stdCode, ".");
		std::string exchg = ay[0];
		std::string bscCode;
		if (exchg.compare("SSE") == 0)
			bscCode = "SH";
		else
			bscCode = "SZ";

		if (ay.size() == 2)
			bscCode += ay[1];
		else
			bscCode += ay[2];
		return bscCode;
	}

	static std::string stdCodeToBscCode(const char* stdCode)
	{
		if (isStdStkCode(stdCode))
			return stdStkCodeToBscCode(stdCode);
		else
			return stdFutCodeToBscCode(stdCode);
	}

	//static void extractStdFutCode(const char* stdCode, std::string& exchg, std::string& bscCode, std::string& commID, bool& isHot)
	//{
	//	isHot = StrUtil::endsWith(stdCode, ".HOT", false);
	//	StringVector ay = StrUtil::split(stdCode, ".");
	//	exchg = ay[0];
	//	bscCode = ay[1];
	//	if (!isHot)
	//	{
	//		if (exchg.compare("CZCE") == 0 && ay[2].size() == 4)
	//			bscCode += ay[2].substr(1);
	//		else
	//			bscCode += ay[2];
	//	}
	//	commID = ay[1];
	//}

	static void extractStdFutCode(const char* stdCode, CodeInfo& codeInfo)
	{
		codeInfo._hot = StrUtil::endsWith(stdCode, ".HOT", false);
		StringVector ay = StrUtil::split(stdCode, ".");
		strcpy(codeInfo._exchg, ay[0].c_str());
		strcpy(codeInfo._code, ay[1].c_str());
		codeInfo._category = CC_Future;
		if (!codeInfo._hot)
		{
			if (strcmp(codeInfo._exchg, "CZCE") == 0 && ay[2].size() == 4)
			{
				//bscCode += ay[2].substr(1);
				strcat(codeInfo._code + strlen(codeInfo._code), ay[2].substr(1).c_str());
			}
			else
			{
				//bscCode += ay[2];
				strcat(codeInfo._code + strlen(codeInfo._code), ay[2].c_str());
			}
		}
		//commID = ay[1];
		strcpy(codeInfo._product, ay[1].c_str());
	}

	//static void extractStdStkCode(const char* stdCode, std::string& exchg, std::string& bscCode, std::string& commID)
	//{
	//	StringVector ay = StrUtil::split(stdCode, ".");
	//	exchg = ay[0];
	//	if(ay.size() > 2)
	//	{
	//		commID = ay[1];
	//		bscCode = ay[2];
	//	}
	//	else
	//	{
	//		commID = "STK";
	//		bscCode = ay[1];
	//	}
	//}

	static void extractStdStkCode(const char* stdCode, CodeInfo& codeInfo)
	{
		StringVector ay = StrUtil::split(stdCode, ".");
		codeInfo._category = CC_Stock;
		//exchg = ay[0];
		strcpy(codeInfo._exchg, ay[0].c_str());
		if (ay.size() > 2)
		{
			//commID = ay[1];
			strcpy(codeInfo._product, ay[1].c_str());
			//bscCode = ay[2];
			if (ay[2].back() == 'Q')
			{
				strcpy(codeInfo._code, ay[2].substr(0, ay[2].size() - 1).c_str());
				codeInfo._exright = true;
			}
			else
			{
				strcpy(codeInfo._code, ay[2].c_str());
				codeInfo._exright = false;
			}
		}
		else
		{
			//commID = "STK";
			strcpy(codeInfo._product, "STK");
			//bscCode = ay[1];
			if (ay[1].back() == 'Q')
			{
				strcpy(codeInfo._code, ay[1].substr(0, ay[1].size() - 1).c_str());
				codeInfo._exright = true;
			}
			else
			{
				strcpy(codeInfo._code, ay[1].c_str());
				codeInfo._exright = false;
			}
		}
	}

	static void extractStdCode(const char* stdCode, std::string& exchg, std::string& bscCode, std::string& commID, bool& isHot)
	{
		CodeInfo codeInfo;
		if(isStdStkCode(stdCode))
		{
			extractStdStkCode(stdCode, codeInfo);
			isHot = false;
		}
		else
		{
			extractStdFutCode(stdCode, codeInfo);
		}

		exchg = codeInfo._exchg;
		bscCode = codeInfo._code;
		commID = codeInfo._product;
		isHot = codeInfo._hot;
	}

	static void extractStdCode(const char* stdCode, CodeInfo& codeInfo)
	{
		if (isStdStkCode(stdCode))
		{
			extractStdStkCode(stdCode, codeInfo);
		}
		else
		{
			extractStdFutCode(stdCode, codeInfo);
		}
	}

	static bool	isStdFutHotCode(const char* stdCode)
	{
		return StrUtil::endsWith(stdCode, ".HOT", false);
	}
};

