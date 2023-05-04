/////////////////////////////////////////////////////////////////////////
///产品 HTS
///公司 福建顶点软件股份有限公司
///文件 itpdk_typedef.h
///用途 定义了类型
///版本
///20210910  5.1.0.0	郑东辉		新增版本说明
/////////////////////////////////////////////////////////////////////////

#pragma once


#ifdef _WINDOWS
#define ITPDK_API  extern "C" __declspec(dllexport)
#else
#define ITPDK_API  extern "C" __attribute__ ((visibility("default")))
#endif

//定义推送回调函数
typedef void(*pMessageCallbackMethod)(const char* pTime, const char* pMsg, int nType);
//定义异步回调函数
typedef void(*pAsyncCallbackFunc)(const char* pTime, const char* pMsg, int nType);
//定义掉线、重连回调函数
typedef void(*pConnEventCallback)(const char* pKhh, const char* pConnKey, int nEvent, int nType);


//基本类型
#if defined(__APPLE__)

typedef signed char    int8;
typedef short          int16;
typedef int            int32;
typedef long           int64;

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef unsigned long  uint64;

typedef unsigned char  size8;
typedef unsigned short size16;
typedef unsigned int   size32;
typedef unsigned long  size64;

typedef char           vbool;
typedef unsigned char  vbyte;
typedef long           vdate;
typedef float          float32;
typedef double         float64;

#elif defined(__linux__)

typedef signed char    int8;
typedef short          int16;
typedef int            int32;
typedef long           int64;

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;
typedef unsigned long  uint64;

typedef unsigned char  size8;
typedef unsigned short size16;
typedef unsigned int   size32;
typedef unsigned long  size64;

typedef char           vbool;
typedef unsigned char  vbyte;
typedef long           vdate;
typedef float          float32;
typedef double         float64;

#else // Windows

typedef __int8            int8;
typedef __int16           int16;
typedef __int32           int32;
typedef __int64           int64;

typedef unsigned __int8   uint8;
typedef unsigned __int16  uint16;
typedef unsigned __int32  uint32;
typedef unsigned __int64  uint64;

typedef unsigned __int8   size8;
typedef unsigned __int16  size16;
typedef unsigned __int32  size32;
typedef unsigned __int64  size64;

typedef __int8            vbool;
typedef unsigned char     vbyte;
typedef __int64           vdate;
typedef float             float32;
typedef double            float64;

#endif

typedef int8  offset8;
typedef int16 offset16;
typedef int32 offset32;
typedef int64 offset64;

/* apple, linux default to x64, windows maybe x32 or x64 */
#if defined(WIN32) && !defined(WIN64)

typedef int32     offset_t;
typedef offset_t  nodeoff;
typedef int32     vhandle;
typedef size32    vsize_t;

#else

typedef int64     offset_t;
typedef offset_t  nodeoff;
typedef int64     vhandle;
typedef size64    vsize_t;

#endif


//基类类型值范围
static const int8  kMinInt8 = -128;
static const int8  kMaxInt8 = 127;
static const uint8 kMaxUInt8 = 255U;

static const int16  kMinInt16 = -32768;
static const int16  kMaxInt16 = 32767;
static const uint16 kMaxUInt16 = 65535U;

static const int32  kMinInt32 = -2147483647 - 1;
static const int32  kMaxInt32 = 2147483647;
static const uint32 kMaxUInt32 = 4294967295U;

static const int64  kMinInt64 = -9223372036854775807LL - 1;
static const int64  kMaxInt64 = 9223372036854775807LL;
static const uint64 kMaxUInt64 = 18446744073709551615ULL;

