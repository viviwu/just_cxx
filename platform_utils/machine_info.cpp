//
// Created by vivi wu on 6/27/23.
//
#ifdef _WIN32
// Windows-specific code
#include <WinSock2.h>  // forward declaration
#include <Windows.h>
#include <iphlpapi.h>
#include <iomanip>

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
#include <vector>

#ifdef _WIN32

#pragma comment(lib, "iphlpapi.lib")

// Windows-specific code
#include <Windows.h>
#include <iphlpapi.h>
#include <stdio.h>

int infoOfAdaptersAddresses() {
    PIP_ADAPTER_ADDRESSES pAdapterAddresses = NULL;
    ULONG outBufLen = 0;
    DWORD dwRetVal = 0;

    // 获取适配器信息所需要的缓冲区大小
    dwRetVal = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_ALL_INTERFACES, NULL, NULL, &outBufLen);
    if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
        // 分配缓冲区
        pAdapterAddresses = (PIP_ADAPTER_ADDRESSES) malloc(outBufLen);
        if (pAdapterAddresses == NULL) {
            printf("Error: out of memory\n");
            return 1;
        }

        // 获取适配器信息
        dwRetVal = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_ALL_INTERFACES, NULL, pAdapterAddresses, &outBufLen);
        if (dwRetVal == NO_ERROR) {
            // 遍历适配器信息
            PIP_ADAPTER_ADDRESSES pCurrAdapter = pAdapterAddresses;
            while (pCurrAdapter) {
                printf("AdapterName: %s\n", pCurrAdapter->AdapterName);
                printf("FriendlyName: %lS\n", pCurrAdapter->FriendlyName);
                if(pCurrAdapter->Description != NULL) printf("AdapterDescription: %S\n", pCurrAdapter->Description);
                printf("Adapter MAC Address: %02X-%02X-%02X-%02X-%02X-%02X\n",
                       pCurrAdapter->PhysicalAddress[0], pCurrAdapter->PhysicalAddress[1],
                       pCurrAdapter->PhysicalAddress[2], pCurrAdapter->PhysicalAddress[3],
                       pCurrAdapter->PhysicalAddress[4], pCurrAdapter->PhysicalAddress[5]);

                // 遍历适配器的 单播地址
                PIP_ADAPTER_UNICAST_ADDRESS pUnicast = pCurrAdapter->FirstUnicastAddress;
                while (pUnicast) {
                    char *unicast  = pUnicast->Address.lpSockaddr->sa_data;
                    if(strlen(unicast )>0) printf("Unicast Address: %s\n", unicast );
                    pUnicast = pUnicast->Next;
                }

                // 遍历适配器的网关地址
                PIP_ADAPTER_GATEWAY_ADDRESS_LH pGateway = pCurrAdapter->FirstGatewayAddress;
                while (pGateway) {
                    char *gateway = pGateway->Address.lpSockaddr->sa_data;
                    if(strlen(gateway)>0)
                        printf("Gateway Address: %s\n", gateway);
                    pGateway = pCurrAdapter->FirstGatewayAddress;
                }

                // 遍历适配器的 DNS 服务器地址
                PIP_ADAPTER_DNS_SERVER_ADDRESS pDnsServer = pCurrAdapter->FirstDnsServerAddress;
                while (pDnsServer) {
                    char * dns_svr = pDnsServer->Address.lpSockaddr->sa_data;
                    if(strlen(dns_svr)>0)
                        printf("DNS Server Address: %s\n", dns_svr);
                    pDnsServer = pDnsServer->Next;
                }

                printf("\n");
                pCurrAdapter = pCurrAdapter->Next;
            }
        } else {
            printf("Error: GetAdaptersAddresses failed with error %d\n", dwRetVal);
            return 1;
        }
    } else {
        printf("Error: GetAdaptersAddresses failed with error %d\n", dwRetVal);
        return 1;
    }

    free(pAdapterAddresses);
    return 0;
}


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

//    GF_guess_localip();

//    infoOfAdaptersAddresses();

    infoOfNetworkInterfaceCard();

    infoOfOperationSystemVersion();

    infoOfCPU();
    
    infoOfDisk();

//    getchar();
    return 0;
}

