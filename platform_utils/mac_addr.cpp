//
// Created by viviwu on 2023/6/28.
//

#include <windows.h>

#include <tchar.h>
#include <strsafe.h>
#include <setupapi.h>
#include <ntddndis.h>
#include <hidsdi.h>
#include <iostream>
#include <xstring>

#pragma comment (lib, "Setupapi.lib")
#pragma comment (lib, "hid.lib")

#define MAX_NO 10

const GUID GUID_QUERYSET[] = {
    // 网卡原生MAC地址（包含USB网卡）
    {0xAD498944, 0x762F, 0x11D0, 0x8D, 0xCB, 0x00, 0xC0, 0x4F, 0xC3, 0x35, 0x8C},

    // 网卡原生MAC地址（剔除USB网卡）
    {0xAD498944, 0x762F, 0x11D0, 0x8D, 0xCB, 0x00, 0xC0, 0x4F, 0xC3, 0x35, 0x8C},
};

INT QueryMacAddress() {

  HDEVINFO hdev_info;
  DWORD member_index, required_size;
  SP_DEVICE_INTERFACE_DATA interface_data;
  PSP_DEVICE_INTERFACE_DETAIL_DATA p_interface_detail_data;
  SP_DEVINFO_DATA device_info;
  INT devs_total = 0;
  TCHAR friendly_name[256];
//    PDWORD reg_data_type;
  CHAR *device_path;

  // 获取设备信息集
  hdev_info = SetupDiGetClassDevs(GUID_QUERYSET, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
  if (hdev_info == INVALID_HANDLE_VALUE) {
    return -1;
  }

  interface_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
  device_info.cbSize = sizeof(SP_DEVINFO_DATA);
  // 枚举设备信息集中所有设备
  for (member_index = 0; member_index < MAX_NO; member_index++) {
    // 获取设备接口
    if (!SetupDiEnumDeviceInterfaces(hdev_info, NULL, GUID_QUERYSET, member_index, &interface_data)) {
      break;  // 设备枚举完毕
    }
    printf("********************member_index=%d ************************\n", member_index);

    // 获取接收缓冲区大小，函数返回值为FALSE，GetLastError()=ERROR_INSUFFICIENT_BUFFER
    SetupDiGetDeviceInterfaceDetail(hdev_info,
                                    &interface_data,
                                    NULL,
                                    0,
                                    &required_size,
                                    NULL);
    // 申请接收缓冲区
    p_interface_detail_data = (PSP_DEVICE_INTERFACE_DETAIL_DATA) malloc(required_size);
    p_interface_detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

    // 获取设备细节信息
    if (SetupDiGetDeviceInterfaceDetail(hdev_info,
                                        &interface_data,
                                        p_interface_detail_data,
                                        required_size,
                                        NULL,
                                        NULL)) {
      device_path = p_interface_detail_data->DevicePath;
      // 剔除虚拟网卡
      if (_tcsnicmp(device_path + 4, TEXT("root"), 4) == 0) {
        continue;
      } else {
        // 获取设备句柄
        HANDLE hDeviceFile = CreateFile(device_path,
                                        0,
                                        FILE_SHARE_READ | FILE_SHARE_WRITE,
                                        NULL,
                                        OPEN_EXISTING,
                                        0,
                                        &device_info);
        if (hDeviceFile != INVALID_HANDLE_VALUE) {
          ULONG in_buf;
          BYTE ucData[8];
          DWORD dwByteRet;

          // 获取当前MAC地址
          in_buf = OID_802_3_CURRENT_ADDRESS;
          BOOL isOK = DeviceIoControl(hDeviceFile,
                                      IOCTL_NDIS_QUERY_GLOBAL_STATS,
                                      &in_buf,
                                      sizeof(in_buf),
                                      ucData,
                                      sizeof(ucData),
                                      &dwByteRet,
                                      NULL);
          if (isOK) {
            // memcpy( mac_address_list[iIndex].CurrentAddress, ucData, dwByteRet );
            printf("CurrentAddress:");
            for (int i = 0; i < 6; ++i)
              printf("%02X", ucData[i]);

            printf("\nPermanentAddress:");
            // 获取原生MAC地址
            in_buf = OID_802_3_PERMANENT_ADDRESS;
            isOK = DeviceIoControl(hDeviceFile,
                                   IOCTL_NDIS_QUERY_GLOBAL_STATS,
                                   &in_buf,
                                   sizeof(in_buf),
                                   ucData,
                                   sizeof(ucData),
                                   &dwByteRet,
                                   NULL);
            if (isOK) {
              //memcpy( mac_address_list[iIndex].PermanentAddress, ucData, dwByteRet );
              for (int i = 0; i < 6; ++i)
                printf("%02X", ucData[i]);
              printf("\n");
            }
          }
          CloseHandle(hDeviceFile);
        }else
          printf("DevicePath:%s \n", p_interface_detail_data->DevicePath);
      }

    }
    memset(&device_info, 0, sizeof(SP_DEVINFO_DATA));
    if (SetupDiGetDeviceInterfaceDetail(hdev_info,
                                        &interface_data,
                                        p_interface_detail_data,
                                        required_size,
                                        NULL,
                                        &device_info)){
      printf("ClassGuid: %s \n", device_info.ClassGuid.Data4);
      if (SetupDiGetDeviceRegistryProperty(hdev_info,
                                           &device_info,
                                           SPDRP_FRIENDLYNAME,
                                           NULL,
                                           (PBYTE) friendly_name,
                                           sizeof(friendly_name),
                                           NULL)) {
        printf("%zd friendly_name: %s\n", _tcsclen(friendly_name), friendly_name);
      }
    }

    memset(&device_info, 0, sizeof(SP_DEVINFO_DATA));
    BOOL ok=SetupDiGetDeviceInterfaceDetail(hdev_info,
                                            &interface_data,
                                            p_interface_detail_data,
                                            required_size,
                                            NULL,
                                            &device_info);
    if (ok){
      printf("ClassGuid: %s \n", device_info.ClassGuid.Data4);
      if (SetupDiGetDeviceRegistryProperty(hdev_info,
                                           &device_info,
                                           SPDRP_FRIENDLYNAME,
                                           NULL,
                                           (PBYTE) friendly_name,
                                           sizeof(friendly_name),
                                           NULL)) {
        printf("%zd friendly_name: %s\n", _tcsclen(friendly_name), friendly_name);
      }
    }

    devs_total++;
    free(p_interface_detail_data);
  }

  SetupDiDestroyDeviceInfoList(hdev_info);

  return devs_total;
}

int EnumPortsWdm() {
  DWORD member_index, interface_detail_data_size;
  DWORD required_size;
  TCHAR friendly_name[256], property_buffer[256];
  HDEVINFO hdev_info;
  SP_DEVICE_INTERFACE_DETAIL_DATA *p_interface_detail_data;
  SP_DEVICE_INTERFACE_DATA interface_data;
  SP_DEVINFO_DATA device_info;

  static const GUID ___GUID_CLASS_COMPORT = \
    {0x86E0D1E0L, 0x8089, 0x11D0, {0x9C, 0xE4, 0x08, 0x00, 0x3E, 0x30, 0x1F, 0x73}};
  /*static const GUID ___GUID_CLASS_COMPORT = \
  { 0xAD498944, 0x762F, 0x11D0, { 0x8D, 0xCB, 0x00, 0xC0, 0x4F, 0xC3, 0x35, 0x8C } };*/


  hdev_info = SetupDiGetClassDevs(&___GUID_CLASS_COMPORT, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);
  if (hdev_info == INVALID_HANDLE_VALUE) {
    return -1;
  }

  interface_detail_data_size = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) + 256;

  p_interface_detail_data = (SP_DEVICE_INTERFACE_DETAIL_DATA *) calloc(1, interface_detail_data_size);
  if (!p_interface_detail_data) {
    SetupDiDestroyDeviceInfoList(hdev_info);
    return -1;
  }
  printf("interface_detail_data_size=%d \n", interface_detail_data_size); //261
  interface_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
  p_interface_detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

  for (member_index = 0;
       SetupDiEnumDeviceInterfaces(hdev_info, NULL, &___GUID_CLASS_COMPORT, member_index, &interface_data);
       member_index++) {
    memset(&device_info, 0, sizeof(SP_DEVINFO_DATA));
    device_info.cbSize = sizeof(SP_DEVINFO_DATA);
    printf("member_index=%d  \n", member_index);
    if (!SetupDiGetDeviceInterfaceDetail(hdev_info,
                                         &interface_data,
                                         p_interface_detail_data,
                                         interface_detail_data_size,
                                         &required_size,
                                         &device_info)) {
      break;
    }

    printf("required_size=%d  \n", required_size); //108
    printf("DevicePath:%s \n", p_interface_detail_data->DevicePath);
    //MessageBox( NULL, p_interface_detail_data->DevicePath, TEXT( "PATH" ), MB_OK );
    // 申请接收缓冲区
//        p_interface_detail_data = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc( required_size );

    if (SetupDiGetDeviceRegistryProperty(hdev_info,
                                         &device_info,
                                         SPDRP_FRIENDLYNAME,
                                         NULL,
                                         (PBYTE) friendly_name,
                                         sizeof(friendly_name),
                                         NULL)) {
      printf("friendly_name:%s \n", friendly_name);
      // MessageBox( NULL, friendly_name, TEXT( "NAME" ), MB_OK );
    }

    if (SetupDiGetDeviceRegistryProperty(hdev_info,
                                         &device_info,
                                         SPDRP_LOCATION_INFORMATION,
                                         NULL,
                                         (PBYTE) property_buffer,
                                         sizeof(property_buffer),
                                         NULL)) {
      if (_tcsnicmp(property_buffer, TEXT("USB"), 3) == 0) {
        MessageBox(NULL, TEXT("USB"), TEXT("TYPE"), MB_OK);
      }
    }
  }

  free(p_interface_detail_data);
  SetupDiDestroyDeviceInfoList(hdev_info);
  return 0;
}

int main() {

  QueryMacAddress();

//    EnumPortsWdm();

  return 0;
}

bool getDeviceProperty(HDEVINFO devices, SP_DEVINFO_DATA &device_info, DWORD property, std::wstring &value) {
  DWORD size = 8192;

  try {
    DWORD data_type;
    DWORD requested_size;
    LPTSTR buffer = new TCHAR[(size / sizeof(TCHAR)) + 1];
    memset(buffer, 0x00, (size / sizeof(TCHAR)) + 1);

    if (buffer) {
      bool success = true;

      while (!SetupDiGetDeviceRegistryProperty(devices,
                                               &device_info,
                                               property,
                                               &data_type,
                                               reinterpret_cast<LPBYTE>(buffer),
                                               size,
                                               &requested_size)) {
        if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
          success = false;
          break;
        }

        if (data_type != REG_SZ) {
          success = false;
          break;
        }

        size = requested_size;
        delete[] buffer;
        buffer = new TCHAR[(size / sizeof(TCHAR)) + 1];
        memset(buffer, 0x00, (size / sizeof(TCHAR)) + 1);

        if (!buffer) {
          success = false;
          break;
        }
      }

      if (success) {
        value = std::wstring(reinterpret_cast<const wchar_t *const>(buffer));
      }

      delete[] buffer;

      return true;
    }
  }
  catch (...) {
    std::wcerr << "Allocation error. This is serious !" << std::endl;
  }

  return false;
}