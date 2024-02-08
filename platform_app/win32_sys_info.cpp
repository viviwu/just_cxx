/**
  ******************************************************************************
  * @file           : win_adapter_info.cpp.c
  * @author         : wuxw
  * @brief          : None
  ******************************************************************************
*/

// Windows-specific code
#include <WinSock2.h>  // forward declaration
#include <Windows.h>
#include <iphlpapi.h>
#include <iomanip>
#include <WS2tcpip.h>

#include <iostream>

#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Ws2_32.lib")

/**
 * 支持IPv4和IPv6网络，并提供更全面的地址信息
 */
int listAdapterInfo() {
  WSADATA wsaData;
  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    std::cerr << "Failed to initialize Winsock." << std::endl;
    return 1;
  }

  ULONG outBufLen = 0;
  if (GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, nullptr, &outBufLen) != ERROR_BUFFER_OVERFLOW) {
    std::cerr << "Error in GetAdaptersAddresses (1st call)" << std::endl;
    WSACleanup();
    return 1;
  }

  IP_ADAPTER_ADDRESSES* pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
  if (!pAddresses) {
    std::cerr << "Memory allocation failed" << std::endl;
    WSACleanup();
    return 1;
  }

  if (GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, nullptr, pAddresses, &outBufLen) != NO_ERROR) {
    std::cerr << "Error in GetAdaptersAddresses (2nd call)" << std::endl;
    free(pAddresses);
    WSACleanup();
    return 1;
  }

  for (IP_ADAPTER_ADDRESSES* adapter = pAddresses; adapter != nullptr; adapter = adapter->Next) {
    printf("Adapter Name: %s\n", adapter->AdapterName);
    if (adapter->FriendlyName != nullptr && adapter->FriendlyName[0] != L'\0')  {
      // FriendlyName is not null and not empty
      printf("FriendlyName: %LS\n", adapter->FriendlyName); // wprintf(L
    }

    printf("MAC Address: ");
    for (UINT i = 0; i < adapter->PhysicalAddressLength; i++) {
      printf("%02X", adapter->PhysicalAddress[i]);
      if (i < adapter->PhysicalAddressLength - 1) {
        printf(":");
      }
    }
    printf("\n");

    // Output IP
    for (IP_ADAPTER_UNICAST_ADDRESS* address = adapter->FirstUnicastAddress; address != nullptr; address = address->Next) {
      sockaddr* sockaddr_ptr = address->Address.lpSockaddr;
      if (sockaddr_ptr->sa_family == AF_INET) {
        sockaddr_in* sockaddr_ipv4 = reinterpret_cast<sockaddr_in*>(sockaddr_ptr);
        printf("IPv4 Address: %s\n", inet_ntoa(sockaddr_ipv4->sin_addr));
      } else if (sockaddr_ptr->sa_family == AF_INET6) {
        sockaddr_in6* sockaddr_ipv6 = reinterpret_cast<sockaddr_in6*>(sockaddr_ptr);
        char str_buffer[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &(sockaddr_ipv6->sin6_addr), str_buffer, INET6_ADDRSTRLEN);
        printf("IPv6 Address: %s\n", str_buffer);
      }
    }

    // Output DNS ServerIP
    IP_ADAPTER_DNS_SERVER_ADDRESS* dnsServer = adapter->FirstDnsServerAddress;
    while (dnsServer != nullptr) {
      sockaddr* sockaddr_ptr = dnsServer->Address.lpSockaddr;
      if (sockaddr_ptr->sa_family == AF_INET) {
        sockaddr_in* sockaddr_ipv4 = reinterpret_cast<sockaddr_in*>(sockaddr_ptr);
        printf("IPv4 DNS Server: %s\n", inet_ntoa(sockaddr_ipv4->sin_addr));
      } else if (sockaddr_ptr->sa_family == AF_INET6) {
        sockaddr_in6* sockaddr_ipv6 = reinterpret_cast<sockaddr_in6*>(sockaddr_ptr);
        char str_buffer[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &(sockaddr_ipv6->sin6_addr), str_buffer, INET6_ADDRSTRLEN);
        printf("IPv6 DNS Server: %s\n", str_buffer);
      }
      dnsServer = dnsServer->Next;
    }

    // subnet
    if (adapter->FirstPrefix != nullptr) {
      sockaddr* subnetMask = adapter->FirstPrefix->Address.lpSockaddr;
      if (subnetMask->sa_family == AF_INET) {
        sockaddr_in* sockaddr_ipv4 = reinterpret_cast<sockaddr_in*>(subnetMask);
        char str_buffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(sockaddr_ipv4->sin_addr), str_buffer, INET_ADDRSTRLEN);
        printf("Subnet Mask: %s\n", str_buffer);
      } else if (subnetMask->sa_family == AF_INET6) {
        sockaddr_in6* sockaddr_ipv6 = reinterpret_cast<sockaddr_in6*>(subnetMask);
        char str_buffer[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &(sockaddr_ipv6->sin6_addr), str_buffer, INET6_ADDRSTRLEN);
        printf("Subnet Mask: %s\n", str_buffer);
      }
    }
    // GATEWAY
    for (IP_ADAPTER_GATEWAY_ADDRESS* gateway = adapter->FirstGatewayAddress; gateway != nullptr; gateway = gateway->Next) {
      sockaddr* gatewayAddress = gateway->Address.lpSockaddr;
      if (gatewayAddress->sa_family == AF_INET) {
        sockaddr_in* sockaddr_ipv4 = reinterpret_cast<sockaddr_in*>(gatewayAddress);
        char str_buffer[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(sockaddr_ipv4->sin_addr), str_buffer, INET_ADDRSTRLEN);
        printf("Gateway Address: %s\n", str_buffer);
      } else if (gatewayAddress->sa_family == AF_INET6) {
        sockaddr_in6* sockaddr_ipv6 = reinterpret_cast<sockaddr_in6*>(gatewayAddress);
        char str_buffer[INET6_ADDRSTRLEN];
        inet_ntop(AF_INET6, &(sockaddr_ipv6->sin6_addr), str_buffer, INET6_ADDRSTRLEN);
        printf("Gateway Address: %s\n", str_buffer);
      }
    }

    printf("\n");

  }

  free(pAddresses);

  WSACleanup();
  return 0;
}


/**
 * deprecated
 */
void infoOfNetworkInterfaceCard() {
  std::cout << "**************************" << __func__
            << "**************************" << std::endl;

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
      std::cout << "##############" << std::endl;
      // Check if the adapter name contains "virtual", "vmware", or "vbox"
      std::string adapterName(pAdapter->Description);
      if (adapterName.find("Virtual") != std::string::npos ||
          adapterName.find("Vmware") != std::string::npos ||
          adapterName.find("Vbox") != std::string::npos) {
        std::cout << "VirtualAdapter: " << adapterName << std::endl;
      } else {
        // Print out the adapter information
        std::cout << "ComboIndex: " << pAdapter->ComboIndex << std::endl;
        std::cout << "Adapter Name: " << pAdapter->AdapterName << std::endl;
        std::cout << "Adapter Description: " << pAdapter->Description
                  << std::endl;
        std::cout << "IP Address: " << pAdapter->IpAddressList.IpAddress.String
                  << std::endl;
        std::cout << "Subnet Mask: " << pAdapter->IpAddressList.IpMask.String
                  << std::endl;
        std::cout << "Default Gateway: "
                  << pAdapter->GatewayList.IpAddress.String << std::endl;
        std::cout << "Adapter Type: " << pAdapter->Type << pAdapter->HaveWins
                  << std::endl;
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


/**
 * get windows machine info, for example: GetVersionExGetSystemInfoGetDiskFreeSpaceEx ... and so on
 */

int getWindowsMachineInfo() {
  // Get Windows version information
  OSVERSIONINFOEX osvi;
  ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
  if (GetVersionEx(reinterpret_cast<OSVERSIONINFO*>(&osvi))) {
    std::cout << "Windows version: " << osvi.dwMajorVersion << "."
              << osvi.dwMinorVersion << "." << osvi.dwBuildNumber << std::endl;
    std::cout << "Windows PlatformId: " << osvi.dwPlatformId << std::endl;  // 2:Windows NT
  } else {
    std::cerr << "Failed to get Windows version information." << std::endl;
  }

  // Get system information
  SYSTEM_INFO sysInfo;
  GetSystemInfo(&sysInfo);
  std::cout << "Number of Processors: " << sysInfo.dwNumberOfProcessors << std::endl;
  switch (sysInfo.wProcessorArchitecture) {
    case PROCESSOR_ARCHITECTURE_AMD64:
      std::cout << "PROCESSOR_ARCHITECTURE_AMD64" << std::endl;
      break;
    case PROCESSOR_ARCHITECTURE_ARM:
      std::cout << "PROCESSOR_ARCHITECTURE_ARM" << std::endl;
      break;
    case PROCESSOR_ARCHITECTURE_IA64:
      std::cout << "PROCESSOR_ARCHITECTURE_IA64"
                << std::endl;  // Intel Itanium-based
      break;
    case PROCESSOR_ARCHITECTURE_INTEL:
      std::cout << "PROCESSOR_ARCHITECTURE_INTEL(x86)" << std::endl;
      break;
    default:
      std::cout << "Unknown PROCESSOR_ARCHITECTURE" << std::endl;
      break;
  }

  // Get disk space information
  ULARGE_INTEGER freeBytesAvailable;
  ULARGE_INTEGER totalNumberOfBytes;
  ULARGE_INTEGER totalNumberOfFreeBytes;

  if (GetDiskFreeSpaceEx(nullptr, &freeBytesAvailable, &totalNumberOfBytes, &totalNumberOfFreeBytes)) {
    double GB_SIZE = 1024 * 1024 * 1024.0;
    std::cout << "Free Disk Space: " << freeBytesAvailable.QuadPart / GB_SIZE << " GB"<< std::endl;
    std::cout << "Total Disk Space: " << totalNumberOfBytes.QuadPart / GB_SIZE << " GB" << std::endl;
    std::cout << "Total Free Disk Space: " << totalNumberOfFreeBytes.QuadPart / GB_SIZE << " GB" << std::endl;
  } else {
    std::cerr << "Failed to get disk space information." << std::endl;
  }

  return 0;
}


int main() {

  listAdapterInfo();

  getWindowsMachineInfo();

  return 0;
}
