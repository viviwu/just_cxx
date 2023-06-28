//
// Created by viviwu on 2023/6/28.
//
#include <windows.h>
#include <iostream>
#include <comdef.h>
#include <Wbemidl.h>
#pragma comment(lib, "wbemuuid.lib")

std::string BSTRToMultiByteString(BSTR &adapterType ){
    int adapterTypeLen = SysStringLen(adapterType);
    std::cout<<"AdapterType.bstrVal StringLen: "<< adapterTypeLen << std::endl;
    // 输出BSTR对象的16进制值
//    unsigned char* adapterTypeBytes = reinterpret_cast<unsigned char*>(adapterType);
//        for (int i = 0; i < adapterTypeLen * 2; i++){
//            std::printf("%02X ", adapterTypeBytes[i]);
//            std::printf("%c ", adapterTypeBytes[i]);
//        }
//        std::printf("\n");

    std::wstring adapterTypeStr(adapterType, adapterTypeLen);
//        std::wcout << L"AdapterType: " << adapterTypeStr << std::endl;
// 将Unicode字符串转换为ANSI字符串
    int ansiLen = WideCharToMultiByte(CP_ACP, 0, adapterTypeStr.c_str(), adapterTypeStr.length(), nullptr, 0, nullptr, nullptr);
    BSTR ansiStr = ::SysAllocStringByteLen(nullptr, ansiLen);
    WideCharToMultiByte(CP_ACP, 0, adapterTypeStr.c_str(), adapterTypeStr.length(), reinterpret_cast<char*>(ansiStr), ansiLen, nullptr, nullptr);

    std::ios::sync_with_stdio(false);
    std::cout.imbue(std::locale(""));

// 输出新的BSTR对象
//    std::cout << reinterpret_cast<char*>(ansiStr) << std::endl;
    std::string std_str(reinterpret_cast<char*>(ansiStr));

  // 释放新的BSTR对象
    ::SysFreeString(ansiStr);
    return std_str;
}


int main()
{
    HRESULT hres;

    // 初始化 COM 库
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres))
    {
        std::cerr << "Failed to initialize COM library. Error code: " << hres << std::endl;
        return 1;
    }

    // 初始化 WMI
    hres = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    );
    if (FAILED(hres))
    {
        std::cerr << "Failed to initialize security. Error code: " << hres << std::endl;
        CoUninitialize();
        return 1;
    }

    // 创建 WMI 服务管理器
    IWbemLocator* pLoc = NULL;
    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (LPVOID*)&pLoc
    );
    if (FAILED(hres))
    {
        std::cerr << "Failed to create IWbemLocator object. Error code: " << hres << std::endl;
        CoUninitialize();
        return 1;
    }

    // 连接 WMI 服务
    IWbemServices* pSvc = NULL;
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),  // WMI namespace
        NULL,     // User name
        NULL,     // User password
        0,        // Locale
        NULL,     // Security flags
        0,        // Authority
        0,        // Context object
        &pSvc     // IWbemServices proxy
    );
    if (FAILED(hres))
    {
        std::cerr << "Failed to connect to WMI service. Error code: " << hres << std::endl;
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    // 设置安全级别
    hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL,
                           RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
    if (FAILED(hres)) {
        std::cout << "Failed to set security level. Error code: " << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    // 设置 WMI 查询语句
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        _bstr_t("WQL"),
        _bstr_t("SELECT * FROM Win32_NetworkAdapter WHERE PhysicalAdapter = TRUE"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator
    );//  AdapterTypeId=0 AND MACAddress IS NOT NULL

    if (FAILED(hres))
    {
        std::cerr << "Failed to execute WMI query. Error code: " << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    // 遍历查询结果，获取网卡信息
    IWbemClassObject* pClsObj = NULL;
    ULONG uReturn = 0;
    while (pEnumerator)
    {
        hres = pEnumerator->Next(WBEM_INFINITE, 1, &pClsObj, &uReturn);
        if (0 == uReturn || FAILED(hres)) break;

        // Get the value of the Name property
//        VARIANT vtProp;
//        hres = pClsObj->Get(L"DeviceId", 0, &vtProp, 0, 0);
//        std::wcout << L"DeviceId: " << vtProp.bstrVal << std::endl;
//        VariantClear(&vtProp);
        // 获取 MAC 地址
        VARIANT vtMac;
        hres = pClsObj->Get(L"MACAddress", 0, &vtMac, 0, 0);

        // 获取网卡名称
        VARIANT vtName;
        hres = pClsObj->Get(L"Name", 0, &vtName, 0, 0);

        // 获取网卡描述
        VARIANT vtDesc;
        hres = pClsObj->Get(L"Description", 0, &vtDesc, 0, 0);

        // 获取网卡厂商
        VARIANT vtManufac;
        hres = pClsObj->Get(L"Manufacturer", 0, &vtManufac, 0, 0);

        // 获取网卡类型
        VARIANT vtType;
        hres = pClsObj->Get(L"AdapterType", 0, &vtType, 0, 0);
        // 判断网卡类型是否为物理网卡
        bool isPhysical = false;
        if (SUCCEEDED(hres) && vtType.vt == VT_BSTR && vtType.bstrVal != NULL)
        {
            int str_len = SysStringLen(vtType.bstrVal);
            std::wcout << L"Adapter Type Size: " << str_len << std::endl;
            std::cout << L"AdapterType: " << BSTRToMultiByteString(vtType.bstrVal) << std::endl;

            // "Ethernet 802.3"  "Wireless 802.11"
            if(wcscmp(vtType.bstrVal, L"802.3") == 0 ||
               wcscmp(vtType.bstrVal, L"802.11") == 0)
            {
                isPhysical = true;
            }
        }
        isPhysical = true;

        // 输出网卡信息
        if (SUCCEEDED(hres) && vtMac.vt == VT_BSTR && vtMac.bstrVal != NULL && isPhysical)
        {
            std::wcout << L"Name: " << vtName.bstrVal << std::endl;
            std::wcout << L"Description: " << vtDesc.bstrVal << std::endl;
            std::wcout << L"Manufacturer: " << vtManufac.bstrVal << std::endl;
            std::wcout << L"MAC Address: " << vtMac.bstrVal << std::endl;

        }

        // 释放资源
        VariantClear(&vtMac);
        VariantClear(&vtName);
        VariantClear(&vtDesc);
        VariantClear(&vtManufac);
        VariantClear(&vtType);
        pClsObj->Release();
    }

    // 释放资源
    pEnumerator->Release();
    pSvc->Release();
    pLoc->Release();
    CoUninitialize();

    return 0;
}