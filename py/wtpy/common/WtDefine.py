from ctypes import c_uint, c_void_p, CFUNCTYPE, POINTER, c_char_p, c_bool, c_ulong
from .WtStructs import  WTSBarStruct, WTSTickStruct

# 回调函数定义
CB_STRATEGY_INIT = CFUNCTYPE(c_void_p, c_ulong)
CB_STRATEGY_TICK = CFUNCTYPE(c_void_p, c_ulong, c_char_p, POINTER(WTSTickStruct))
CB_STRATEGY_CALC = CFUNCTYPE(c_void_p, c_ulong)
CB_STRATEGY_BAR = CFUNCTYPE(c_void_p, c_ulong, c_char_p, c_char_p, POINTER(WTSBarStruct))
CB_STRATEGY_GET_BAR = CFUNCTYPE(c_void_p, c_ulong, c_char_p, c_char_p, POINTER(WTSBarStruct), c_bool)
CB_STRATEGY_GET_TICK = CFUNCTYPE(c_void_p, c_ulong, c_char_p, POINTER(WTSTickStruct), c_bool)
CB_ENGINE_EVENT = CFUNCTYPE(c_void_p, c_ulong, c_ulong, c_ulong)
