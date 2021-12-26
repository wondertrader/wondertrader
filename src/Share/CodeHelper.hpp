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

USING_NS_WTP;

//主力合约后缀
static const char* SUFFIX_HOT = ".HOT";

//次主力合约后缀
static const char* SUFFIX_2ND = ".2ND";

//前复权合约代码后缀
static const char SUFFIX_QFQ = '-';

//后复权合约代码后缀
static const char SUFFIX_HFQ = '+';

class CodeHelper
{
public:
	typedef struct _CodeInfo
	{
		char _code[MAX_INSTRUMENT_LENGTH];		//合约代码
		char _exchg[MAX_INSTRUMENT_LENGTH];		//交易所代码
		char _product[MAX_INSTRUMENT_LENGTH];	//品种代码

		//By Wesley @ 2021.12.25
		//去掉合约类型，这里不再进行判断
		//整个CodeHelper会重构
		//ContractCategory	_category;		//合约类型
		union
		{
			uint8_t	_hotflag;	//主力标记，0-非主力，1-主力，2-次主力
			uint8_t	_exright;	//是否是复权代码,如SH600000Q: 0-不复权, 1-前复权, 2-后复权
		};

		inline bool isExright() const { return _exright != 0; }
		inline bool isHot() const { return _hotflag ==1; }
		inline bool isSecond() const { return _hotflag == 2; }
		inline bool isFlat() const { return _hotflag == 0; }

		//inline bool isFuture() const { return _category == CC_Future; }
		//inline bool isStock() const { return _category == CC_Stock; }
		//inline bool isFutOpt() const { return _category == CC_FutOption; }
		//inline bool isETFOpt() const { return _category == CC_ETFOption; }
		//inline bool isSpotOpt() const { return _category == CC_SpotOption; }

		//inline bool isOption() const { return _category == CC_SpotOption || _category == CC_FutOption || _category == CC_ETFOption; }

		inline const char* stdCommID() const
		{
			static char buffer[64] = { 0 };
			if (strlen(buffer) == 0)
				sprintf(buffer, "%s.%s", _exchg, _product);

			return buffer;
		}

		_CodeInfo()
		{
			memset(this, 0, sizeof(_CodeInfo));
			//_category = CC_Future;
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
	static inline bool	isStdChnFutOptCode(const char* code)
	{
		using namespace boost::xpressive;
		/* 定义正则表达式 */
		static cregex reg_stk = cregex::compile("^[A-Z]+.[A-z]+\\d{4}.(C|P).\\d+$");	//CFFEX.IO2007.C.4000
		return 	regex_match(code, reg_stk);
	}

	/*
	 *	是否是标准分月期货合约代码
	 *	//CFFEX.IF.2007
	 */
	static inline bool	isStdMonthlyFutCode(const char* code)
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
	 *	从基础分月合约代码提取基础品种代码
	 *	如ag1912 -> ag
	 *	这个只有分月期货品种才有意义
	 *	这个不会有永续合约的代码传到这里来，如果有的话就是调用的地方有Bug!
	 */
	static inline std::string rawMonthCodeToRawCommID(const char* code)
	{
		int nLen = 0;
		while ('A' <= code[nLen] && code[nLen] <= 'z')
			nLen++;

		std::string strRet(code, nLen);
		return std::move(strRet);
	}

	/*
	 *	基础分月合约代码转标准码
	 *	如ag1912转成全码
	 *	这个不会有永续合约的代码传到这里来，如果有的话就是调用的地方有Bug!
	 */
	static inline std::string rawMonthCodeToStdCode(const char* code, const char* exchg, bool isComm = false)
	{
		std::string pid = code;
		if (!isComm)
			pid = rawMonthCodeToRawCommID(code);

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
	 *	原始常规代码转标准代码
	 *	这种主要针对非分月合约而言
	 */
	static inline std::string rawFlatCodeToStdCode(const char* code, const char* exchg, const char* pid)
	{
		return std::move(StrUtil::printf("%s.%s.%s", exchg, pid, code));
	}

	static inline bool isMonthlyCode(const char* code)
	{
		return isdigit(code[strlen(code) - 1]);
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
	static inline CodeInfo extractStdChnFutOptCode(const char* stdCode)
	{
		CodeInfo codeInfo;

		StringVector ay = StrUtil::split(stdCode, ".");
		strcpy(codeInfo._exchg, ay[0].c_str());
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
		//期权的代码规则和其他都不一样，所以单独判断
		if(isStdChnFutOptCode(stdCode))
		{
			return std::move(extractStdChnFutOptCode(stdCode));
		}
		else
		{
			/*
			 *	By Wesley @ 2021.12.25
			 *	1、先看是不是Q和H结尾的，如果是复权标记确认以后，最后一段长度-1，复制到code，如SSE.STK.600000Q
			 *	2、再看是不是分月合约，如果是，则将product字段拼接月份给code（郑商所特殊处理），如CFFEX.IF.2112
			 *	3、最后看看是不是HOT和2ND结尾的，如果是，则将product拷贝给code，如DCE.m.HOT
			 *	4、如果都不是，则原样复制第三段，如BINANCE.DC.BTCUSDT/SSE.STK.600000
			 */
			CodeInfo codeInfo;
			StringVector ay = StrUtil::split(stdCode, ".");
			//codeInfo._category = CC_Stock;
			strcpy(codeInfo._exchg, ay[0].c_str());
			strcpy(codeInfo._product, ay[1].c_str());
			if (ay[2].back() == SUFFIX_QFQ || ay[2].back() == SUFFIX_HFQ)
			{
				strcpy(codeInfo._code, ay[2].substr(0, ay[2].size() - 1).c_str());
				codeInfo._exright = (ay[2].back() == SUFFIX_QFQ)?1:2;
			}
			else if(isdigit(ay[2].back()))
			{
				//如果最后一段是数字，说明是分月合约
				//TODO: 这样的判断存在一个假设，最后一位是数字的一定是期货分月合约，以后可能会有问题，先注释一下
				//那么code得加上品种id
				//郑商所得单独处理一下，这个只能hardcode了
				if(strcmp(codeInfo._product, "CZCE") == 0)
					sprintf(codeInfo._code, "%s%s", codeInfo._product, ay[2].c_str()+1);
				else
					sprintf(codeInfo._code, "%s%s", codeInfo._product, ay[2].c_str());
			}
			else
			{
				codeInfo._hotflag = CodeHelper::isStdFutHotCode(stdCode) ? 1 : (CodeHelper::isStdFut2ndCode(stdCode) ? 2 : 0);
				if(codeInfo._hotflag == 0)
					strcpy(codeInfo._code, ay[2].c_str());
				else
					strcpy(codeInfo._code, codeInfo._product);
			}

			return codeInfo;
		}
	}
};

