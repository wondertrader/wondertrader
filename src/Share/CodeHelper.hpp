/*!
 * \file CodeHelper.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 代码辅助类,封装到一起方便使用
 */
#pragma once
#include "StrUtil.hpp"
#include "../Includes/WTSTypes.h"

#include <boost/xpressive/xpressive_dynamic.hpp>

USING_NS_OTP;

//主力合约后缀
static const char* SUFFIX_HOT = ".HOT";

//次主力合约后缀
static const char* SUFFIX_2ND = ".2ND";

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
			uint8_t	_hotflag;	//主力标记，0-非主力，1-主力，2-次主力
			uint8_t	_exright;	//是否是复权代码,如SH600000Q: 0-不复权, 1-前复权, 2-后复权
		};

		inline bool isExright() const { return _exright != 0; }
		inline bool isHot() const { return _hotflag ==1; }
		inline bool isSecond() const { return _hotflag == 2; }
		inline bool isFlat() const { return _hotflag == 0; }

		inline bool isFuture() const { return _category == CC_Future; }
		inline bool isStock() const { return _category == CC_Stock; }
		inline bool isFutOpt() const { return _category == CC_FutOption; }
		inline bool isETFOpt() const { return _category == CC_ETFOption; }
		inline bool isSpotOpt() const { return _category == CC_SpotOption; }

		inline bool isOption() const { return _category == CC_SpotOption || _category == CC_FutOption || _category == CC_ETFOption; }

		inline const char* pureStdCode() const
		{
			static char buffer[64] = { 0 };
			if (strlen(buffer) == 0)
				sprintf(buffer, "%s.%s.%s", _exchg, _product, _code);

			return buffer;
		}

		_CodeInfo()
		{
			memset(this, 0, sizeof(_CodeInfo));
			_category = CC_Future;
		}
	} CodeInfo;

private:
	static inline std::size_t find(const char* src, char symbol = '.', bool bReverse = false)
	{
		std::size_t len = strlen(src);
		if (len != 0)
		{
			if (bReverse)
			{
				for (std::size_t idx = len - 1; idx >= 0; idx--)
				{
					if (src[idx] == symbol)
						return idx;
				}
			}
			else
			{
				for (std::size_t idx = 0; idx < len; idx++)
				{
					if (src[idx] == symbol)
						return idx;
				}
			}
		}


		return std::string::npos;
	}

public:
	/*
	 *	是否是标准的股票代码
	 *	格式如SSE.STK.600000[Q|H]
	 */
	static inline bool	isStdStkCode(const char* code)
	{
		using namespace boost::xpressive;
		/* 定义正则表达式 */
		static cregex reg_stk = cregex::compile("^[A-Z]+.[A-Z]+.\\d{6,16}(Q?|H)$");
		return 	regex_match(code, reg_stk);
	}

	/*
	 *	是否是标准期货主力合约代码
	 */
	static inline bool	isStdFutHotCode(const char* stdCode)
	{
		return StrUtil::endsWith(stdCode, SUFFIX_HOT, false);
	}

	/*
	 *	是否是标准期货次主力合约代码
	 */
	static inline bool	isStdFut2ndCode(const char* stdCode)
	{
		return StrUtil::endsWith(stdCode, SUFFIX_2ND, false);
	}

	/*
	 *	是否是期货期权合约代码
	 *	CFFEX.IO2007.C.4000
	 */
	static inline bool	isStdFutOptCode(const char* code)
	{
		using namespace boost::xpressive;
		/* 定义正则表达式 */
		static cregex reg_stk = cregex::compile("^[A-Z]+.[A-z]+\\d{4}.(C|P).\\d+$");	//CFFEX.IO2007.C.4000
		return 	regex_match(code, reg_stk);
	}

	/*
	 *	是否是标准期货合约代码
	 *	//CFFEX.IF.2007
	 */
	static inline bool	isStdFutCode(const char* code)
	{
		using namespace boost::xpressive;
		/* 定义正则表达式 */
		static cregex reg_stk = cregex::compile("^[A-Z]+.[A-z]+.\\d{4}$");	//CFFEX.IO.2007
		return 	regex_match(code, reg_stk);
	}

	/*
	 *	标准代码转标准品种ID
	 *	如SHFE.ag.1912->SHFE.ag
	 *	如果是简化的股票代码，如SSE.600000，则转成SSE.STK
	 */
	static inline std::string stdCodeToStdCommID(const char* stdCode)
	{
		auto idx = find(stdCode, '.', true);
		std::string stdCommID(stdCode, idx);
		return std::move(stdCommID);
	}

	/*
	 *	从基础合约代码提取基础品种代码
	 *	如ag1912 -> ag
	 */
	static inline std::string rawFutCodeToRawCommID(const char* code)
	{
		int nLen = 0;
		while ('A' <= code[nLen] && code[nLen] <= 'z')
			nLen++;

		std::string strRet(code, nLen);
		return std::move(strRet);
	}

	/*
	 *	基础合约代码转标准码
	 *	如ag1912转成全码
	 *	-- 暂时没有地方调用 --
	 */
	static inline std::string rawFutCodeToStdCode(const char* code, const char* exchg, bool isComm = false)
	{
		std::string pid = code;
		if (!isComm)
			pid = rawFutCodeToRawCommID(code);

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
		return std::move(ret);
	}

	/*
	 *	原始股票代码转标准代码
	 *	-- 暂时没有地方调用 --
	 */
	static inline std::string rawStkCodeToStdCode(const char* code, const char* exchg, const char* pid)
	{
		return std::move(StrUtil::printf("%s.%s.%s", exchg, pid, code));
	}

	/*
	 *	期货期权代码标准化
	 *	标准期货期权代码格式为CFFEX.IO2008.C.4300
	 *	-- 暂时没有地方调用 --
	 */
	static inline std::string rawFutOptCodeToStdCode(const char* code, const char* exchg)
	{
		using namespace boost::xpressive;
		/* 定义正则表达式 */
		static cregex reg_stk = cregex::compile("^[A-Z|a-z]+\\d{4}-(C|P)-\\d+$");	//中金所、大商所格式IO2013-C-4000
		bool bMatch = regex_match(code, reg_stk);
		if(bMatch)
		{
			std::string s = StrUtil::printf("%s.%s", exchg, code);
			StrUtil::replace(s, "-", ".");
			return std::move(s);
		}
		else
		{
			//郑商所上期所期权代码格式ZC2010P11600

			//先从后往前定位到P或C的位置
			int idx = strlen(code) - 1;
			for(; idx >= 0; idx--)
			{
				if(!isdigit(code[idx]))
					break;
			}
			
			std::string s = exchg;
			s.append(".");
			s.append(code, idx);
			s.append(".");
			s.append(&code[idx], 1);
			s.append(".");
			s.append(&code[idx + 1]);
			return std::move(s);
		}
	}

	/*
	 *	标准合约代码转主力代码
	 */
	static inline std::string stdCodeToStdHotCode(const char* stdCode)
	{
		std::size_t idx = find(stdCode, '.', true);
		if (idx == std::string::npos)
			return "";		
		
		std::string stdWrappedCode;
		stdWrappedCode.resize(idx + strlen(SUFFIX_HOT) + 1);
		strncpy((char*)stdWrappedCode.data(), stdCode, idx);
		strcpy((char*)stdWrappedCode.data()+idx, SUFFIX_HOT);
		return std::move(stdWrappedCode);
	}

	/*
	 *	标准合约代码转次主力代码
	 */
	static inline std::string stdCodeToStd2ndCode(const char* stdCode)
	{
		std::size_t idx = find(stdCode, '.', true);
		if (idx == std::string::npos)
			return "";

		std::string stdWrappedCode;
		stdWrappedCode.resize(idx + strlen(SUFFIX_2ND) + 1);
		strncpy((char*)stdWrappedCode.data(), stdCode, idx);
		strcpy((char*)stdWrappedCode.data() + idx, SUFFIX_2ND);
		return std::move(stdWrappedCode);
	}

	/*
	 *	标准期货期权代码转原代码
	 *	-- 暂时没有地方调用 --
	 */
	static inline std::string stdFutOptCodeToRawCode(const char* stdCode)
	{
		std::string ret = stdCode;
		auto pos = ret.find(".");
		ret = ret.substr(pos + 1);
		if (strncmp(stdCode, "CFFEX", 5) == 0 || strncmp(stdCode, "DCE", 3) == 0)
			StrUtil::replace(ret, ".", "-");
		else
			StrUtil::replace(ret, ".", "");
		return std::move(ret);
	}

	/*
	 *	标准期货代码转原始代码
	 *	-- 暂时没有地方调用 --
	 */
	static inline std::string stdFutCodeToRawCode(const char* stdCode)
	{
		StringVector ay = StrUtil::split(stdCode, ".");
		std::string exchg = ay[0];
		std::string rawCode = ay[1];
		if (exchg.compare("CZCE") == 0 && ay[2].size() == 4)
			rawCode += ay[2].substr(1);
		else
			rawCode += ay[2];
		return std::move(rawCode);
	}

	/*
	 *	标准股票代码转原始代码
	 *	-- 暂时没有地方调用 --
	 */
	static inline std::string stdStkCodeToRawCode(const char* stdCode)
	{
		auto idx = find(stdCode, '.', true);
		return std::move(stdCode + idx + 1);
	}

	/*
	 *	标准代码转成原始代码
	 *	-- 暂时没有地方调用 -- 
	 */
	static inline std::string stdCodeToRawCode(const char* stdCode)
	{
		if (isStdStkCode(stdCode))
			return stdStkCodeToRawCode(stdCode);
		else if (isStdFutOptCode(stdCode))
			return stdFutOptCodeToRawCode(stdCode);
		else
			return stdFutCodeToRawCode(stdCode);
	}

	/*
	 *	提取标准期货合约代码的信息
	 */
	static inline CodeInfo extractStdFutCode(const char* stdCode)
	{
		CodeInfo codeInfo;
		codeInfo._hotflag = CodeHelper::isStdFutHotCode(stdCode) ? 1 : (CodeHelper::isStdFut2ndCode(stdCode) ? 2 : 0);
		StringVector ay = StrUtil::split(stdCode, ".");
		strcpy(codeInfo._exchg, ay[0].c_str());
		strcpy(codeInfo._code, ay[1].c_str());
		codeInfo._category = CC_Future;
		if (codeInfo.isFlat())
		{
			if (strcmp(codeInfo._exchg, "CZCE") == 0 && ay[2].size() == 4)
			{
				//rawCode += ay[2].substr(1);
				strcat(codeInfo._code + strlen(codeInfo._code), ay[2].substr(1).c_str());
			}
			else
			{
				//rawCode += ay[2];
				strcat(codeInfo._code + strlen(codeInfo._code), ay[2].c_str());
			}
		}
		//commID = ay[1];
		strcpy(codeInfo._product, ay[1].c_str());
		return std::move(codeInfo);
	}

	/*
	 *	提取标准股票代码的信息
	 */
	static inline CodeInfo extractStdStkCode(const char* stdCode)
	{
		CodeInfo codeInfo;

		StringVector ay = StrUtil::split(stdCode, ".");
		codeInfo._category = CC_Stock;
		strcpy(codeInfo._exchg, ay[0].c_str());
		//By Wesley @ 2021.12.22
		//不再兼容简写的股票代码，如SSE.600000
		//if (ay.size() > 2)
		{
			//commID = ay[1];
			strcpy(codeInfo._product, ay[1].c_str());
			//rawCode = ay[2];
			if (ay[2].back() == 'Q')
			{
				strcpy(codeInfo._code, ay[2].substr(0, ay[2].size() - 1).c_str());
				codeInfo._exright = 1;
			}
			else if (ay[2].back() == 'H')
			{
				strcpy(codeInfo._code, ay[2].substr(0, ay[2].size() - 1).c_str());
				codeInfo._exright = 2;
			}
			else
			{
				strcpy(codeInfo._code, ay[2].c_str());
				codeInfo._exright = 0;
			}
		}

		return std::move(codeInfo);
	}

	static inline int indexCodeMonth(const char* code)
	{
		if (strlen(code) == 0)
			return -1;

		int idx = 0;
		int len = strlen(code);
		while(idx < len)
		{
			if (isdigit(code[idx]))
				return idx;

			idx++;
		}
		return -1;
	}

	/*
	 *	提取标准期货期权代码的信息
	 */
	static inline CodeInfo extractStdFutOptCode(const char* stdCode)
	{
		CodeInfo codeInfo;

		StringVector ay = StrUtil::split(stdCode, ".");
		strcpy(codeInfo._exchg, ay[0].c_str());
		codeInfo._category = CC_FutOption;
		if(strcmp(codeInfo._exchg, "SHFE") == 0 || strcmp(codeInfo._exchg, "CZCE") == 0)
		{
			sprintf(codeInfo._code, "%s%s%s", ay[1].c_str(), ay[2].c_str(), ay[3].c_str());
		}
		else
		{
			sprintf(codeInfo._code, "%s-%s-%s", ay[1].c_str(), ay[2].c_str(), ay[3].c_str());
		}

		int mpos = indexCodeMonth(ay[1].c_str());

		if(strcmp(codeInfo._exchg, "CZCE") == 0)
		{
			strncpy(codeInfo._product, ay[1].c_str(), mpos);
			strcat(codeInfo._product, ay[2].c_str());
		}
		else if (strcmp(codeInfo._exchg, "CFFEX") == 0)
		{
			strncpy(codeInfo._product, ay[1].c_str(), mpos);
		}
		else
		{
			strncpy(codeInfo._product, ay[1].c_str(), mpos);
			strcat(codeInfo._product, "_o");
		}

		return std::move(codeInfo);
	}

	/*
	 *	提起标准代码的信息
	 */
	static CodeInfo extractStdCode(const char* stdCode)
	{
		if (isStdStkCode(stdCode))
		{
			return std::move(extractStdStkCode(stdCode));
		}
		else if(isStdFutOptCode(stdCode))
		{
			return std::move(extractStdFutOptCode(stdCode));
		}
		else
		{
			return std::move(extractStdFutCode(stdCode));
		}
	}
};

