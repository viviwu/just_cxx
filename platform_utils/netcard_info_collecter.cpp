//
// Created by viviwu on 2023/7/4.
//

#include "netcard_info_collecter.h"

#include <tchar.h>
#include <strsafe.h>
#include <corecrt_wstring.h>

#include <ntddndis.h>
#include <hidsdi.h>
#include <iostream>
#include <xstring>

#pragma comment (lib, "Setupapi.lib")
#pragma comment (lib, "hid.lib")

NetcardInfoCollecter::NetcardInfoCollecter(){
    std::cout<<__FUNCTION__ <<std::endl;
}

NetcardInfoCollecter::~NetcardInfoCollecter(){
    std::cout<<__FUNCTION__ <<std::endl;
}

BOOL NetcardInfoCollecter::GetAllPhysicsNetCardName()
{
    BOOL        bRet = TRUE;
    GUID  NetCardClassGuid   =
        {0X4D36E972,   0XE325,   0X11CE,   {0XBF,   0XC1,   0X08,   0X00,   0X2B,   0XE1,   0X03,   0X18}};

    //得到一个Class下信息集句柄 
    hdev_info = SetupDiGetClassDevs(&NetCardClassGuid, NULL, NULL, DIGCF_PRESENT);
    if(hdev_info == INVALID_HANDLE_VALUE)
    {
        std::wcout<<_T("NewDeviceInfoSet == INVALID_HANDLE_VALUE")<<std::endl;

        bRet = FALSE;
        return bRet;
    }

    //对得到的此class信息集进行枚举 
    for(int Device_Index=0; ; Device_Index++)
    {
        memset((void*)&(m_NetCardDevInfoData), 0, sizeof(SP_DEVINFO_DATA));
        m_NetCardDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
        BOOL        bResult  = SetupDiEnumDeviceInfo(hdev_info, Device_Index, &m_NetCardDevInfoData);
        if(bResult == 0)
        {
            if(GetLastError() == ERROR_NO_MORE_ITEMS)
            {
                break;
            }
        }

        DWORD   dwBufSize = 0;
        BYTE    *pbReuslt = MySetupDiGetDeviceRegistryProperty(
            SPDRP_ENUMERATOR_NAME ,
            NULL,&dwBufSize);
        if(pbReuslt)
        {
            TCHAR *tcName = (TCHAR*)pbReuslt;
            //_wcsupr_s(tcName,dwBufSize);
            tcName = CharUpper(tcName);
            // if(wcscmp(tcName,_T("ROOT")) != 0)
            if(strcmp(tcName, "ROOT")!= 0)
            {
                m_NetCardNum++;
                GetNetCardDes();
            }

            free(pbReuslt);
        }
    }
    //销毁一个Class的device information set 
    SetupDiDestroyDeviceInfoList(hdev_info);
    return bRet;
}

void NetcardInfoCollecter::GetNetCardDes()
{
    DWORD   dwBufSize = 0;
    BYTE     *pbTemp =  MySetupDiGetDeviceRegistryProperty(
        SPDRP_DEVICEDESC,
        NULL,&dwBufSize);
    if(pbTemp)
    {
        BYTE     *pbNetDes = new BYTE[dwBufSize];
        memset(pbNetDes,0x00,dwBufSize);
        memcpy(pbNetDes,pbTemp,dwBufSize);
        m_NetCardDesMap.insert(std::make_pair(m_NetCardNum,pbNetDes));
        free(pbTemp);
    }
}


BYTE* NetcardInfoCollecter::MySetupDiGetDeviceRegistryProperty(
    IN DWORD  Property,
    OUT PDWORD  PropertyRegDataType ,
    OUT PDWORD  pdwBufSize)
{
    DWORD       Required_Size = 0;
    BOOL bResult = SetupDiGetDeviceRegistryProperty(
        hdev_info,
        &m_NetCardDevInfoData,
        Property ,
        PropertyRegDataType,
        NULL, 0,
        &Required_Size);
    if(!bResult)
    {
        if(ERROR_INSUFFICIENT_BUFFER == GetLastError())
        {
            *pdwBufSize = Required_Size;
            BYTE        *dev_property = (BYTE*)malloc(Required_Size);

            bResult = SetupDiGetDeviceRegistryProperty(
                hdev_info,
                &m_NetCardDevInfoData,
                Property ,
                PropertyRegDataType,
                dev_property,
                Required_Size, NULL);
            if(bResult)
                return dev_property;
            else
            {
                free(dev_property);
                return NULL;
            }
        }
        else
            return NULL;
    }
    else
        return NULL;
} 
 
int main(){
    NetcardInfoCollecter dev_info;
    dev_info.GetAllPhysicsNetCardName();
    getchar();
    return 0;
}