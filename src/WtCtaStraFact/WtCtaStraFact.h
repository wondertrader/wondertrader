#pragma once
#include "../Includes/CtaStrategyDefs.h"

USING_NS_WTP;

class WtStraFact : public ICtaStrategyFact
{
public:
	WtStraFact();
	virtual ~WtStraFact();

public:
	/*
	 *	获取工厂名称
	 */
	virtual const char* getName() override;

	/*
	 *	创建策略
	 *	@name	策略名称
	 *	@id		要创建的策略对象的ID
	 */
	virtual CtaStrategy* createStrategy(const char* name, const char* id) override;

	/*
	 *	枚举策略名称
	 *	@cb	枚举策略名称的回调函数
	 */
	virtual void enumStrategy(FuncEnumStrategyCallback cb) override;

	/*
	 *	删除策略
	 *	@stra	策略对象指针
	 */
	virtual bool deleteStrategy(CtaStrategy* stra) override;	
};

