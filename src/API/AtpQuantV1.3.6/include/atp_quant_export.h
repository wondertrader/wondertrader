#ifndef _ATP_QUANT_EXPORT_H_
#define _ATP_QUANT_EXPORT_H_

#if defined _WIN32	
#      define  QUANT_API __declspec(dllimport)
#else 
#      define  QUANT_API  __attribute__((visibility("default")))
#endif

#endif  //#ifndef _ATP_QUANT_EXPORT_H_