//
// Created by dejavu on 2023/6/30.
//

#ifndef QUERY_MAC_ADDRESS_H_
#define QUERY_MAC_ADDRESS_H_

/* ----------------------------------------------------------
�ļ����ƣ�QueryMacAddress.h

����������
	ͨ��WDK��ȡ����ԭ��MAC��ַ�͵�ǰMAC��ַ��

�ӿں�����
	QueryMacAddress
------------------------------------------------------------ */
#pragma once

#include <windows.h>

#define MAC_ADDRESS_BYTE_LEN		6	// MAC��ַ�ֽڳ���

typedef struct _T_MAC_ADDRESS {
  BYTE	PermanentAddress[MAC_ADDRESS_BYTE_LEN];	// ԭ��MAC��ַ
  BYTE	CurrentAddress[MAC_ADDRESS_BYTE_LEN];			// ��ǰMAC��ַ
} T_MAC_ADDRESS;

#ifdef __cplusplus
extern "C"
{
#endif

/*
���ܣ�ͨ��WDK��ȡ����ԭ��MAC��ַ�͵�ǰMAC��ַ
��ڲ�����
	query_type����Ҫ��ȡ����������
			0������USB����
			1��������USB����
	mac_address_list���洢����MAC��ַ
	uSize���ɴ洢�����������Ŀ
����ֵ��
	 -1����ȡ�豸��Ϣ��ʧ��
	 -2����֧�ֵĲ�ѯ����
	>=0��ʵ�ʻ�ȡ���豸��
*/
INT QueryMacAddress( INT query_type, T_MAC_ADDRESS *mac_address_list, INT list_size );

#ifdef __cplusplus
}
#endif

#endif //QUERY_MAC_ADDRESS_H_

