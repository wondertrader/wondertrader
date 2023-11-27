/*!
 * \file WTSCmpHelper.hpp
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief 数据压缩辅助类,利用zstdlib压缩
 */
#pragma once
#include <string>
#include <stdint.h>

#include "../WTSUtils/zstdlib/zstd.h"

class WTSCmpHelper
{
public:
	static std::string compress_data(const void* data, size_t dataLen, uint32_t uLevel = 1)
	{
		std::string desBuf;
		std::size_t const desLen = ZSTD_compressBound(dataLen);
		desBuf.resize(desLen, 0);
		size_t const cSize = ZSTD_compress((void*)desBuf.data(), desLen, data, dataLen, uLevel);
		desBuf.resize(cSize);
		return desBuf;
	}

	static std::string uncompress_data(const void* data, size_t dataLen)
	{
		std::string desBuf;
		unsigned long long const desLen = ZSTD_getFrameContentSize(data, dataLen);
		desBuf.resize((std::size_t)desLen, 0);
		size_t const dSize = ZSTD_decompress((void*)desBuf.data(), (size_t)desLen, data, dataLen);
		if (dSize != desLen)
			throw std::runtime_error("uncompressed data size does not match calculated data size");
		return desBuf;
	}
};

