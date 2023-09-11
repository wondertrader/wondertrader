#ifndef SSE_STRUCT_H
#define SSE_STRUCT_H

#include <stdint.h>
#include <iomanip>
#define MESSAGE_MARKET_DATA        0x01
#define MESSAGE_DEPTH              0x02
#define MESSAGE_MARKET_DATA_STATIC 0x03
#define MESSAGE_SNAP_SHOT          0x04
#define MESSAGE_DEPTH_SNAP_SHOT    0x05
#define MESSAGE_TYPE_HEART_BEAT    0x00
#define XELE_MD_DATA_VERSION       0x01


typedef int16_t  TXeleMdInstrumentNo16_Type;
typedef int32_t  TXeleMdSnapNoType;
typedef int32_t  TXeleMdChangeNoType;
typedef uint32_t TXeleMdSnapDateTimeType;
typedef int32_t  TXeleMdPriceTypeInt;
typedef int16_t  TXeleMdVolume16_Type;
typedef int32_t  TXeleMdVolume32_Type;
typedef int32_t  TXeleMdLastPriceType;
typedef int64_t  TXeleMdTurnoverType;
typedef double   TXeleMdTurnoverTypeDouble;
typedef int64_t  TXeleMdOpenInterestType;
typedef double   TXeleMdOpenInterestTypeDouble;
typedef int32_t  TXeleMdPacketNoType;
typedef int16_t  TXeleMdTopicIDType;
typedef uint16_t TXeleMdSnapMillisecType;
typedef uint32_t TXeleMdSnapTimeType;
typedef int8_t   TXeleMdCenterChangeNoType;
typedef int16_t  TXeleMdLengthType;
typedef char     TXeleMdInstrumentIDType[31];
typedef char     TXeleMdUnderlyingInstrIDType[31];
typedef char     TXeleMdProductClassType;
typedef double   TXeleMdStrikePriceType;
typedef char     TXeleMdOptionsTypeType;
typedef int32_t  TXeleMdVolumeMultipleType;
typedef double   TXeleMdUnderlyingMultipleType;
typedef int32_t  TXeleMdIsTradingType;
typedef char     TXeleMdCurrencyIDType[4];
typedef double   TXeleMdPriceTickType;
typedef double   TXeleMdCodecPriceType;
typedef int32_t  TXeleMdInstrumentNo32_Type;
typedef double   TXeleMdPriceTypeDouble;
typedef double   TXeleMdPreOpenInterestType;
typedef double   TXeleMdPreDeltaType;
typedef double   TXeleMdCurrDeltType;
typedef char     TXeleMdActionDayType[9];
typedef char     TXeleMdUpdateTimeType[9];
typedef int32_t  TXeleMdUpdateMilliSecType;

#pragma pack(push, 1)
struct CXeleShfeMarketHead {
  int8_t Version;
  int8_t Type;
};

struct CXeleShfeMarketData {
  TXeleMdInstrumentNo16_Type InstrumentNo;
  TXeleMdSnapDateTimeType    SnapDateTime;
  TXeleMdPriceTypeInt        BidPrice;
  TXeleMdVolume32_Type       BidVolume;
  TXeleMdPriceTypeInt        AskPrice;
  TXeleMdVolume32_Type       AskVolume;
  TXeleMdLastPriceType       LastPrice;
  TXeleMdVolume32_Type       Volume;
  TXeleMdTurnoverType        Turnover;
  TXeleMdOpenInterestType    OpenInterest;
};

struct CXeleShfeHeartBeat {
  TXeleMdLengthType         Length;
  TXeleMdPacketNoType       PacketNo;
  TXeleMdTopicIDType        TopicID;
  TXeleMdSnapMillisecType   SnapMillisec;
  TXeleMdSnapNoType         SnapNo;
  TXeleMdSnapTimeType       SnapTime;
  TXeleMdCenterChangeNoType CenterChangeNo;
};

struct CXeleShfeDepthMarketData {
  TXeleMdInstrumentNo16_Type InstrumentNo;
  TXeleMdSnapDateTimeType    SnapDateTime;
  TXeleMdPriceTypeInt        BidPrice1;
  TXeleMdVolume32_Type       BidVolume1;
  TXeleMdPriceTypeInt        AskPrice1;
  TXeleMdVolume32_Type       AskVolume1;
  TXeleMdPriceTypeInt        BidPrice2;
  TXeleMdVolume32_Type       BidVolume2;
  TXeleMdPriceTypeInt        AskPrice2;
  TXeleMdVolume32_Type       AskVolume2;
  TXeleMdPriceTypeInt        BidPrice3;
  TXeleMdVolume32_Type       BidVolume3;
  TXeleMdPriceTypeInt        AskPrice3;
  TXeleMdVolume32_Type       AskVolume3;
  TXeleMdPriceTypeInt        BidPrice4;
  TXeleMdVolume32_Type       BidVolume4;
  TXeleMdPriceTypeInt        AskPrice4;
  TXeleMdVolume32_Type       AskVolume4;
  TXeleMdPriceTypeInt        BidPrice5;
  TXeleMdVolume32_Type       BidVolume5;
  TXeleMdPriceTypeInt        AskPrice5;
  TXeleMdVolume32_Type       AskVolume5;
  TXeleMdLastPriceType       LastPrice;
  TXeleMdVolume32_Type       Volume;
  TXeleMdTurnoverType        Turnover;
  TXeleMdOpenInterestType    OpenInterest;
};


struct CXeleShfeMarketDataStatic {
  TXeleMdLengthType             Length;
  TXeleMdInstrumentIDType       InstrumentID;
  TXeleMdUnderlyingInstrIDType  UnderlyingInstrID;
  TXeleMdProductClassType       ProductClass;
  TXeleMdStrikePriceType        StrikePrice;
  TXeleMdOptionsTypeType        OptionsType;
  TXeleMdVolumeMultipleType     VolumeMultiple;
  TXeleMdUnderlyingMultipleType UnderlyingMultiple;
  TXeleMdIsTradingType          IsTrading;
  TXeleMdCurrencyIDType         CurrencyID;
  TXeleMdPriceTickType          PriceTick;
  TXeleMdCodecPriceType         CodecPrice;
  TXeleMdInstrumentNo32_Type    InstrumentNo;
  TXeleMdPriceTypeDouble        HighestPrice;
  TXeleMdPriceTypeDouble        LowestPrice;
  TXeleMdPriceTypeDouble        OpenPrice;
  TXeleMdPriceTypeDouble        ClosePrice;
  TXeleMdPriceTypeDouble        SettlementPrice;
  TXeleMdPriceTypeDouble        UpperLimitPrice;
  TXeleMdPriceTypeDouble        LowerLimitPrice;
  TXeleMdPriceTypeDouble        PreSettlementPrice;
  TXeleMdPriceTypeDouble        PreClosePrice;
  TXeleMdPreOpenInterestType    PreOpenInterest;
  TXeleMdPreDeltaType           PreDelta;
  TXeleMdCurrDeltType           CurrDelta;
};

struct CXeleShfeSnapShot {
  TXeleMdLengthType             Length;
  TXeleMdInstrumentIDType       InstrumentID;
  TXeleMdUnderlyingInstrIDType  UnderlyingInstrID;
  TXeleMdProductClassType       ProductClass;
  TXeleMdStrikePriceType        StrikePrice;
  TXeleMdOptionsTypeType        OptionsType;
  TXeleMdVolumeMultipleType     VolumeMultiple;
  TXeleMdUnderlyingMultipleType UnderlyingMultiple;
  TXeleMdIsTradingType          IsTrading;
  TXeleMdCurrencyIDType         CurrencyID;
  TXeleMdPriceTickType          PriceTick;
  TXeleMdCodecPriceType         CodecPrice;
  TXeleMdInstrumentNo32_Type    InstrumentNo;
  TXeleMdSnapNoType             SnapNo;
  TXeleMdPacketNoType           PacketNo;
  TXeleMdChangeNoType           ChangeNo;
  TXeleMdPriceTypeDouble        HighestPrice;
  TXeleMdPriceTypeDouble        LowestPrice;
  TXeleMdPriceTypeDouble        OpenPrice;
  TXeleMdPriceTypeDouble        ClosePrice;
  TXeleMdPriceTypeDouble        SettlementPrice;
  TXeleMdPriceTypeDouble        UpperLimitPrice;
  TXeleMdPriceTypeDouble        LowerLimitPrice;
  TXeleMdPriceTypeDouble        PreSettlementPrice;
  TXeleMdPriceTypeDouble        PreClosePrice;
  TXeleMdPreOpenInterestType    PreOpenInterest;
  TXeleMdPreDeltaType           PreDelta;
  TXeleMdCurrDeltType           CurrDelta;
  TXeleMdActionDayType          ActionDay;
  TXeleMdUpdateTimeType         UpdateTime;
  TXeleMdUpdateMilliSecType     UpdateMilliSec;
  TXeleMdPriceTypeDouble        BidPrice;
  TXeleMdVolume32_Type          BidVolume;
  TXeleMdPriceTypeDouble        AskPrice;
  TXeleMdVolume32_Type          AskVolume;
  TXeleMdPriceTypeDouble        LastPrice;
  TXeleMdVolume32_Type          Volume;
  TXeleMdTurnoverTypeDouble     Turnover;
  TXeleMdOpenInterestTypeDouble OpenInterest;
};

struct CXeleShfeDepthSnapShot {
  TXeleMdLengthType             Length;
  TXeleMdInstrumentIDType       InstrumentID;
  TXeleMdUnderlyingInstrIDType  UnderlyingInstrID;
  TXeleMdProductClassType       ProductClass;
  TXeleMdStrikePriceType        StrikePrice;
  TXeleMdOptionsTypeType        OptionsType;
  TXeleMdVolumeMultipleType     VolumeMultiple;
  TXeleMdUnderlyingMultipleType UnderlyingMultiple;
  TXeleMdIsTradingType          IsTrading;
  TXeleMdCurrencyIDType         CurrencyID;
  TXeleMdPriceTickType          PriceTick;
  TXeleMdCodecPriceType         CodecPrice;
  TXeleMdInstrumentNo32_Type    InstrumentNo;
  TXeleMdSnapNoType             SnapNo;
  TXeleMdPacketNoType           PacketNo;
  TXeleMdChangeNoType           ChangeNo;
  TXeleMdPriceTypeDouble        HighestPrice;
  TXeleMdPriceTypeDouble        LowestPrice;
  TXeleMdPriceTypeDouble        OpenPrice;
  TXeleMdPriceTypeDouble        ClosePrice;
  TXeleMdPriceTypeDouble        SettlementPrice;
  TXeleMdPriceTypeDouble        UpperLimitPrice;
  TXeleMdPriceTypeDouble        LowerLimitPrice;
  TXeleMdPriceTypeDouble        PreSettlementPrice;
  TXeleMdPriceTypeDouble        PreClosePrice;
  TXeleMdPreOpenInterestType    PreOpenInterest;
  TXeleMdPreDeltaType           PreDelta;
  TXeleMdCurrDeltType           CurrDelta;
  TXeleMdActionDayType          ActionDay;
  TXeleMdUpdateTimeType         UpdateTime;
  TXeleMdUpdateMilliSecType     UpdateMilliSec;
  TXeleMdPriceTypeDouble        BidPrice1;
  TXeleMdVolume32_Type          BidVolume1;
  TXeleMdPriceTypeDouble        AskPrice1;
  TXeleMdVolume32_Type          AskVolume1;
  TXeleMdPriceTypeDouble        BidPrice2;
  TXeleMdVolume32_Type          BidVolume2;
  TXeleMdPriceTypeDouble        AskPrice2;
  TXeleMdVolume32_Type          AskVolume2;
  TXeleMdPriceTypeDouble        BidPrice3;
  TXeleMdVolume32_Type          BidVolume3;
  TXeleMdPriceTypeDouble        AskPrice3;
  TXeleMdVolume32_Type          AskVolume3;
  TXeleMdPriceTypeDouble        BidPrice4;
  TXeleMdVolume32_Type          BidVolume4;
  TXeleMdPriceTypeDouble        AskPrice4;
  TXeleMdVolume32_Type          AskVolume4;
  TXeleMdPriceTypeDouble        BidPrice5;
  TXeleMdVolume32_Type          BidVolume5;
  TXeleMdPriceTypeDouble        AskPrice5;
  TXeleMdVolume32_Type          AskVolume5;
  TXeleMdPriceTypeDouble        LastPrice;
  TXeleMdVolume32_Type          Volume;
  TXeleMdTurnoverTypeDouble     Turnover;
  TXeleMdOpenInterestTypeDouble OpenInterest;
};

#pragma pack(pop)

//end of struct

#endif
