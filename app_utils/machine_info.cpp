//
// Created by vivi wu on 6/27/23.
//
#ifdef _WIN32
// Windows-specific code
#include <winsock2.h>  // forward declaration
#include <iphlpapi.h>
#include <windows.h>
#pragma comment(lib, "iphlpapi.lib")

/*******WMI*********/
#include <comdef.h>
#include <Wbemidl.h>

/*******WMI*********/
#pragma comment(lib, "wbemuuid.lib")

#elif __APPLE__
// macOS-specific code
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/utsname.h> # os
#include <sys/sysctl.h>  # CPU info
#include <sys/mount.h> # Disk file info

#endif

#include <iostream>
#include <string>

#ifdef _WIN32
// Windows-specific code


void infoOfNetworkInterfaceCard() {
    std::cout <<"**************************"<< __func__ <<"**************************"<< std::endl;

    // Get the size of the buffer needed for the adapter information
    ULONG ulBufferSize = 0;
    GetAdaptersInfo(NULL, &ulBufferSize);

    // Allocate memory for the adapter information buffer
    PIP_ADAPTER_INFO pAdapterInfo = new IP_ADAPTER_INFO[ulBufferSize];

    // Get the adapter information
    if (GetAdaptersInfo(pAdapterInfo, &ulBufferSize) == NO_ERROR) {
        // Loop through the list of adapters
        PIP_ADAPTER_INFO pAdapter = pAdapterInfo;
        while (pAdapter) {
          std::cout <<"##############"<< std::endl;
            // Check if the adapter name contains "virtual", "vmware", or "vbox"
            std::string adapterName(pAdapter->Description);
            if (adapterName.find("Virtual") != std::string::npos ||
                adapterName.find("Vmware") != std::string::npos ||
                adapterName.find("Vbox") != std::string::npos ) {
                std::cout << "VirtualAdapter: " << adapterName << std::endl;
            } else {
                // Print out the adapter information
                std::cout << "ComboIndex: " << pAdapter->ComboIndex << std::endl;
                std::cout << "Adapter Name: " << pAdapter->AdapterName << std::endl;
                std::cout << "Adapter Description: " << pAdapter->Description << std::endl;
                std::cout << "IP Address: " << pAdapter->IpAddressList.IpAddress.String << std::endl;
                std::cout << "Subnet Mask: " << pAdapter->IpAddressList.IpMask.String << std::endl;
                std::cout << "Default Gateway: " << pAdapter->GatewayList.IpAddress.String << std::endl;
                std::cout << "Adapter Type: " << pAdapter->Type  << pAdapter->HaveWins << std::endl;
                std::cout << "HaveWinServer: " << pAdapter->HaveWins << std::endl;
                std::cout << "Adapter Addr: ";
                for (int i = 0; i < pAdapter->AddressLength; i++) {
                    if (i == (pAdapter->AddressLength - 1))
                        printf("%.2X\n", (int)pAdapter->Address[i]);
                    else
                        printf("%.2X-", (int)pAdapter->Address[i]);
                }
            }

            // Move on to the next adapter
            pAdapter = pAdapter->Next;
        }
    }

    // Free the adapter information buffer
    delete[] pAdapterInfo;
    
}


void infoOfOperationSystemVersion() {
    std::cout <<"**************************"<< __func__ <<"**************************"<< std::endl;

    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    std::cout << "Windows version: " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion << std::endl;
    std::cout << "Windows PlatformId: " << osvi.dwPlatformId  << std::endl;
    std::cout << "Windows dwBuildNumber: " << osvi.dwBuildNumber  << std::endl;
}


void infoOfCPU() {
    std::cout <<"**************************"<< __func__ <<"**************************"<< std::endl;

    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    std::cout << "processors dwOemId: " << sysInfo.dwOemId << std::endl;
    std::cout << "Number of processors: " << sysInfo.dwNumberOfProcessors << std::endl;
    std::cout << "Processor type: " << sysInfo.dwProcessorType << std::endl;
    std::cout << "Processor architecture: ";

    switch (sysInfo.wProcessorArchitecture)
    {
        case PROCESSOR_ARCHITECTURE_AMD64:
            std::cout << "x64" << std::endl;
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            std::cout << "ARM" << std::endl;
            break;
        case PROCESSOR_ARCHITECTURE_IA64:
            std::cout << "Intel Itanium-based" << std::endl;
            break;
        case PROCESSOR_ARCHITECTURE_INTEL:
            std::cout << "x86" << std::endl;
            break;
        default:
            std::cout << "Unknown" << std::endl;
            break;
    }

}


void infoOfDisk() {
     std::cout <<"**************************"<< __func__ <<"**************************"<< std::endl;

    ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;
    GetDiskFreeSpaceEx("C:", &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes);
    std::cout << "Free space: " << freeBytesAvailable.QuadPart << std::endl;
    std::cout << "Total space: " << totalNumberOfBytes.QuadPart << std::endl;
}

int infoOfWMI() {
    std::cout <<"**************************"<< __func__ <<"**************************"<< std::endl;

    HRESULT hr;
    IWbemLocator* pLocator = NULL;
    IWbemServices* pService = NULL;
    IEnumWbemClassObject* pEnum = NULL;
    IWbemClassObject* pObj = NULL;
    VARIANT var;

    // Initialize COM
    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr)) {
        std::cerr << "Error: CoInitializeEx failed" << std::endl;
        return -1;
    }

    // Create a WMI locator
    hr = CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, (LPVOID*)&pLocator);
    if (FAILED(hr)) {
        std::cerr << "Error: CoCreateInstance failed" << std::endl;
        CoUninitialize();
        return -1;
    }

    // Connect to the WMI service
    hr = pLocator->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &pService);
    if (FAILED(hr)) {
        std::cerr << "Error: ConnectServer failed" << std::endl;
        pLocator->Release();
        CoUninitialize();
        return -1;
    }

    // Set the security
    hr = CoSetProxyBlanket(pService, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);
    if (FAILED(hr)) {
        std::cerr << "Error: CoSetProxyBlanket failed" << std::endl;
        pService->Release();
        pLocator->Release();
        CoUninitialize();
        return -1;
    }

    // Execute the query to get the network adapters
    hr = pService->ExecQuery(_bstr_t("WQL"), _bstr_t("SELECT * FROM Win32_NetworkAdapter"), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnum);
    if (FAILED(hr)) {
        std::cerr << "Error: ExecQuery failed" << std::endl;
        pService->Release();
        pLocator->Release();
        CoUninitialize();
        return -1;
    }

    // Loop through the results
    while (pEnum->Next(WBEM_INFINITE, 1, &pObj, NULL) == S_OK) {
        // Get the MAC address
        hr = pObj->Get(L"MACAddress", 0, &var, NULL, NULL);
        if (SUCCEEDED(hr)) {
            std::wcout << L"MAC Address: " << var.bstrVal << std::endl;
            VariantClear(&var);
        }

        // Get the manufacturer
        hr = pObj->Get(L"Manufacturer", 0, &var, NULL, NULL);
        if (SUCCEEDED(hr)) {
            std::wcout << L"Manufacturer: " << var.bstrVal << std::endl;
            VariantClear(&var);
        }

        pObj->Release();
    }

    // Clean up
    pEnum->Release();
    pService->Release();
    pLocator->Release();
    CoUninitialize();
    return 0;
}

#elif __APPLE__
// macOS-specific code

void infoOfOperationSystemVersion() {
    std::cout <<"**************************"<< __func__ <<"**************************"<< std::endl;

    struct utsname info;
    uname(&info);
    std::cout << "OS name: " << info.sysname << std::endl;
    std::cout << "Release: " << info.release << std::endl;
    std::cout << "Version: " << info.version << std::endl;

}

void infoOfNetworkInterfaceCard() {
    std::cout <<"**************************"<< __func__ <<"**************************"<< std::endl;

    struct ifaddrs* ifaList = nullptr;
    getifaddrs(&ifaList);
    for (struct ifaddrs* ifa = ifaList; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == nullptr) {
            continue;
        }

        int family = ifa->ifa_addr->sa_family;
        if (family == AF_INET || family == AF_INET6) {
            std::cout << "Interface name: " << ifa->ifa_name << std::endl;

            char ipAddrStr[INET6_ADDRSTRLEN];
            void* addrPtr = &(reinterpret_cast<struct sockaddr_in*>(ifa->ifa_addr))->sin_addr;
            if (family == AF_INET6) {
                addrPtr = &(reinterpret_cast<struct sockaddr_in6*>(ifa->ifa_addr))->sin6_addr;
            }
            inet_ntop(family, addrPtr, ipAddrStr, INET6_ADDRSTRLEN);
            std::cout << "IP address: " << ipAddrStr << std::endl;
        }
    }
    freeifaddrs(ifaList);

}



void infoOfCPU() {
    std::cout <<"**************************"<< __func__ <<"**************************"<< std::endl;

    int mib[2] = { CTL_HW, HW_MODEL };
    size_t len = 0;
    sysctl(mib, 2, nullptr, &len, nullptr, 0);
    char model[len];
    sysctl(mib, 2, model, &len, nullptr, 0);
    std::cout << "CPU model: " << model << std::endl;

}


void infoOfDisk() {
    std::cout <<"**************************"<< __func__ <<"**************************"<< std::endl;

    struct statfs sfs;
    statfs("/", &sfs);
    std::cout << "Total space: " << sfs.f_blocks * sfs.f_bsize << std::endl;
    std::cout << "Free space: " << sfs.f_bfree * sfs.f_bsize << std::endl;

}

#endif

int main(){

    infoOfOperationSystemVersion();
        
    infoOfNetworkInterfaceCard();
    
    infoOfCPU();
    
    infoOfDisk();

    infoOfWMI();

    return 0;
}

