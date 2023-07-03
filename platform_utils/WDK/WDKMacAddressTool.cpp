//
// Created by dejavu on 2023/6/30.
//

#include "WDKMacAddressTool.h"

#include <tchar.h>
#include <strsafe.h>
#include <setupapi.h>
#include <ntddndis.h>
#include <algorithm>

#ifdef __cplusplus
extern "C"
{
#endif

#include <hidsdi.h>

#ifdef __cplusplus
}
#endif

#pragma comment (lib, "Setupapi.lib")
#pragma comment (lib, "hid.lib")

const GUID GUID_QUERYSET[] = {
    // 网卡原生MAC地址（包含USB网卡）
    {0xAD498944, 0x762F, 0x11D0, 0x8D, 0xCB, 0x00, 0xC0, 0x4F, 0xC3, 0x35, 0x8C},

    // 网卡原生MAC地址（剔除USB网卡）
    {0xAD498944, 0x762F, 0x11D0, 0x8D, 0xCB, 0x00, 0xC0, 0x4F, 0xC3, 0x35, 0x8C},
};

// 获取网卡原生MAC地址
static BOOL GetGetMacAddress( TCHAR* device_path, T_MAC_ADDRESS *mac_address_list, INT iIndex, BOOL includeUSB )
{
  HANDLE	hDeviceFile;
  BOOL	isOK = FALSE;

  // 剔除虚拟网卡
  if( _tcsnicmp( device_path + 4, TEXT("root"), 4 ) == 0 )
  {
    return FALSE;
  }

  if( !includeUSB )
  {	// 剔除USB网卡
    if( _tcsnicmp( device_path + 4, TEXT("usb"), 4 ) == 0 )
    {
      return FALSE;
    }
  }

  // 获取设备句柄
  hDeviceFile = CreateFile( device_path,
                            0,
                            FILE_SHARE_READ | FILE_SHARE_WRITE,
                            NULL,
                            OPEN_EXISTING,
                            0,
                            NULL);

  if( hDeviceFile != INVALID_HANDLE_VALUE )
  {
    ULONG	in_buf;
    BYTE	ucData[8];
    DWORD	dwByteRet;

    // 获取当前MAC地址
    in_buf = OID_802_3_CURRENT_ADDRESS;
    isOK = DeviceIoControl( hDeviceFile, IOCTL_NDIS_QUERY_GLOBAL_STATS, &in_buf, sizeof(in_buf), ucData, sizeof(ucData), &dwByteRet, NULL );
    if( isOK )
    {
      memcpy( mac_address_list[iIndex].CurrentAddress, ucData, dwByteRet );

      // 获取原生MAC地址
      in_buf = OID_802_3_PERMANENT_ADDRESS;
      isOK = DeviceIoControl( hDeviceFile, IOCTL_NDIS_QUERY_GLOBAL_STATS, &in_buf, sizeof(in_buf), ucData, sizeof(ucData), &dwByteRet, NULL );
      if( isOK )
      {
        memcpy( mac_address_list[iIndex].PermanentAddress, ucData, dwByteRet );
      }
    }

    CloseHandle( hDeviceFile );
  }

  return isOK;
}

static BOOL GetDeviceProperty( TCHAR* device_path, INT query_type, T_MAC_ADDRESS *mac_address_list, INT iIndex )
{
  BOOL isOK = FALSE;

  switch( query_type )
  {
    case 0:		// 网卡原生MAC地址（包含USB网卡）
      isOK = GetGetMacAddress( device_path, mac_address_list, iIndex, TRUE );
      break;

    case 1:		// 网卡原生MAC地址（剔除USB网卡）
      isOK = GetGetMacAddress( device_path, mac_address_list, iIndex, FALSE );
      break;

    default:
      break;
  }

  return isOK;
}

INT QueryMacAddress( INT query_type, T_MAC_ADDRESS *mac_address_list, INT list_size )
{
  HDEVINFO	hdev_info;
  DWORD		member_index, required_size;
  SP_DEVICE_INTERFACE_DATA			device_interface_data;
  PSP_DEVICE_INTERFACE_DETAIL_DATA	device_interface_detail_data;
  SP_DEVINFO_DATA device_info_data;
  INT	devs_total = 0;
  TCHAR fname[256], locinfo[256];
  PDWORD reg_data_type;

  // 判断查询类型是否支持
  if( (query_type < 0) || (query_type >= sizeof(GUID_QUERYSET)/sizeof(GUID)) )
  {
    return -2;	// 查询类型不支持
  }

  // 获取设备信息集
  hdev_info = SetupDiGetClassDevs( GUID_QUERYSET + query_type, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE );
  if( hdev_info == INVALID_HANDLE_VALUE )
  {
    return -1;
  }

    device_interface_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
  // 枚举设备信息集中所有设备
  for( member_index = 0; ((mac_address_list == NULL) || (devs_total < list_size)); member_index++ )
  {	// 获取设备接口
    if( !SetupDiEnumDeviceInterfaces( hdev_info, NULL, GUID_QUERYSET + query_type, member_index, &device_interface_data ) )
    {	// 设备枚举完毕
      break;
    }

    // 获取接收缓冲区大小，函数返回值为FALSE，GetLastError()=ERROR_INSUFFICIENT_BUFFER
    SetupDiGetDeviceInterfaceDetail( hdev_info, &device_interface_data, NULL, 0, &required_size, NULL );

    // 申请接收缓冲区
    device_interface_detail_data = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc( required_size );
    device_interface_detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

    // 获取设备细节信息
    if( SetupDiGetDeviceInterfaceDetail( hdev_info, &device_interface_data, device_interface_detail_data, required_size, NULL, NULL ) )
    {
      if( mac_address_list != NULL )
      {
        if( GetDeviceProperty( device_interface_detail_data->DevicePath, query_type, mac_address_list, devs_total ) )
        {
          devs_total++;
        }
      }
      else
      {
        devs_total++;
      }
    }

    free( device_interface_detail_data );
  }

  SetupDiDestroyDeviceInfoList( hdev_info );

  return devs_total;
}
