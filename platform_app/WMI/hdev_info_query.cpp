#include "hdev_info_query.h"
//#include "UnmanagedCode.h"
#include <WbemIdl.h>
#include <atlconv.h>
#include <comutil.h>
#include <ntddndis.h>
#include <strsafe.h>
#include <tchar.h>
#include <algorithm>
#include <iostream>

using namespace std;

#pragma comment (lib, "comsuppw.lib")
#pragma comment (lib, "wbemuuid.lib")


typedef struct _T_WQL_QUERY
{
	CHAR*	szSelect;		// SELECT语句
	WCHAR*	szProperty;		// 属性字段
} T_WQL_QUERY;

// WQL查询语句
const T_WQL_QUERY szWQLQuery[] = {

	// 网卡原生MAC地址
	"SELECT * FROM Win32_NetworkAdapter WHERE (mac_addr IS NOT NULL) AND (NOT (pnp_dev_id LIKE 'ROOT%'))",
	L"pnp_dev_id",

	// 硬盘序列号
	"SELECT * FROM Win32_DiskDrive WHERE (serial_number IS NOT NULL) AND (MediaType LIKE 'Fixed hard disk%')",
	L"serial_number",

	// 主板序列号
	"SELECT * FROM Win32_BaseBoard WHERE (serial_number IS NOT NULL)",
	L"serial_number",	

	// 处理器ID
	"SELECT * FROM Win32_Processor WHERE (ProcessorId IS NOT NULL)",
	L"ProcessorId",

	// BIOS序列号
	"SELECT * FROM Win32_BIOS WHERE (serial_number IS NOT NULL)",
	L"serial_number",

	// 主板型号
	"SELECT * FROM Win32_BaseBoard WHERE (Product IS NOT NULL)",
	L"Product",

	// 网卡当前MAC地址
	"SELECT * FROM Win32_NetworkAdapter WHERE (mac_addr IS NOT NULL) AND (NOT (pnp_dev_id LIKE 'ROOT%'))",
	L"mac_addr",

	// 
	//"SELECT * FROM Win32_OperatingSystem WHERE (Name IS NOT NULL)",
	//L"Name",

};

// 通过“pnp_dev_id”获取网卡原生MAC地址
static BOOL QueryPNPdeviceID( const TCHAR *pnp_dev_id, TCHAR *mac_addr, UINT uSize )
{
	TCHAR	device_path[MAX_PATH];
	HANDLE	device_handle;	
	BOOL	isOK = FALSE;

	// 生成设备路径名
	StringCchCopy( device_path, MAX_PATH, TEXT("\\\\.\\") );
	StringCchCat( device_path, MAX_PATH, pnp_dev_id );
	StringCchCat( device_path, MAX_PATH, TEXT("#{ad498944-762f-11d0-8dcb-00c04fc3358c}") );

	// 将“pnp_dev_id”中的“\”替换成“#”，以获得真正的设备路径名
	std::replace( device_path + 4, device_path + 4 + _tcslen(pnp_dev_id), TEXT('\\'), TEXT('#') ); 

	// 获取设备句柄
	device_handle = CreateFile( device_path,
		0,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);

	if( device_handle != INVALID_HANDLE_VALUE )
	{	
		ULONG	dwID;
		BYTE	ucData[8];
		DWORD	dwByteRet;		

		// 获取网卡原生MAC地址
		dwID = OID_802_3_PERMANENT_ADDRESS;
		isOK = DeviceIoControl( device_handle, IOCTL_NDIS_QUERY_GLOBAL_STATS, &dwID, sizeof(dwID), ucData, sizeof(ucData), &dwByteRet, NULL );
		if( isOK )
		{	// 将字节数组转换成16进制字符串
			for( DWORD i = 0; i < dwByteRet; i++ )
			{
				StringCchPrintf( mac_addr + (i << 1), uSize - (i << 1), TEXT("%02X"), ucData[i] );
			}
		}

		CloseHandle( device_handle );
	}

	return isOK;
}

static BOOL QueryHardDiskSerialNumber( TCHAR *serial_number, UINT uSize )
{
	UINT	out_buf_len;
	UINT	i;

	out_buf_len = _tcslen( serial_number );
	if( out_buf_len == 40 )	// InterfaceType = "IDE"
	{	// 需要将16进制编码串转换为字符串
		TCHAR ch, szBuf[32];
		BYTE b;		

		for( i = 0; i < 20; i++ )
		{	// 将16进制字符转换为高4位
			ch = serial_number[i * 2];
			if( (ch >= '0') && (ch <= '9') )
			{
				b = ch - '0';
			}
			else if( (ch >= 'A') && (ch <= 'F') )
			{
				b = ch - 'A' + 10;
			}
			else if( (ch >= 'a') && (ch <= 'f') )
			{
				b = ch - 'a' + 10;
			}
			else
			{	// 非法字符
				break;
			}

			b <<= 4;

			// 将16进制字符转换为低4位
			ch = serial_number[i * 2 + 1];
			if( (ch >= '0') && (ch <= '9') )
			{
				b += ch - '0';
			}
			else if( (ch >= 'A') && (ch <= 'F') )
			{
				b += ch - 'A' + 10;
			}
			else if( (ch >= 'a') && (ch <= 'f') )
			{
				b += ch - 'a' + 10;
			}
			else
			{	// 非法字符
				break;
			}

			szBuf[i] = b;
		}

		if( i == 20 )
		{	// 转换成功
			szBuf[i] = L'\0';
			StringCchCopy( serial_number, uSize, szBuf );
			out_buf_len = _tcslen( serial_number );
		}
	}

	// 每2个字符互换位置
	for( i = 0; i < out_buf_len; i += 2 )
	{
		std::swap( serial_number[i], serial_number[i+1] );
	}

	// 去掉空格
	std::remove( serial_number, serial_number + _tcslen(serial_number) + 1, L' ' );

	return TRUE;
}

static BOOL QueryProperty( INT query_type, TCHAR *szProperty, UINT uSize )
{
	BOOL isOK = TRUE;

	switch( query_type )
	{
	case 0:		// 网卡原生MAC地址		
		isOK = QueryPNPdeviceID( szProperty, szProperty, uSize );
		break;

	case 1:		// 硬盘序列号
		isOK = QueryHardDiskSerialNumber( szProperty, uSize );
		break;

	case 6:		// 网卡当前MAC地址
		// 去掉冒号
		std::remove( szProperty, szProperty + _tcslen(szProperty) + 1, L':' );
		break;

	default:
		// 去掉空格
		std::remove( szProperty, szProperty + _tcslen(szProperty) + 1, L' ' );
	}

	return isOK;
}

static BOOL isSupportDiskWMI()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	OSVERSIONINFOEX os;
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if (GetVersionEx((OSVERSIONINFO *)&os))
	{
		//vista之前不支持
		if (os.dwMajorVersion <= 5)
		{
			return FALSE;
		}
	}
	return TRUE;
}

// 基于Windows Management Instrumentation（Windows管理规范）
// 参照MSDN例子
INT __stdcall DeviceInfoQuery( INT query_type, T_DEVICE_PROPERTY *properties, INT list_size )
{
	////定义COM调用的返回  HRESULT com返回类型
	HRESULT hres;
	INT	devs_total = 0;
	
	if (properties == NULL)
	{
		return -1;
	}

	// 判断查询类型是否支持
	if( (query_type < 0) || (query_type >= sizeof(szWQLQuery)/sizeof(T_WQL_QUERY)) )
	{
		cout << "Unsupport QueryType" << endl;
		return -1;	// 查询类型不支持
	}
	// 硬盘序列号获取采用查询硬件的方式获取,XP不支持WMI方式获取硬盘信息
/*
	if (query_type == 1 && !isSupportDiskWMI()) {
		DiskInfo handle = DiskInfo::GetDiskInfo();
		UINT count = handle.LoadDiskInfo();
		if (count > 0) {
			for (UINT i = 0; i < count; i++) {
				if (devs_total >= list_size)
				{
					break;
				}
				char* serial_number = handle.serial_number(i);
				int out_buf_length;
				out_buf_length = MultiByteToWideChar(CP_ACP, 0, serial_number, strlen(serial_number) + 1, NULL, 0);
				MultiByteToWideChar(CP_ACP, 0, serial_number, strlen(serial_number) + 1, properties[devs_total].szProperty, out_buf_length);
				devs_total++;
			}
		} else {
			UINT zeroRightCount = handle.m_serizalNoVec.size();
			for (UINT i = 0; i < zeroRightCount; i++) {
				if (devs_total >= list_size)
				{
					break;
				}
				char* serial_number = handle.m_serizalNoVec[i];
				int out_buf_length;
				out_buf_length = MultiByteToWideChar(CP_ACP, 0, serial_number, strlen(serial_number) + 1, NULL, 0);
				MultiByteToWideChar(CP_ACP, 0, serial_number, strlen(serial_number) + 1, properties[devs_total].szProperty, out_buf_length);
				devs_total++;
			}
		}
		return devs_total;
	}
*/
    // step1 初始化COM
    hres = CoInitializeEx( NULL, COINIT_MULTITHREADED ); 
    if( FAILED(hres) )
    {
		cout << "Failed to initialize COM library. Error code = 0x"
			<< hex << hres << endl;
        return -2;
    }

    // step2 设置COM的安全认证级别
	hres = CoInitializeSecurity( 
		NULL, 
		-1, 
		NULL, 
		NULL, 
		RPC_C_AUTHN_LEVEL_DEFAULT, 
		RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
		);
	if( FAILED(hres) && hres != RPC_E_TOO_LATE)
    {
		cout << "Failed to initialize security. Error code = 0x"
			<< hex << hres << endl;
        CoUninitialize();
        return -2;
    }
    
	// step3 获得WMI连接COM接口
	// 创建一个CLSID_WebmLocator对象
    IWbemLocator *pLoc = NULL;
    hres = CoCreateInstance( 
		CLSID_WbemLocator,             
        NULL, 
        CLSCTX_INPROC_SERVER, 
        IID_IWbemLocator,
		reinterpret_cast<LPVOID*>(&pLoc)
		); 
    if( FAILED(hres) )
    {
		CoUninitialize();
        return -2;
    }

    // step4 通过连接接口连接WMI的内核对象名"ROOT\\CIMV2"
	IWbemServices *pSvc = NULL;
	hres = pLoc->ConnectServer(
         _bstr_t( L"ROOT\\CIMV2" ),
         NULL,
         NULL,
         NULL,
         0,
         NULL,
         NULL,
         &pSvc
		 );    
    if( FAILED(hres) )
    {
		cout << "Could not connect. Error code = 0x"
			<< hex << hres << endl;
		pLoc->Release(); 
        CoUninitialize();
        return -2;
    }

	// step5 设置请求代理的安全级别
    hres = CoSetProxyBlanket(
		pSvc,
		RPC_C_AUTHN_WINNT,
		RPC_C_AUTHZ_NONE,
		NULL,
		RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE
		);
	if( FAILED(hres) )
    {
		cout << "Could not set proxy blanket. Error code = 0x"
			<< hex << hres << endl;
        pSvc->Release();
        pLoc->Release();     
        CoUninitialize();
        return -2;
    }

    // step6 通过请求代理来向WMI发送请求
    IEnumWbemClassObject *pEnumerator = NULL;
    hres = pSvc->ExecQuery(
		bstr_t("WQL"), 
		bstr_t( szWQLQuery[query_type].szSelect ),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, 
        NULL,
        &pEnumerator
		);
	if( FAILED(hres) )
    {
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return -3;
    }
	
    // step7 循环枚举所有的结果对象
	IWbemClassObject *pclsObj = NULL;
	ULONG uReturn = 0;
    while( pEnumerator )
    {
		if( (properties != NULL) && (devs_total >= list_size) )
		{
			break;
		}
        pEnumerator->Next(
			WBEM_INFINITE,
			1, 
            &pclsObj,
			&uReturn
			);
        if( uReturn == 0 )
        {
            break;
        }
		if( properties != NULL )
		{	// 获取属性值
			VARIANT vtProperty;
			//Variant 是一种特殊的数据类型，除了定长String数据及用户定义类型外，可以包含任何种类的数据。
			//Variant 也可以包含Empty、Error、Nothing及Null等特殊值。
			//可以用VarType函数或TypeName函数来决定如何处理 Variant 中的数据。
			VariantInit( &vtProperty );	
			hres = pclsObj->Get( szWQLQuery[query_type].szProperty, 0, &vtProperty, NULL, NULL );
			if (FAILED(hres))
			{
				cout << "Could not Get szProperty. Error code = 0x"
					<< hex << hres << endl;
				//释放资源
				CoUninitialize();
				return -2;
			}
			USES_CONVERSION; StringCchCopy( properties[devs_total].szProperty, PROPERTY_MAX_LEN, W2T(vtProperty.bstrVal) );  // 这里vtProperty类型为bstr
			VariantClear( &vtProperty );

			// 对属性值做进一步的处理
			if( QueryProperty( query_type, properties[devs_total].szProperty, PROPERTY_MAX_LEN ) )
			{
				devs_total++;
			}
		}
		else
		{
			devs_total++;
		}

		pclsObj->Release();
    } // End While

    // 释放资源
	pEnumerator->Release();
    pSvc->Release();
    pLoc->Release();
    CoUninitialize();
	
    return devs_total;
}
