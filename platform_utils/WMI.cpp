//
// Created by viviwu on 2023/6/28.
//

#include <windows.h>

#include <tchar.h>
#include <strsafe.h>
#include <setupapi.h>
#include <ntddndis.h>
#include <hidsdi.h>

#pragma comment (lib, "Setupapi.lib")
#pragma comment (lib, "hid.lib")

#define MAX_NO 10

const GUID GUID_QUERYSET[] = {
    // 网卡原生MAC地址（包含USB网卡）
    {0xAD498944, 0x762F, 0x11D0, 0x8D, 0xCB, 0x00, 0xC0, 0x4F, 0xC3, 0x35, 0x8C},

    // 网卡原生MAC地址（剔除USB网卡）
    {0xAD498944, 0x762F, 0x11D0, 0x8D, 0xCB, 0x00, 0xC0, 0x4F, 0xC3, 0x35, 0x8C},
};

INT QueryMacAddress()
{

    HDEVINFO	hdev_info;
    DWORD		member_index, required_size;
    SP_DEVICE_INTERFACE_DATA			device_interface_data;
    PSP_DEVICE_INTERFACE_DETAIL_DATA	p_device_interface_detail_data;
    SP_DEVINFO_DATA device_info_data;
    INT	devs_total = 0;
    TCHAR fname[256];
    PDWORD reg_data_type;
    CHAR   *device_path ;

    // 获取设备信息集
    hdev_info = SetupDiGetClassDevs( GUID_QUERYSET, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE );
    if( hdev_info == INVALID_HANDLE_VALUE )
    {
        return -1;
    }

    device_interface_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    device_info_data.cbSize = sizeof(SP_DEVINFO_DATA);
    // 枚举设备信息集中所有设备
    for( member_index = 0; member_index< MAX_NO; member_index++ )
    {	// 获取设备接口
        if( !SetupDiEnumDeviceInterfaces( hdev_info, NULL, GUID_QUERYSET, member_index, &device_interface_data ) )
        {	// 设备枚举完毕
            break;
        }
        memset( &device_info_data, 0, sizeof( SP_DEVINFO_DATA ) );

        // 获取接收缓冲区大小，函数返回值为FALSE，GetLastError()=ERROR_INSUFFICIENT_BUFFER
        SetupDiGetDeviceInterfaceDetail( hdev_info, &device_interface_data, NULL, 0, &required_size, NULL );

        // 申请接收缓冲区
        p_device_interface_detail_data = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc( required_size );
        p_device_interface_detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
/*
        // 获取设备细节信息
        if( SetupDiGetDeviceInterfaceDetail( hdev_info, &device_interface_data, p_device_interface_detail_data, required_size, NULL, NULL ) )
        {
            device_path = p_device_interface_detail_data->DevicePath;
            // 剔除虚拟网卡
            if( _tcsnicmp( device_path + 4, TEXT("root"), 4 ) == 0 )
            {
                printf("DevicePath:%s \n", p_device_interface_detail_data->DevicePath);
                continue;
            }

        }
*/
        if ( SetupDiGetDeviceRegistryProperty( hdev_info, &device_info_data, SPDRP_FRIENDLYNAME, NULL, ( PBYTE )fname, sizeof( fname ), NULL ) ){
            printf("%d \n", _tcsclen(fname));
//                printf("fname: %s\ n", fname);
        }
        devs_total++;
        free( p_device_interface_detail_data );
    }

    SetupDiDestroyDeviceInfoList( hdev_info );

    return devs_total;
}

int EnumPortsWdm()
{
    int member_index, interface_detail_data_size;
    DWORD required_size;
    TCHAR fname[256], locinfo[256];
    HDEVINFO hdev_info;
    SP_DEVICE_INTERFACE_DETAIL_DATA* p_device_interface_detail_data;
    SP_DEVICE_INTERFACE_DATA device_interface_data;
    SP_DEVINFO_DATA device_info_data;

    static const GUID ___GUID_CLASS_COMPORT = \
    { 0x86E0D1E0L, 0x8089, 0x11D0, { 0x9C, 0xE4, 0x08, 0x00, 0x3E, 0x30, 0x1F, 0x73 } };
    /*static const GUID ___GUID_CLASS_COMPORT = \
    { 0xAD498944, 0x762F, 0x11D0, { 0x8D, 0xCB, 0x00, 0xC0, 0x4F, 0xC3, 0x35, 0x8C } };*/


    hdev_info = SetupDiGetClassDevs( &___GUID_CLASS_COMPORT, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE );
    if ( hdev_info == INVALID_HANDLE_VALUE )
    {
        return -1;
    }

    interface_detail_data_size = sizeof( SP_DEVICE_INTERFACE_DETAIL_DATA ) + 256;
    printf("interface_detail_data_size=%d \n", interface_detail_data_size); //261
    p_device_interface_detail_data = ( SP_DEVICE_INTERFACE_DETAIL_DATA* )calloc( 1, interface_detail_data_size );
    if ( !p_device_interface_detail_data )
    {
        SetupDiDestroyDeviceInfoList( hdev_info );
        return -1;
    }

    device_interface_data.cbSize = sizeof( SP_DEVICE_INTERFACE_DATA );
    p_device_interface_detail_data->cbSize = sizeof( SP_DEVICE_INTERFACE_DETAIL_DATA );

    for ( member_index = 0; SetupDiEnumDeviceInterfaces( hdev_info, NULL, &___GUID_CLASS_COMPORT, member_index, &device_interface_data ); member_index++ )
    { 
        memset( &device_info_data, 0, sizeof( SP_DEVINFO_DATA ) );
        device_info_data.cbSize = sizeof( SP_DEVINFO_DATA );

        if ( !SetupDiGetDeviceInterfaceDetail( hdev_info, &device_interface_data, p_device_interface_detail_data, interface_detail_data_size, &required_size, &device_info_data ) )
        {
            break;
        }
        printf("member_index=%d  \n", member_index);
        printf("required_size=%d  \n", required_size); //108
        printf("DevicePath:%s \n", p_device_interface_detail_data->DevicePath);
        //MessageBox( NULL, p_device_interface_detail_data->DevicePath, TEXT( "PATH" ), MB_OK );
        // 申请接收缓冲区
//        p_device_interface_detail_data = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc( required_size );

        if ( SetupDiGetDeviceRegistryProperty( hdev_info, &device_info_data, SPDRP_FRIENDLYNAME, NULL, ( PBYTE )fname, sizeof( fname ), NULL ) )
        {
            printf("fname:%s \n", fname);
            // MessageBox( NULL, fname, TEXT( "NAME" ), MB_OK );
        }

        if ( SetupDiGetDeviceRegistryProperty( hdev_info, &device_info_data, SPDRP_LOCATION_INFORMATION, NULL, ( PBYTE )locinfo, sizeof( locinfo ), NULL ) )
        {
            if ( _tcsnicmp( locinfo, TEXT( "USB" ), 3 ) == 0 )
            {
                MessageBox( NULL, TEXT( "USB" ), TEXT( "TYPE" ), MB_OK );
            }
        }
    }

    free( p_device_interface_detail_data );
    SetupDiDestroyDeviceInfoList( hdev_info );
    return 0;
}

int main(){

//    QueryMacAddress();

    EnumPortsWdm();

    return 0;
}