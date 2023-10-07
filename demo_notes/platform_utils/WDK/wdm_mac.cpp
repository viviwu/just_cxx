//
// Created by viviwu on 2023/6/28.


#include <windows.h>

#include <hidsdi.h>
#include <iostream>
#include <ntddndis.h>
#include <setupapi.h>
#include <strsafe.h>
#include <tchar.h>
#include <xstring>

#include <Ndisguid.h>
//#include <ntddser.h>
#include <Ntddmodm.h>
#include <Ntddstor.h>
#include <Usbiodef.h>

#pragma comment(lib, "Setupapi.lib")
#pragma comment(lib, "hid.lib")

using namespace std;

#define MAX_NO 10

INT QueryMacAddress() {
  HDEVINFO hdev_info;
  DWORD member_index, required_size;
  SP_DEVICE_INTERFACE_DATA interface_data;
  PSP_DEVICE_INTERFACE_DETAIL_DATA p_interface_detail_data;
  SP_DEVINFO_DATA device_info;
  INT devs_total = 0;

  // 网卡原生MAC地址（包含USB网卡） 和  网卡原生MAC地址（剔除USB网卡）
  const GUID GUID_QUERYSET[] = {GUID_NDIS_LAN_CLASS, GUID_NDIS_LAN_CLASS};
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
      break; // 设备枚举完毕
    }
    printf("********************member_index=%d ************************\n", member_index);

    // 获取接收缓冲区大小，函数返回值为FALSE，GetLastError()=ERROR_INSUFFICIENT_BUFFER
    SetupDiGetDeviceInterfaceDetail(hdev_info, &interface_data, NULL, 0, &required_size, NULL);
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
      // if(strlen(device_path)>0)  printf("device_path: %s \n", device_path);
      // 剔除虚拟网卡 :  pci#ven vs root#vmware
      if (_tcsnicmp(p_interface_detail_data->DevicePath + 4, TEXT("root"), 4) == 0 ||
          strstr(p_interface_detail_data->DevicePath, "vwifi") != NULL) {
        printf("ignore virtual device!: \n");
        // continue;
      }
      // 获取设备句柄
      HANDLE file_handle =
          CreateFile(p_interface_detail_data->DevicePath,
                     0,
                     FILE_SHARE_READ | FILE_SHARE_WRITE,
                     NULL,
                     OPEN_EXISTING,
                     0,
                     &device_info);
      if (file_handle != INVALID_HANDLE_VALUE) {
        ULONG read_opt;
        BYTE out_buf[8] = {0};
        DWORD out_buf_len;

        // 获取当前MAC地址
        read_opt = OID_802_3_CURRENT_ADDRESS;
        if (DeviceIoControl(file_handle, IOCTL_NDIS_QUERY_GLOBAL_STATS, &read_opt, sizeof(read_opt), out_buf,
                            sizeof(out_buf), &out_buf_len, NULL)) {
          // memcpy( mac_address_list[iIndex].CurrentAddress, out_buf, out_buf_len );
          printf("Current   Address[%ld]: ", out_buf_len);
          for (int i = 0; i < 6; ++i)
            printf("%02X ", out_buf[i]);
        }
        memset(out_buf, 0, 8);
        // 获取原生MAC地址
        read_opt = OID_802_3_PERMANENT_ADDRESS;
        if (DeviceIoControl(file_handle, IOCTL_NDIS_QUERY_GLOBAL_STATS, &read_opt, sizeof(read_opt), out_buf,
                            sizeof(out_buf), &out_buf_len, NULL)) {
          // memcpy( mac_address_list[iIndex].PermanentAddress, out_buf, out_buf_len );
          printf("\nPermanent Address[%ld]: ", out_buf_len);
          for (int i = 0; i < 6; ++i)
            printf("%02X ", out_buf[i]);
          printf("\n");
        }
      } else {
        printf("INVALID_HANDLE_VALUE!-->GetDeviceInterfaceDetail : failed\n");
      }
      CloseHandle(file_handle);
    } else {
      printf("GetDeviceInterfaceDetail : failed\n");
    }
    devs_total++;
    free(p_interface_detail_data);
  }

  SetupDiDestroyDeviceInfoList(hdev_info);

  return devs_total;
}

// The following define is from ntddser.h in the DDK. It is also needed for serial port enumeration.
#ifndef GUID_CLASS_COMPORT
DEFINE_GUID(GUID_CLASS_COMPORT, 0x86e0d1e0L, 0x8089, 0x11d0, 0x9c, 0xe4, 0x08, 0x00, 0x3e, 0x30, 0x1f, 0x73);
#endif
int GetMacProperty() {
  // Create a device information set that will be the container for
  // the device interfaces.
  GUID *dev_guid = (GUID *) &GUID_CLASS_COMPORT;
  HDEVINFO hdev_info = INVALID_HANDLE_VALUE;
  SP_DEVICE_INTERFACE_DETAIL_DATA *p_interface_detail_data = NULL;
  SP_DEVINFO_DATA dev_info = {0};
  dev_info.cbSize = sizeof(SP_DEVINFO_DATA);

  hdev_info = SetupDiGetClassDevs(dev_guid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);

  if (INVALID_HANDLE_VALUE == hdev_info) {
    return FALSE;
  }

  // Enumerate the serial ports
  BOOL bOk = TRUE;
  SP_DEVICE_INTERFACE_DATA dev_if_dt;
  DWORD dwDetDataSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA) + 256;
  p_interface_detail_data = (SP_DEVICE_INTERFACE_DETAIL_DATA *) new char[dwDetDataSize];
  if (!p_interface_detail_data) {
    return FALSE;
  }
  // This is required, according to the documentation. Yes, it's weird.
  dev_if_dt.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
  p_interface_detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
  for (DWORD member_index = 0;; member_index++) {
    bOk = SetupDiEnumDeviceInterfaces(hdev_info, NULL, dev_guid, member_index, &dev_if_dt);
    if (bOk) {
      // Got a device. Get the details.
      bOk = SetupDiGetDeviceInterfaceDetail(hdev_info,
                                            &dev_if_dt,
                                            p_interface_detail_data,
                                            dwDetDataSize,
                                            NULL,
                                            &dev_info);
      if (bOk) {
        // Got a path to the device. Try to get some more info.
        BYTE fname[256] = {0};
        BYTE desc[256] = {0};
        BOOL bSuccess = SetupDiGetDeviceRegistryProperty(hdev_info, &dev_info, SPDRP_FRIENDLYNAME, NULL, (PBYTE) fname,
                                                         sizeof(fname), NULL);
        bSuccess =
            bSuccess && SetupDiGetDeviceRegistryProperty(hdev_info, &dev_info, SPDRP_DEVICEDESC, NULL, (PBYTE) desc,
                                                         sizeof(desc), NULL);
        BOOL bUsbDevice = FALSE;
        WCHAR locinfo[256] = {0};
        if (SetupDiGetDeviceRegistryProperty(hdev_info, &dev_info, SPDRP_LOCATION_INFORMATION, NULL, (PBYTE) locinfo,
                                             sizeof(locinfo), NULL)) {
          // Just check the first three characters to determine
          // if the port is connected to the USB bus. This isn't
          // an infallible method; it would be better to use the
          // BUS GUID. Currently, Windows doesn't let you query
          // that though (SPDRP_BUSTYPEGUID seems to exist in
          // documentation only).
          bUsbDevice = (wcsncmp(locinfo, L"USB", 3) == 0);
        }
        if (bSuccess) {
          printf("FriendlyName = %s \r\n", fname);
          printf("Port Desc = %s \r\n", desc);
        }

      } else {
        if (p_interface_detail_data != NULL) {
          delete[](char *) p_interface_detail_data;
        }
        if (hdev_info != INVALID_HANDLE_VALUE) {
          SetupDiDestroyDeviceInfoList(hdev_info);
        }
        return FALSE;
      }
    } else {
      DWORD err = GetLastError();
      if (err != ERROR_NO_MORE_ITEMS) {
        if (p_interface_detail_data != NULL) {
          delete[](char *) p_interface_detail_data;
        }
        if (hdev_info != INVALID_HANDLE_VALUE) {
          SetupDiDestroyDeviceInfoList(hdev_info);
        }
        return FALSE;
      }
    }
  }

  if (p_interface_detail_data != NULL) {
    delete[](char *) p_interface_detail_data;
  }
  if (hdev_info != INVALID_HANDLE_VALUE) {
    SetupDiDestroyDeviceInfoList(hdev_info);
  }

  return TRUE;
}

int main(int argc, char *argv[]) {
  QueryMacAddress();

//    setlocale(LC_ALL, "chs");
//    GetMacProperty();

  getchar();
  return 0;
}
