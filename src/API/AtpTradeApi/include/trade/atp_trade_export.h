#pragma once

#if defined _WIN32
#   if    defined TRADE_API_USE_STATIC
#      define  TRADE_API  
#   elif  defined TRADE_API_BUILD_EXPORT	
#      define  TRADE_API __declspec(dllexport)
#   else
#      define  TRADE_API __declspec(dllimport)
#   endif
#else 
#      define  TRADE_API  __attribute__((visibility("default")))
#endif
