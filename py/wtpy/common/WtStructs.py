from ctypes import Structure, c_char, c_int32, c_uint16, c_uint32, c_uint64, c_double

MAX_INSTRUMENT_LENGTH = c_char*32
MAX_EXCHANGE_LENGTH = c_char*10
PriceQueueType = c_double*10
VolumnQueueType = c_uint32*10


class WTSTickStruct(Structure):
    _fields_ = [("exchg", MAX_EXCHANGE_LENGTH),
                ("code", MAX_INSTRUMENT_LENGTH),
                ("price", c_double),
                ("open", c_double),
                ("high", c_double),
                ("low", c_double),
                ("settle_price", c_double),

                ("upper_limit", c_double),
                ("lower_limit", c_double),

                ("total_volumn", c_uint32),
                ("volumn", c_uint32),
                ("total_turnover", c_double),
                ("turn_over", c_double),
                ("open_interest", c_uint32),
                ("diff_interest", c_int32),

                ("trading_date", c_uint32),
                ("action_date", c_uint32),
                ("action_time", c_uint32),

                ("pre_close", c_double),
                ("pre_settle", c_double),
                ("pre_interest", c_uint32),

                ("bid_prices", PriceQueueType),
                ("ask_prices", PriceQueueType),
                ("bid_qty", VolumnQueueType),
                ("ask_qty", VolumnQueueType)]
    _pack_ = 1


class WTSBarStruct(Structure):
    _fields_ = [("date", c_uint32),
                ("time", c_uint32),
                ("open", c_double),
                ("high", c_double),
                ("low", c_double),
                ("close", c_double),
                ("settle", c_double),
                ("money", c_double),
                ("vol", c_uint32),
                ("hold", c_uint32),
                ("diff", c_int32)]
    _pack_ = 1
