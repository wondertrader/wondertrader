#pragma once
#include "../Includes/WTSMarcos.h"

typedef void(PORTER_FLAG *FuncMQCallback)(WtUInt32 id, const char* topic, const char* data, WtUInt32 dataLen);
typedef void(PORTER_FLAG *FuncLogCallback)(WtUInt32 id, const char* message, bool bServer);