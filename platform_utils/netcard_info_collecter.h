//
// Created by viviwu on 2023/7/4.
//

#ifndef JUST_CXX_NETCARD_INFO_COLLECTER_H
#define JUST_CXX_NETCARD_INFO_COLLECTER_H
#include <windows.h>
#include <setupapi.h>
#include <map>

class NetcardInfoCollecter {
public:
    NetcardInfoCollecter();

    ~NetcardInfoCollecter();

    BOOL GetAllPhysicsNetCardName();
    void GetNetCardDes();
    BYTE* NetcardInfoCollecter::MySetupDiGetDeviceRegistryProperty(
        IN DWORD  Property,
        OUT PDWORD  PropertyRegDataType ,
        OUT PDWORD  pdwBufSize);

    int m_NetCardNum;
    HDEVINFO hdev_info;
    SP_DEVINFO_DATA m_NetCardDevInfoData;
//    PSP_DEVICE_INTERFACE_DETAIL_DATA
    std::map<int, BYTE*> m_NetCardDesMap;
};

#endif // JUST_CXX_NETCARD_INFO_COLLECTER_H
