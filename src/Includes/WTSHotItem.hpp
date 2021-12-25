/*!
 * \file WTSHotItem.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief Wt主力切换规则对象定义文件
 */
#pragma once
#include "WTSObject.hpp"

NS_WTP_BEGIN
class WTSHotItem : public WTSObject
{
protected:
	WTSHotItem(){}
	virtual ~WTSHotItem(){}

public:
	static WTSHotItem* create(const char* exchg, const char* product, const char* from, const char* to, uint32_t dt, double oldclose = 0, double newclose = 0)
	{
		WTSHotItem* pRet = new WTSHotItem();
		pRet->_exchg = exchg;
		pRet->_product = product;
		pRet->_hot = pRet->_product + "0001";
		pRet->_from = from;
		pRet->_to = to;
		pRet->_dt = dt;
		pRet->_oldclose = oldclose;
		pRet->_newclose = newclose;
		return pRet;
	}

	const char*		exchg() const{return _exchg.c_str();}
	const char*		product() const{return _product.c_str();}
	const char*		hot() const{return _hot.c_str();}
	const char*		from() const{return _from.c_str();}
	const char*		to() const{return _to.c_str();}
	uint32_t		switchdate() const{return _dt;}

private:
	std::string		_exchg;
	std::string		_product;
	std::string		_hot;
	std::string		_from;
	std::string		_to;
	uint32_t		_dt;
	double		_oldclose;
	double		_newclose;
};
NS_WTP_END
