//
// Created by viviwu on 2023/6/28.
//
#include <Wbemidl.h>
#include <comdef.h>
#include <iostream>
#include <string>
#include <windows.h>

#pragma comment(lib, "wbemuuid.lib")

// Convert a BSTR to a std::string
std::string bstrToString(BSTR bstr) {
    std::wstring wstr(bstr);
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    std::string str(size, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &str[0], size, NULL, NULL);
    return str;
}

// Convert a BSTR to a char*
char* bstrToChar(BSTR bstr) {
    std::wstring wstr(bstr);
    int size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
    char* str = new char[size];
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, str, size, NULL, NULL);
    return str;
}

int main() {
    HRESULT hres;

    // ��ʼ�� COM ��
    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        std::cerr << "Failed to initialize COM library. Error code: " << hres << std::endl;
        return 1;
    }

    // ��ʼ�� WMI
    hres =
        CoInitializeSecurity(NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_DEFAULT, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE, NULL);
    if (FAILED(hres)) {
        std::cerr << "Failed to initialize security. Error code: " << hres << std::endl;
        CoUninitialize();
        return 1;
    }

    // ���� WMI �������
    IWbemLocator* pLoc = NULL;
    hres = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLoc);
    if (FAILED(hres)) {
        std::cerr << "Failed to create IWbemLocator object. Error code: " << hres << std::endl;
        CoUninitialize();
        return 1;
    }

    // ���� WMI ����
    IWbemServices* pSvc = NULL;
    hres = pLoc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), // WMI namespace
                               NULL,                    // User name
                               NULL,                    // User password
                               0,                       // Locale
                               NULL,                    // Security flags
                               0,                       // Authority
                               0,                       // Context object
                               &pSvc                    // IWbemServices proxy
    );
    if (FAILED(hres)) {
        std::cerr << "Failed to connect to WMI service. Error code: " << hres << std::endl;
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    // ���ð�ȫ����
    hres = CoSetProxyBlanket(pSvc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE,
                             NULL, EOAC_NONE);
    if (FAILED(hres)) {
        std::cerr << "Failed to set security blanket. Error code = 0x" << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    // ���� WMI ��ѯ���
    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        _bstr_t("WQL"), _bstr_t("SELECT * FROM Win32_NetworkAdapter"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator); // WHERE AdapterTypeId=0 AND MACAddress IS NOT NULL \ NetConnectionStatus=2\ WHERE PhysicalAdapter=TRUE

    if (FAILED(hres)) {
        std::cerr << "Failed to execute WMI query. Error code = 0x" << std::hex << hres << std::endl;
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return 1;
    }

    // ������ѯ�������ȡ������Ϣ
    IWbemClassObject* pClsObj = NULL;
    ULONG uReturn = 0;
    while (pEnumerator) {
        hres = pEnumerator->Next(WBEM_INFINITE, 1, &pClsObj, &uReturn);
        if (0 == uReturn || FAILED(hres))
            break;
        // Get the value of the Name property :PNPDeviceIDNameManufacturer
        // NetConnectionStatusSpeedMACAddressAdapterTypePhysicalAdapterNetEnabled

        // ��ȡ������ PNPDeviceID ����
        VARIANT vtPNPDeviceID;
        hres = pClsObj->Get(L"PNPDeviceID", 0, &vtPNPDeviceID, 0, 0);
        if (FAILED(hres)) {
            std::cerr << "Failed to get PNPDeviceID property. Error code = 0x" << std::hex << hres << std::endl;
            pClsObj->Release();
            continue;
        }

        // ��ȡ MAC ��ַ
        VARIANT vtMac;
        hres = pClsObj->Get(L"MACAddress", 0, &vtMac, 0, 0);

        // ��ȡ��������
        VARIANT vtName;
        hres = pClsObj->Get(L"Name", 0, &vtName, 0, 0);

        // ��ȡ��������
        VARIANT vtDesc;
        hres = pClsObj->Get(L"Description", 0, &vtDesc, 0, 0);

        // ��ȡ��������
        VARIANT vtManufac;
        hres = pClsObj->Get(L"Manufacturer", 0, &vtManufac, 0, 0);

        // �ж������Ƿ�Ϊ��������
        /*
       VARIANT vtPhysicalAdapter;
       VariantInit(&vtPhysicalAdapter);
       hres = pClsObj->Get(bstr_t(L"PhysicalAdapter"), 0, &vtPhysicalAdapter, NULL, NULL);
       if (SUCCEEDED(hres) && vtPhysicalAdapter.vt == VT_BOOL) {
         if (vtPhysicalAdapter.boolVal == VARIANT_TRUE) {
           std::wcout << L"Physical network adapter found:" << std::endl;
         } else {
           std::wcout << L"Virtual network adapter found:" << std::endl;
   //        continue;
         }
       }
      */
        // PCI-SIG��Peripheral Component Interconnect Special Interest Group��
        if (vtPNPDeviceID.vt == VT_BSTR) {
            std::wstring strPNPDeviceID = vtPNPDeviceID.bstrVal;
            if (strPNPDeviceID.find(L"PCI\\VEN_") != std::wstring::npos) {
                std::wcout << L"Hardware network adapter found:" << std::endl;
            } else {
                std::wcout << L"Virtual network adapter found:" << std::endl;
                // continue;
            }
        }

        // ���������Ϣ
        if (SUCCEEDED(hres) && vtMac.vt == VT_BSTR && vtMac.bstrVal != NULL) {
            std::wcout << L"Name: " << vtName.bstrVal << std::endl;
            std::wcout << L"Description: " << vtDesc.bstrVal << std::endl;
            std::wcout << L"Manufacturer: " << vtManufac.bstrVal << std::endl;
            std::wcout << L"MAC Address: " << vtMac.bstrVal << std::endl;
            std::wcout << "--------------------------" << std::endl;
        }

        // �ͷ���Դ
        //    VariantClear(&vtPhysicalAdapter);
        VariantClear(&vtPNPDeviceID);
        VariantClear(&vtMac);
        VariantClear(&vtName);
        VariantClear(&vtDesc);
        VariantClear(&vtManufac);
        pClsObj->Release();
    }

    // �ͷ���Դ
    pEnumerator->Release();
    pSvc->Release();
    pLoc->Release();
    CoUninitialize();

    getchar();
    return 0;
}
