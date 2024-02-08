/**
  ******************************************************************************
  * @file           : unix_sys_info.cpp.c
  * @author         : wuxw
  * @brief          : None
  ******************************************************************************
*/

#ifdef __APPLE__
// macOS-specific code
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <sys/utsname.h> # os
#include <sys/sysctl.h>  # CPU info
#include <sys/mount.h> # Disk file info


// macOS-specific code

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

int main() {
    std::cout <<"***********OS info***************"<< std::endl;

    struct utsname info;
    uname(&info);
    std::cout << "OS name: " << info.sysname << std::endl;
    std::cout << "Release: " << info.release << std::endl;
    std::cout << "Version: " << info.version << std::endl;

    std::cout <<"**********CPU info****************"<< std::endl;

    int mib[2] = { CTL_HW, HW_MODEL };
    size_t len = 0;
    sysctl(mib, 2, nullptr, &len, nullptr, 0);
    char model[len];
    sysctl(mib, 2, model, &len, nullptr, 0);
    std::cout << "CPU model: " << model << std::endl;

    std::cout <<"**********Disk info****************"<< std::endl;

    struct statfs sfs;
    statfs("/", &sfs);
    std::cout << "Total space: " << sfs.f_blocks * sfs.f_bsize << std::endl;
    std::cout << "Free space: " << sfs.f_bfree * sfs.f_bsize << std::endl;

    infoOfNetworkInterfaceCard();

  return 0;
}




#endif