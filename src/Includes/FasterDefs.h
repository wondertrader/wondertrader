#pragma once

#include "WTSMarcos.h"

#include "../FasterLibs/tsl/robin_map.h"
#include "../FasterLibs/tsl/robin_set.h"

NS_WTP_BEGIN

#define faster_hashmap tsl::robin_map
#define faster_hashset tsl::robin_set

NS_WTP_END