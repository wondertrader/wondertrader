/*!
 * \file IRdmDtReader.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include <stdint.h>

#include "../Includes/WTSMarcos.h"
#include "../Includes/WTSTypes.h"

NS_WTP_BEGIN
class WTSKlineSlice;
class WTSTickSlice;
class WTSOrdQueSlice;
class WTSOrdDtlSlice;
class WTSTransSlice;
class IBaseDataMgr;
class IHotMgr;
class WTSVariant;


/*
 *	@brief ���ݶ�ȡģ��ص��ӿ�
 *	@details ��Ҫ�������ݶ�ȡģ�������ģ��ص�
 */
class IRdmDtReaderSink
{
public:
	/*
	 *	@brief	��ȡ�������ݹ���ӿ�ָ��
	 */
	virtual IBaseDataMgr*	get_basedata_mgr() = 0;

	/*
	 *	@brief	��ȡ�����л��������ӿ�ָ��
	 */
	virtual IHotMgr*		get_hot_mgr() = 0;

	/*
	 *	@brief	������ݶ�ȡģ�����־
	 */
	virtual void		reader_log(WTSLogLevel ll, const char* message) = 0;
};

/*
 *	@brief	������ݶ�ȡ�ӿ�
 *
 *	�����ģ���ṩ��������(tick��K��)��ȡ�ӿ�
 */
class IRdmDtReader
{
public:
	IRdmDtReader() :_sink(NULL) {}
	virtual ~IRdmDtReader(){}

public:
	virtual void init(WTSVariant* cfg, IRdmDtReaderSink* sink) { _sink = sink; }

	virtual WTSOrdDtlSlice*	readOrdDtlSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) = 0;
	virtual WTSOrdQueSlice*	readOrdQueSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) = 0;
	virtual WTSTransSlice*	readTransSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) = 0;

	virtual WTSTickSlice*	readTickSliceByDate(const char* stdCode, uint32_t uDate = 0) = 0;

	virtual WTSTickSlice*	readTickSliceByRange(const char* stdCode, uint64_t stime, uint64_t etime = 0) = 0;
	virtual WTSKlineSlice*	readKlineSliceByRange(const char* stdCode, WTSKlinePeriod period, uint64_t stime, uint64_t etime = 0) = 0;

	virtual WTSTickSlice*	readTickSliceByCount(const char* stdCode, uint32_t count, uint64_t etime = 0) = 0;
	virtual WTSKlineSlice*	readKlineSliceByCount(const char* stdCode, WTSKlinePeriod period, uint32_t count, uint64_t etime = 0) = 0;

protected:
	IRdmDtReaderSink*	_sink;
};

//�������ݴ洢����
typedef IRdmDtReader* (*FuncCreateRdmDtReader)();
//ɾ�����ݴ洢����
typedef void(*FuncDeleteRdmDtReader)(IRdmDtReader* store);

NS_WTP_END