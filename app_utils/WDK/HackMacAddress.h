//
// Created by dejavu on 2023/6/30.
//

#ifndef JUST_CXX_APP_UTILS_HACKMACADDRESS_H_
#define JUST_CXX_APP_UTILS_HACKMACADDRESS_H_

//class HackMacAddress { };
/* ----------------------------------------------------------
�ļ����ƣ�HackMacAddress.h

���ߣ��ؽ���

MSN��splashcn@msn.com

�汾��ʷ��
	V1.0	2010��05��23��
			�����ʽ�汾��

����������
	ͨ��WDK��ȡ����ԭ��MAC��ַ�͵�ǰMAC��ַ��

�ӿں�����
	HackMacAddress
------------------------------------------------------------ */
#pragma once

#include <windows.h>

#define MACADDRESS_BYTELEN		6	// MAC��ַ�ֽڳ���

typedef struct _T_MAC_ADDRESS {
  BYTE	PermanentAddress[MACADDRESS_BYTELEN];	// ԭ��MAC��ַ
  BYTE	CurrentAddress[MACADDRESS_BYTELEN];			// ��ǰMAC��ַ
} T_MAC_ADDRESS;

#ifdef __cplusplus
extern "C"
{
#endif

/*
���ܣ�ͨ��WDK��ȡ����ԭ��MAC��ַ�͵�ǰMAC��ַ
��ڲ�����
	iQueryType����Ҫ��ȡ����������
			0������USB����
			1��������USB����
	pMacAddress���洢����MAC��ַ
	uSize���ɴ洢�����������Ŀ
����ֵ��
	 -1����ȡ�豸��Ϣ��ʧ��
	 -2����֧�ֵĲ�ѯ����
	>=0��ʵ�ʻ�ȡ���豸��
*/
INT HackMacAddress( INT iQueryType, T_MAC_ADDRESS *pMacAddress, INT iSize );

#ifdef __cplusplus
}
#endif

#endif //JUST_CXX_APP_UTILS_HACKMACADDRESS_H_

