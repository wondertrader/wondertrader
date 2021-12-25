/*!
 * \file IHotMgr.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 主力合约管理器接口定义
 */
#pragma once
#include "WTSMarcos.h"
#include <vector>
#include <string>
#include <stdint.h>

typedef struct _HotSection
{
	std::string	_code;
	uint32_t	_s_date;
	uint32_t	_e_date;

	_HotSection(const char* code, uint32_t sdate, uint32_t edate)
		: _s_date(sdate), _e_date(edate), _code(code)
	{
	
	}

} HotSection;
typedef std::vector<HotSection>	HotSections;

NS_WTP_BEGIN

#define HOTS_MARKET		"HOTS_MARKET"
#define SECONDS_MARKET	"SECONDS_MARKET"

class IHotMgr
{
public:
	/*
	 *	获取分月代码
	 *	@pid	品种代码
	 *	@dt		日期(交易日)
	 */
	virtual const char* getRawCode(const char* exchg, const char* pid, uint32_t dt)	= 0;

	/*
	 *	获取主力对一个的上一个分月,即上一个主力合约的分月代码
	 *	@pid	品种代码
	 *	@dt		日期(交易日)
	 */
	virtual const char* getPrevRawCode(const char* exchg, const char* pid, uint32_t dt) = 0;

	/*
	 *	获取主力代码
	 *	@rawCode	分月代码
	 *	@dt			日期(交易日)
	 */
	virtual const char* getHotCode(const char* exchg, const char* rawCode, uint32_t dt) = 0;

	/*
	 *	是否主力合约
	 *	@rawCode	分月代码
	 *	@dt			日期(交易日)
	 */
	virtual bool		isHot(const char* exchg, const char* rawCode, uint32_t dt) = 0;

	/*
	 *	分割主力段,将主力合约在某个时段的分月合约全部提出取来
	 */
	virtual bool		splitHotSecions(const char* exchg, const char* hotCode, uint32_t sDt, uint32_t eDt, HotSections& sections) = 0;

	/*
	 *	获取次主力分月代码
	 *	@pid	品种代码
	 *	@dt		日期(交易日)
	 */
	virtual const char* getSecondRawCode(const char* exchg, const char* pid, uint32_t dt) = 0;

	/*
	 *	获取次主力对一个的上一个分月,即上一个次主力合约的分月代码
	 *	@pid	品种代码
	 *	@dt		日期(交易日)
	 */
	virtual const char* getPrevSecondRawCode(const char* exchg, const char* pid, uint32_t dt) = 0;

	/*
	 *	获取次主力代码
	 *	@rawCode	分月代码
	 *	@dt			日期(交易日)
	 */
	virtual const char* getSecondCode(const char* exchg, const char* rawCode, uint32_t dt) = 0;

	/*
	 *	是否次主力合约
	 *	@rawCode	分月代码
	 *	@dt			日期(交易日)
	 */
	virtual bool		isSecond(const char* exchg, const char* rawCode, uint32_t dt) = 0;

	/*
	 *	分割次主力段,将次主力合约在某个时段的分月合约全部提出取来
	 */
	virtual bool		splitSecondSecions(const char* exchg, const char* hotCode, uint32_t sDt, uint32_t eDt, HotSections& sections) = 0;
};
NS_WTP_END
