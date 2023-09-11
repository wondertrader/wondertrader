/*!
 * \file WtDtPorter.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 
 */
#pragma once
#include "PorterDefs.h"


#ifdef __cplusplus
extern "C"
{
#endif

	EXPORT_FLAG void		initialize(WtString cfgFile, WtString logCfg, bool bCfgFile, bool bLogCfgFile);
	EXPORT_FLAG void		start(bool bAsync = false);

	EXPORT_FLAG	WtString	get_version();
	EXPORT_FLAG	void		write_log(unsigned int level, const char* message, const char* catName);


#pragma region "扩展Parser接口"
	/*
	 *	创建ExtParser
	 *	@id		parserid
	 *	@return	创建是否成功
	 */
	EXPORT_FLAG	bool		create_ext_parser(const char* id);

	/*
	 *	注册ExtParser的回调函数
	 *	@cbEvt	行情解析器事件回调函数，主要是连接、关闭、登录等事件
	 *	@cbSub	行情订阅结果回调
	 */
	EXPORT_FLAG void		register_parser_callbacks(FuncParserEvtCallback cbEvt, FuncParserSubCallback cbSub);

	/*
	 *	向底层推送tick数据
	 *	@id			parserid
	 *	@curTick	最新tick数据
	 *	@procFlag	处理标记，0-切片行情，无需处理(ParserUDP)；1-完整快照，需要切片(国内各路通道)；2-极简快照，需要缓存累加（主要针对日线、tick，m1和m5都是自动累加的，虚拟货币行情）
	 */
	EXPORT_FLAG	void		parser_push_quote(const char* id, WTSTickStruct* curTick, WtUInt32 uProcFlag);
#pragma endregion "扩展Parser接口"

#pragma region "扩展Dumper接口"
	EXPORT_FLAG	bool		create_ext_dumper(const char* id);

	EXPORT_FLAG void		register_extended_dumper(FuncDumpBars barDumper, FuncDumpTicks tickDumper);

	EXPORT_FLAG void		register_extended_hftdata_dumper(FuncDumpOrdQue ordQueDumper, FuncDumpOrdDtl ordDtlDumper, FuncDumpTrans transDumper);
#pragma endregion "扩展Dumper接口"

#ifdef __cplusplus
}
#endif