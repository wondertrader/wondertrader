#pragma once

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
 */
#include <string>


typedef unsigned int uint32;
class MD5Sum
{
public:
	MD5Sum();
	MD5Sum(const std::string& sSource);
	MD5Sum(const unsigned char* pachSource, uint32 nLen);

	std::string Calculate(const std::string& sSource);
	std::string Calculate(const unsigned char* pachSource, uint32 nLen);

	std::string GetHash() const;
	const unsigned char* GetRawHash() const { return m_rawHash; }

private:
	std::string			m_sHash;
	unsigned char	m_rawHash[16];
};
