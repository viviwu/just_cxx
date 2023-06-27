//
// Created by viviwu on 2023/6/27.
//

#include <winsock2.h>  # forward declaration
#include <iphlpapi.h>
#include <iostream>
#include <string>

#pragma comment(lib, "iphlpapi.lib")

int main() {
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
