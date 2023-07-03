//
// Created by dejavu on 2023/6/30.
//

#ifndef QUERY_MAC_ADDRESS_H_
#define QUERY_MAC_ADDRESS_H_

/* ----------------------------------------------------------
文件名称：QueryMacAddress.h

功能描述：
	通过WDK获取网卡原生MAC地址和当前MAC地址。

接口函数：
	QueryMacAddress
------------------------------------------------------------ */
#pragma once

#include <windows.h>

#define MAC_ADDRESS_BYTE_LEN		6	// MAC地址字节长度

typedef struct _T_MAC_ADDRESS {
  BYTE	PermanentAddress[MAC_ADDRESS_BYTE_LEN];	// 原生MAC地址
  BYTE	CurrentAddress[MAC_ADDRESS_BYTE_LEN];			// 当前MAC地址
} T_MAC_ADDRESS;

#ifdef __cplusplus
extern "C"
{
#endif

/*
功能：通过WDK获取网卡原生MAC地址和当前MAC地址
入口参数：
	query_type：需要获取的网卡类型
			0：包括USB网卡
			1：不包括USB网卡
	mac_address_list：存储网卡MAC地址
	uSize：可存储的最大网卡数目
返回值：
	 -1：获取设备信息表失败
	 -2：不支持的查询类型
	>=0：实际获取的设备数
*/
INT QueryMacAddress( INT query_type, T_MAC_ADDRESS *mac_address_list, INT list_size );

#ifdef __cplusplus
}
#endif

#endif //QUERY_MAC_ADDRESS_H_

