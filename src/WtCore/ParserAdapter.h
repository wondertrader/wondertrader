/*!
 * \file ParserAdapter.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <memory>
#include <boost/core/noncopyable.hpp>

#include "../Includes/FasterDefs.h"
#include "../Includes/IParserApi.h"


NS_WTP_BEGIN
class WTSVariant;
class IHotMgr;

class IParserStub
{
public:
	virtual void			handle_push_quote(WTSTickData* curTick, uint32_t hotFlag = 0){}

	virtual void			handle_push_order_detail(WTSOrdDtlData* curOrdDtl){}
	virtual void			handle_push_order_queue(WTSOrdQueData* curOrdQue) {}
	virtual void			handle_push_transaction(WTSTransData* curTrans) {}
};

class ParserAdapter : public IParserSpi,
					private boost::noncopyable
{
public:
	ParserAdapter();
	~ParserAdapter();

public:
	bool	init(const char* id, WTSVariant* cfg, IParserStub* stub, IBaseDataMgr* bgMgr, IHotMgr* hotMgr = NULL);

	bool	initExt(const char* id, IParserApi* api, IParserStub* stub, IBaseDataMgr* bgMgr, IHotMgr* hotMgr = NULL);

	void	release();

	bool	run();

	const char* id() const{ return _id.c_str(); }

public:
	virtual void handleSymbolList(const WTSArray* aySymbols) override {}

	/*
	 *	����ʵʱ����
	 *	@quote		ʵʱ����
	 *	@bNeedSlice	�Ƿ���Ҫ��Ƭ,����Ǵ��ⲿ����Ŀ�����������,����Ҫ��Ƭ,������ڲ��㲥�ľͲ���Ҫ��Ƭ
	 */
	virtual void handleQuote(WTSTickData *quote, uint32_t procFlag) override;

	/*
	 *	����ί�ж������ݣ���Ʊlevel2��
	 *	@ordQueData	ί�ж�������
	 */
	virtual void handleOrderQueue(WTSOrdQueData* ordQueData) override;

	/*
	 *	�������ί�����ݣ���Ʊlevel2��
	 *	@ordDetailData	���ί������
	 */
	virtual void handleOrderDetail(WTSOrdDtlData* ordDetailData) override;

	/*
		*	������ʳɽ�����
		*	@transData	��ʳɽ�����
		*/
	virtual void handleTransaction(WTSTransData* transData) override;

	virtual void handleParserLog(WTSLogLevel ll, const char* message) override;

	virtual IBaseDataMgr* getBaseDataMgr() override { return _bd_mgr; }


private:
	IParserApi*			_parser_api;
	FuncDeleteParser	_remover;

	bool				_stopped;

	/*
	 *	���ʱ��������
	 *	���Ϊtrue�������յ������ʱ�����ʱ����
	 *	��Ҫ������ֱ�Ӵ�����Դ����
	 *	��Ϊֱ�Ӵ�����Դ����ܿ��ܻ��д���ʱ��������ݽ���
	 *	��ѡ��Ĭ��Ϊfalse
	 */
	bool				_check_time;

	typedef faster_hashset<LongKey>	ExchgFilter;
	ExchgFilter			_exchg_filter;
	ExchgFilter			_code_filter;
	IBaseDataMgr*		_bd_mgr;
	IHotMgr*			_hot_mgr;
	IParserStub*		_stub;
	WTSVariant*			_cfg;
	std::string			_id;
};

typedef std::shared_ptr<ParserAdapter>	ParserAdapterPtr;
typedef faster_hashmap<std::string, ParserAdapterPtr>	ParserAdapterMap;

class ParserAdapterMgr : private boost::noncopyable
{
public:
	void	release();

	void	run();

	ParserAdapterPtr getAdapter(const char* id);

	bool	addAdapter(const char* id, ParserAdapterPtr& adapter);


public:
	ParserAdapterMap _adapters;
};

NS_WTP_END