/*!
 * \file CtaStrategyDefs.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <string>
#include <stdint.h>

#include "../Includes/WTSMarcos.h"

NS_WTP_BEGIN
class WTSVariant;
class ICtaStraCtx;
class ICtaTickStraCtx;
class WTSTickData;
struct WTSBarStruct;
NS_WTP_END

USING_NS_WTP;

class CtaStrategy
{
public:
	CtaStrategy(const char* id) :_id(id){}
	virtual ~CtaStrategy(){}

public:
	/*
	*	ִ�е�Ԫ����
	*/
	virtual const char* getName() = 0;

	/*
	*	����ִ������������
	*/
	virtual const char* getFactName() = 0;

	/*
	*	��ʼ��
	*/
	virtual bool init(WTSVariant* cfg){ return true; }

	virtual const char* id() const { return _id.c_str(); }

	/*
	 *	��ʼ���ص�
	 */
	virtual void on_init(ICtaStraCtx* ctx){}

	/*
	 *	�����տ�ʼ
	 */
	virtual void on_session_begin(ICtaStraCtx* ctx, uint32_t uTDate) {}

	/*
	 *	�����ս���
	 */
	virtual void on_session_end(ICtaStraCtx* ctx, uint32_t uTDate) {}

	/*
	 *	�����߼�ִ�����
	 */
	virtual void on_schedule(ICtaStraCtx* ctx, uint32_t uDate, uint32_t uTime){}

	/*
	 *	�����߼�ִ�����
	 */
	virtual void on_schedule_done(ICtaStraCtx* ctx, uint32_t uDate, uint32_t uTime) {}

	/*
	 *	tick����
	 */
	virtual void on_tick(ICtaStraCtx* ctx, const char* stdCode, WTSTickData* newTick){}

	/*
	 *	K�߱պ�
	 */
	virtual void on_bar(ICtaStraCtx* ctx, const char* stdCode, const char* period, WTSBarStruct* newBar){}

	/*
	 *	����������
	 */
	virtual void on_condition_triggered(ICtaStraCtx* ctx, const char* stdCode, double target, double price, const char* usertag) {}

protected:
	std::string _id;
};

//////////////////////////////////////////////////////////////////////////
//���Թ����ӿ�
typedef void(*FuncEnumStrategyCallback)(const char* factName, const char* straName, bool isLast);

class ICtaStrategyFact
{
public:
	ICtaStrategyFact(){}
	virtual ~ICtaStrategyFact(){}

public:
	/*
	 *	��ȡ������
	 */
	virtual const char* getName() = 0;

	/*
	 *	ö�ٲ���
	 */
	virtual void enumStrategy(FuncEnumStrategyCallback cb) = 0;

	/*
	 *	�������ƴ���K�߼������
	 */
	virtual CtaStrategy* createStrategy(const char* name, const char* id) = 0;


	/*
	 *	ɾ������
	 */
	virtual bool deleteStrategy(CtaStrategy* stra) = 0;
};

//����ִ�й���
typedef ICtaStrategyFact* (*FuncCreateStraFact)();
//ɾ��ִ�й���
typedef void(*FuncDeleteStraFact)(ICtaStrategyFact* &fact);
