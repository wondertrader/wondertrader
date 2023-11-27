#pragma once
#include "../Includes/WTSMarcos.h"

NS_WTP_BEGIN
class WTSTickData;
class WTSVariant;
class WTSOrdDtlData;
class WTSOrdQueData;
class WTSTransData;

class IDataCaster
{
public:
	virtual void	broadcast(WTSTickData* curTick) = 0;
	virtual void	broadcast(WTSOrdQueData* curOrdQue){}
	virtual void	broadcast(WTSOrdDtlData* curOrdDtl){}
	virtual void	broadcast(WTSTransData* curTrans){}
};

NS_WTP_END
