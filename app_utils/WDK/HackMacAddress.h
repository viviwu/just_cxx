//
// Created by dejavu on 2023/6/30.
//

#ifndef JUST_CXX_APP_UTILS_HACKMACADDRESS_H_
#define JUST_CXX_APP_UTILS_HACKMACADDRESS_H_

//class HackMacAddress { };
/* ----------------------------------------------------------
文件名称：HackMacAddress.h

作者：秦建辉

MSN：splashcn@msn.com

版本历史：
	V1.0	2010年05月23日
			完成正式版本。

功能描述：
	通过WDK获取网卡原生MAC地址和当前MAC地址。

接口函数：
	HackMacAddress
------------------------------------------------------------ */
#pragma once

#include <windows.h>

#define MACADDRESS_BYTELEN		6	// MAC地址字节长度

typedef struct _T_MAC_ADDRESS {
  BYTE	PermanentAddress[MACADDRESS_BYTELEN];	// 原生MAC地址
  BYTE	CurrentAddress[MACADDRESS_BYTELEN];			// 当前MAC地址
} T_MAC_ADDRESS;

#ifdef __cplusplus
extern "C"
{
#endif

/*
功能：通过WDK获取网卡原生MAC地址和当前MAC地址
入口参数：
	iQueryType：需要获取的网卡类型
			0：包括USB网卡
			1：不包括USB网卡
	pMacAddress：存储网卡MAC地址
	uSize：可存储的最大网卡数目
返回值：
	 -1：获取设备信息表失败
	 -2：不支持的查询类型
	>=0：实际获取的设备数
*/
INT HackMacAddress( INT iQueryType, T_MAC_ADDRESS *pMacAddress, INT iSize );

#ifdef __cplusplus
}
#endif

#endif //JUST_CXX_APP_UTILS_HACKMACADDRESS_H_

