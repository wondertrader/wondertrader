/*!
 * \file WTSSwitchItem.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Wt主力切换规则对象定义文件
 */
#pragma once
#include "WTSObject.hpp"
#include <math.h>

inline bool eq(double a, double b = 0.0)
{
	return(fabs(a - b) < 1e-6);
}

NS_WTP_BEGIN
class WTSSwitchItem : public WTSObject
{
protected:
	WTSSwitchItem(){}
	virtual ~WTSSwitchItem(){}

public:
	static WTSSwitchItem* create(const char* exchg, const char* product, const char* from, const char* to, uint32_t dt, double oldclose = 0, double newclose = 0)
	{
		WTSSwitchItem* pRet = new WTSSwitchItem();
		pRet->_exchg = exchg;
		pRet->_product = product;
		pRet->_from = from;
		pRet->_to = to;
		pRet->_dt = dt;
		pRet->_oldclose = oldclose;
		pRet->_newclose = newclose;
		pRet->_factor = eq(oldclose, 0) ? 1 : (newclose / oldclose);
		return pRet;
	}

	const char*		exchg() const{return _exchg.c_str();}
	const char*		product() const{return _product.c_str();}
	const char*		from() const{return _from.c_str();}
	const char*		to() const{return _to.c_str();}
	uint32_t		switchdate() const{return _dt;}

private:
	std::string		_exchg;
	std::string		_product;
	std::string		_from;
	std::string		_to;
	uint32_t		_dt;
	double		_oldclose;
	double		_newclose;
	double		_factor;
};
NS_WTP_END
