//
// Created by vivi wu on 6/27/23.
//
#ifdef _WIN32
// Windows-specific code
#include <winsock2.h>  # forward declaration
#include <iphlpapi.h>
#include <windows.h>

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

#pragma comment(lib, "iphlpapi.lib")

int infoOfNetworkInterfaceCard() {
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
            // Check if the adapter name contains "virtual", "vmware", or "vbox"
            std::string adapterName(pAdapter->Description);
            if (adapterName.find("Virtual") != std::string::npos ||
                adapterName.find("Vmware") != std::string::npos ||
                adapterName.find("Vbox") != std::string::npos) {
                std::cout << "Adapter " << adapterName << " is a virtual network adapter." << std::endl;
            } else {
                std::cout << "Adapter " << adapterName << " is not a virtual network adapter." << std::endl;
                // Print out the adapter information
                std::cout << "Adapter Name: " << pAdapter->AdapterName << std::endl;
                std::cout << "Adapter Description: " << pAdapter->Description << std::endl;
                std::cout << "IP Address: " << pAdapter->IpAddressList.IpAddress.String << std::endl;
                std::cout << "Subnet Mask: " << pAdapter->IpAddressList.IpMask.String << std::endl;
                std::cout << "Default Gateway: " << pAdapter->GatewayList.IpAddress.String << std::endl;
                std::cout << "Type: " << pAdapter->Type << "HaveWins: " << pAdapter->HaveWins << std::endl;
            }

            // Move on to the next adapter
            pAdapter = pAdapter->Next;
        }
    }

    // Free the adapter information buffer
    delete[] pAdapterInfo;

    return 0;
}


int InfoOfOperationSystemVersion() {
    OSVERSIONINFO osvi;
    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osvi);
    std::cout << "Windows version: " << osvi.dwMajorVersion << "." << osvi.dwMinorVersion << std::endl;
    return 0;
}


int infoOfCPU() {
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    std::cout << "Processor architecture: " << sysInfo.wProcessorArchitecture << std::endl;
    std::cout << "Number of processors: " << sysInfo.dwNumberOfProcessors << std::endl;
    std::cout << "Processor type: " << sysInfo.dwProcessorType << std::endl;
    return 0;
}


int infoOfDisk() {
    ULARGE_INTEGER freeBytesAvailable, totalNumberOfBytes, totalNumberOfFreeBytes;
    GetDiskFreeSpaceEx("C:", &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes);
    std::cout << "Free space: " << freeBytesAvailable.QuadPart << std::endl;
    std::cout << "Total space: " << totalNumberOfBytes.QuadPart << std::endl;
    return 0;
}

#elif __APPLE__
// macOS-specific code

int InfoOfOperationSystemVersion() {
    struct utsname info;
    uname(&info);
    std::cout << "OS name: " << info.sysname << std::endl;
    std::cout << "Release: " << info.release << std::endl;
    std::cout << "Version: " << info.version << std::endl;
    return 0;
}

int infoOfNetworkInterfaceCard() {
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
    return 0;
}



int infoOfCPU() {
    int mib[2] = { CTL_HW, HW_MODEL };
    size_t len = 0;
    sysctl(mib, 2, nullptr, &len, nullptr, 0);
    char model[len];
    sysctl(mib, 2, model, &len, nullptr, 0);
    std::cout << "CPU model: " << model << std::endl;
    return 0;
}


int infoOfDisk() {
    struct statfs sfs;
    statfs("/", &sfs);
    std::cout << "Total space: " << sfs.f_blocks * sfs.f_bsize << std::endl;
    std::cout << "Free space: " << sfs.f_bfree * sfs.f_bsize << std::endl;
    return 0;
}

#endif


    int main(){

        return 0;
    }
