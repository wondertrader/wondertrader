/*!
 * \file CodeHelper.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief ���븨����,��װ��һ�𷽱�ʹ��
 */
#pragma once
#include "fmtlib.h"
#include "StrUtil.hpp"
#include "../Includes/WTSTypes.h"
#include "../Includes/IHotMgr.h"

#include <boost/xpressive/xpressive_dynamic.hpp>


USING_NS_WTP;

//������Լ��׺
static const char* SUFFIX_HOT = ".HOT";

//��������Լ��׺
static const char* SUFFIX_2ND = ".2ND";

//ǰ��Ȩ��Լ�����׺
static const char SUFFIX_QFQ = '-';

//��Ȩ��Լ�����׺
static const char SUFFIX_HFQ = '+';

class CodeHelper
{
public:
	typedef struct _CodeInfo
	{
		char _code[MAX_INSTRUMENT_LENGTH];		//��Լ����
		char _exchg[MAX_INSTRUMENT_LENGTH];		//����������
		char _product[MAX_INSTRUMENT_LENGTH];	//Ʒ�ִ���
		char _ruletag[MAX_INSTRUMENT_LENGTH];	//
		char _fullpid[MAX_INSTRUMENT_LENGTH];	//

		//By Wesley @ 2021.12.25
		//ȥ����Լ���ͣ����ﲻ�ٽ����ж�
		//����CodeHelper���ع�
		//ContractCategory	_category;		//��Լ����
		//union
		//{
		//	uint8_t	_hotflag;	//������ǣ�0-��������1-������2-������
		//	uint8_t	_exright;	//�Ƿ��Ǹ�Ȩ����,��SH600000Q: 0-����Ȩ, 1-ǰ��Ȩ, 2-��Ȩ
		//};

		/*
		 *	By Wesley @ 2022.03.07
		 *	ȡ��ԭ����union
		 *	Ҫ��������Ǻ͸�Ȩ��Ƿֿ�����
		 *	��Ϊ����Ҫ��������Լ����Ȩ������
		 */
		uint8_t	_exright;	//�Ƿ��Ǹ�Ȩ����,��SH600000Q: 0-����Ȩ, 1-ǰ��Ȩ, 2-��Ȩ

		//�Ƿ��Ǹ�Ȩ����
		inline bool isExright() const { return _exright != 0; }

		//�Ƿ�ǰ��Ȩ����
		inline bool isForwardAdj() const { return _exright == 1; }

		//�Ƿ��Ȩ����
		inline bool isBackwardAdj() const { return _exright == 2; }

		//��׼Ʒ��ID
		inline const char* stdCommID()
		{
			if (strlen(_fullpid) == 0)
				fmtutil::format_to(_fullpid, "{}.{}", _exchg, _product);

			return _fullpid;
		}

		_CodeInfo()
		{
			memset(this, 0, sizeof(_CodeInfo));
			//_category = CC_Future;
		}

		inline void clear()
		{
			memset(this, 0, sizeof(_CodeInfo));
		}

		inline bool hasRule() const
		{
			return strlen(_ruletag) > 0;
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
	 *	�Ƿ����ڻ���Ȩ��Լ����
	 *	CFFEX.IO2007.C.4000
	 */
	static bool	isStdChnFutOptCode(const char* code)
	{
		/* ����������ʽ */
		//static cregex reg_stk = cregex::compile("^[A-Z]+.[A-z]+\\d{4}.(C|P).\\d+$");	//CFFEX.IO2007.C.4000
		//return 	regex_match(code, reg_stk);
		char state = 0;
		std::size_t i = 0;
		for(; ; i++)
		{
			char ch = code[i];
			if(ch == '\0')
				break;

			if(state == 0)
			{
				if (!('A' <= ch && ch <= 'Z'))
					return false;

				state += 1;
			}
			else if (state == 1)
			{
				if ('A' <= ch && ch <= 'Z')
					continue;

				if (ch == '.')
					state += 1;
				else
					return false;
			}
			else if (state == 2)
			{
				if (!('A' <= ch && ch <= 'z'))
					return false;

				state += 1;
			}
			else if (state == 3)
			{
				if ('A' <= ch && ch <= 'Z')
					continue;

				if ('0' <= ch && ch <= '9')
					state += 1;
				else
					return false;
			}
			else if (state >= 4 && state <= 6)
			{
				if ('0' <= ch && ch <= '9')
					state += 1;
				else
					return false;
			}
			else if (state == 7)
			{
				if (ch == '.')
					state += 1;
				else
					return false;
			}
			else if (state == 8)
			{
				if (ch == 'C' || ch == 'P')
					state += 1;
				else
					return false;
			}
			else if (state == 9)
			{
				if (ch == '.')
					state += 1;
				else
					return false;
			}
			else if (state == 10)
			{
				if ('0' <= ch && ch <= '9')
					state += 1;
				else
					return false;
			}
			else if (state == 11)
			{
				if ('0' <= ch && ch <= '9')
					continue;
				else
					return false;
			}
		}

		return (state == 11);
	}

	/*
	 *	�Ƿ��Ǳ�׼�����ڻ���Լ����
	 *	//CFFEX.IF.2007
	 */
	static inline bool	isStdMonthlyFutCode(const char* code)
	{
		using namespace boost::xpressive;
		/* ����������ʽ */
		static cregex reg_stk = cregex::compile("^[A-Z]+.[A-z]+.\\d{4}$");	//CFFEX.IO.2007
		return 	regex_match(code, reg_stk);
	}

	/*
	 *	��׼����ת��׼Ʒ��ID
	 *	��SHFE.ag.1912->SHFE.ag
	 *	����Ǽ򻯵Ĺ�Ʊ���룬��SSE.600000����ת��SSE.STK
	 */
	static inline std::string stdCodeToStdCommID(const char* stdCode)
	{
		auto idx = find(stdCode, '.', true);
		auto idx2 = find(stdCode, '.', false);
		if(idx != idx2)
		{
			//ǰ������.����ͬһ����˵�������εĴ���
			//��ȡǰ������ΪƷ�ִ���
			return std::string (stdCode, idx);
		}
		else
		{
			//���εĴ��룬ֱ�ӷ���
			//��Ҫ���ĳЩ��������ÿ����Լ�Ľ��׹��򶼲�ͬ�����
			//����������ͰѺ�Լֱ�ӵ���Ʒ������
			return stdCode;
		}
	}

	/*
	 *	�ӻ������º�Լ������ȡ����Ʒ�ִ���
	 *	��ag1912 -> ag
	 *	���ֻ�з����ڻ�Ʒ�ֲ�������
	 *	���������������Լ�Ĵ��봫��������������еĻ����ǵ��õĵط���Bug!
	 */
	static inline std::string rawMonthCodeToRawCommID(const char* code)
	{
		int nLen = 0;
		while ('A' <= code[nLen] && code[nLen] <= 'z')
			nLen++;

		return std::string(code, nLen);
	}

	/*
	 *	�������º�Լ����ת��׼��
	 *	��ag1912ת��ȫ��
	 *	���������������Լ�Ĵ��봫��������������еĻ����ǵ��õĵط���Bug!
	 */
	static inline std::string rawMonthCodeToStdCode(const char* code, const char* exchg, bool isComm = false)
	{
		thread_local static char buffer[64] = { 0 };
		std::size_t len = 0;
		if(isComm)
		{
			len = strlen(exchg);
			memcpy(buffer, exchg, len);
			buffer[len] = '.';
			len += 1;

			auto clen = strlen(code);
			memcpy(buffer+len, code, clen);
			len += clen;
			buffer[len] = '\0';
			len += 1;
		}
		else
		{
			std::string pid = rawMonthCodeToRawCommID(code);
			len = strlen(exchg);
			memcpy(buffer, exchg, len);
			buffer[len] = '.';
			len += 1;

			memcpy(buffer + len, pid.c_str(), pid.size());
			len += pid.size();
			buffer[len] = '.';
			len += 1;

			char* s = (char*)code;
			s += pid.size();
			if (strlen(s) == 4)
			{
				wt_strcpy(buffer + len, s, 4);
				len += 4;
			}
			else
			{
				if (s[0] > '5')
					buffer[len] = '1';
				else
					buffer[len] = '2';
				len += 1;
				wt_strcpy(buffer + len, s, 3);
				len += 3;
			}
		}

		return std::string(buffer, len);
	}

	/*
	 *	ԭʼ�������ת��׼����
	 *	������Ҫ��ԷǷ��º�Լ����
	 */
	static inline std::string rawFlatCodeToStdCode(const char* code, const char* exchg, const char* pid)
	{
		thread_local static char buffer[64] = { 0 };
		auto len = strlen(exchg);
		memcpy(buffer, exchg, len);
		buffer[len] = '.';
		len += 1;

		auto plen = strlen(pid);
		auto clen = strlen(code);

		if (strcmp(code, pid) == 0 || plen == 0)
		{
			memcpy(buffer + len, code, clen);
			len += clen;
			buffer[len] = '\0';
		}
		else
		{
			memcpy(buffer + len, pid, plen);
			len += plen;
			buffer[len] = '.';
			len += 1;

			memcpy(buffer + len, code, clen);
			len += clen;
			buffer[len] = '\0';
		}

		return buffer;
	}

	static inline bool isMonthlyCode(const char* code)
	{
		//using namespace boost::xpressive;
		//���3-6λ�������֣����Ƿ��º�Լ
		//static cregex reg_stk = cregex::compile("^.*[A-z|-]\\d{3,6}$");	//CFFEX.IO.2007
		//return 	regex_match(code, reg_stk);
		auto len = strlen(code);
		char state = 0;
		for (std::size_t i = 0; i < len; i++)
		{
			char ch = code[len - i - 1];
			if (0 <= state && state < 3)
			{
				if (!('0' <= ch && ch <= '9'))
					return false;

				state += 1;
			}
			else if (3 <= state && state < 6)
			{
				if ('0' <= ch && ch <= '9')
					state += 1;
				else if (('A' <= ch && ch <= 'z') || ch == '-')
				{
					state = 7;
					break;
				}
			}
			else if (state == 6)
			{
				if (('A' <= ch && ch <= 'z') || ch == '-')
				{
					state = 7;
					break;
				}
			}
		}

		return state == 7;
	}

	/*
	 *	�ڻ���Ȩ�����׼��
	 *	��׼�ڻ���Ȩ�����ʽΪCFFEX.IO2008.C.4300
	 *	-- ��ʱû�еط����� --
	 */
	static inline std::string rawFutOptCodeToStdCode(const char* code, const char* exchg)
	{
		using namespace boost::xpressive;
		/* ����������ʽ */
		static cregex reg_stk = cregex::compile("^[A-z]+\\d{4}-(C|P)-\\d+$");	//�н�������������ʽIO2013-C-4000
		bool bMatch = regex_match(code, reg_stk);
		if(bMatch)
		{
			std::string s = std::move(fmt::format("{}.{}", exchg, code));
			StrUtil::replace(s, "-", ".");
			return s;
		}
		else
		{
			//֣������������Ȩ�����ʽZC2010P11600

			//�ȴӺ���ǰ��λ��P��C��λ��
			std::size_t idx = strlen(code) - 1;
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
			return s;
		}
	}

	/*
	 *	��׼��Լ����ת��������
	 */
	static inline std::string stdCodeToStdHotCode(const char* stdCode)
	{
		std::size_t idx = find(stdCode, '.', true);
		if (idx == std::string::npos)
			return "";		
		
		std::string stdWrappedCode;
		stdWrappedCode.resize(idx + strlen(SUFFIX_HOT) + 1);
		memcpy((char*)stdWrappedCode.data(), stdCode, idx);
		wt_strcpy((char*)stdWrappedCode.data()+idx, SUFFIX_HOT);
		return stdWrappedCode;
	}

	/*
	 *	��׼��Լ����ת����������
	 */
	static inline std::string stdCodeToStd2ndCode(const char* stdCode)
	{
		std::size_t idx = find(stdCode, '.', true);
		if (idx == std::string::npos)
			return "";

		std::string stdWrappedCode;
		stdWrappedCode.resize(idx + strlen(SUFFIX_2ND) + 1);
		memcpy((char*)stdWrappedCode.data(), stdCode, idx);
		wt_strcpy((char*)stdWrappedCode.data() + idx, SUFFIX_2ND);
		return stdWrappedCode;
	}

	/*
	 *	��׼�ڻ���Ȩ����תԭ����
	 *	-- ��ʱû�еط����� --
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
		return ret;
	}

	static inline int indexCodeMonth(const char* code)
	{
		if (strlen(code) == 0)
			return -1;

		std::size_t idx = 0;
		std::size_t len = strlen(code);
		while(idx < len)
		{
			if (isdigit(code[idx]))
				return (int)idx;

			idx++;
		}
		return -1;
	}

	/*
	 *	��ȡ��׼�ڻ���Ȩ�������Ϣ
	 */
	static CodeInfo extractStdChnFutOptCode(const char* stdCode)
	{
		CodeInfo codeInfo;

		StringVector ay = StrUtil::split(stdCode, ".");
		wt_strcpy(codeInfo._exchg, ay[0].c_str());
		if(strcmp(codeInfo._exchg, "SHFE") == 0 || strcmp(codeInfo._exchg, "CZCE") == 0)
		{
			fmt::format_to(codeInfo._code, "{}{}{}", ay[1], ay[2], ay[3]);
		}
		else
		{
			fmt::format_to(codeInfo._code, "{}-{}-{}", ay[1], ay[2], ay[3]);
		}

		int mpos = indexCodeMonth(ay[1].c_str());

		if(strcmp(codeInfo._exchg, "CZCE") == 0)
		{
			memcpy(codeInfo._product, ay[1].c_str(), mpos);
			strcat(codeInfo._product, ay[2].c_str());
		}
		else if (strcmp(codeInfo._exchg, "CFFEX") == 0)
		{
			memcpy(codeInfo._product, ay[1].c_str(), mpos);
		}
		else
		{
			memcpy(codeInfo._product, ay[1].c_str(), mpos);
			strcat(codeInfo._product, "_o");
		}

		return codeInfo;
	}

	/*
	 *	�����׼�������Ϣ
	 */
	static CodeInfo extractStdCode(const char* stdCode, IHotMgr *hotMgr)
	{
		//��Ȩ�Ĵ���������������һ�������Ե����ж�
		if(isStdChnFutOptCode(stdCode))
		{
			return extractStdChnFutOptCode(stdCode);
		}
		else
		{
			/*
			 *	By Wesley @ 2021.12.25
			 *	1���ȿ��ǲ���Q��H��β�ģ�����Ǹ�Ȩ���ȷ���Ժ����һ�γ���-1�����Ƶ�code����SSE.STK.600000Q
			 *	2���ٿ��ǲ��Ƿ��º�Լ������ǣ���product�ֶ�ƴ���·ݸ�code��֣�������⴦������CFFEX.IF.2112
			 *	3����󿴿��ǲ���HOT��2ND��β�ģ�����ǣ���product������code����DCE.m.HOT
			 *	4����������ǣ���ԭ�����Ƶ����Σ���BINANCE.DC.BTCUSDT/SSE.STK.600000
			 */
			thread_local static CodeInfo codeInfo;
			codeInfo.clear();
			auto idx = StrUtil::findFirst(stdCode, '.');
			wt_strcpy(codeInfo._exchg, stdCode, idx);

			auto idx2 = StrUtil::findFirst(stdCode + idx + 1, '.');
			if (idx2 == std::string::npos)
			{
				wt_strcpy(codeInfo._product, stdCode + idx + 1);

				//By Wesley @ 2021.12.29
				//��������εĺ�Լ���룬��OKEX.BTC-USDT
				//��Ʒ�ִ���ͺ�Լ����һ��
				wt_strcpy(codeInfo._code, stdCode + idx + 1);
			}
			else
			{
				wt_strcpy(codeInfo._product, stdCode + idx + 1, idx2);
				const char* ext = stdCode + idx + idx2 + 2;
				std::size_t extlen = strlen(ext);
				char lastCh = ext[extlen - 1];
				if (lastCh == SUFFIX_QFQ || lastCh == SUFFIX_HFQ)
				{
					codeInfo._exright = (lastCh == SUFFIX_QFQ) ? 1 : 2;

					extlen--;
					lastCh = ext[extlen - 1];
				}
				
				if (extlen == 4 && '0' <= lastCh && lastCh <= '9')
				{
					//������һ����4λ���֣�˵���Ƿ��º�Լ
					//TODO: �������жϴ���һ�����裬���һλ�����ֵ�һ�����ڻ����º�Լ���Ժ���ܻ������⣬��ע��һ��
					//��ôcode�ü���Ʒ��id
					//֣�����õ�������һ�£����ֻ��hardcode��
					auto i = wt_strcpy(codeInfo._code, codeInfo._product);
					if (memcmp(codeInfo._exchg, "CZCE", 4) == 0)
						wt_strcpy(codeInfo._code + i, ext + 1, extlen-1);
					else
						wt_strcpy(codeInfo._code + i, ext, extlen);
				}
				else
				{
					const char* ruleTag = (hotMgr != NULL) ? hotMgr->getRuleTag(ext) :"";
					if (strlen(ruleTag) == 0)
						wt_strcpy(codeInfo._code, ext, extlen);
					else
					{
						wt_strcpy(codeInfo._code, codeInfo._product);
						wt_strcpy(codeInfo._ruletag, ruleTag);
					}
				}
			}			

			return codeInfo;
		}
	}
};

