/*!
 * \file PacketDefine.h
 * \project	WonderTrader
 *
 * \author Wesley
 * \date 2020/03/30
 * 
 * \brief WT系统网络通讯包的通用包头部定义
 */
#pragma once

#pragma pack(push ,1)

#pragma warning(disable:4200)

#define PACKETFLAG_PB	0xffff
#define PACKETFLAG_JSON	0xfffe

struct PacketHead
{
	unsigned short	m_uFlags;
	unsigned short	m_uType;
	unsigned int	m_nLength;
	unsigned int	m_nOrgLen;
	unsigned int	m_nReqID;
	char			m_cData[0];
};

#pragma pack(pop)