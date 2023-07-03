WDK（Windows Driver Kit） 

Windows驱动开发
-DeviceIoControl 函数语法 
BOOL DeviceIoControl(
HANDLE       hDevice,
DWORD        dwIoControlCode,
LPVOID       lpInBuffer,
DWORD        nInBufferSize,
LPVOID       lpOutBuffer,
DWORD        nOutBufferSize,
LPDWORD      lpBytesReturned,
LPOVERLAPPED lpOverlapped
);

第二个参数dwIoControlCode就是由CTL_CODE宏定义的
#define CTL_CODE(DeviceType, Function, Method, Access) (  ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))

CTL_CODE：用于创建一个唯一的32位系统I/O控制代码，这个控制代码包括4部分组成：
DeviceType（设备类型，高16位（16-31位）），
Access（访问限制，14-15位），
Function（功能2-13位），
Method（I/O访问内存使用方式）。

DeviceType例子：16-31位
#define FILE_DEVICE_HOLOGRAPHIC         0x0000005b
#define FILE_DEVICE_SDFXHCI             0x0000005c
#define FILE_DEVICE_UCMUCSI             0x0000005d

Access例子：最大2，两个位就够，14，15位
#define FILE_ANY_ACCESS                 0
#define FILE_SPECIAL_ACCESS    (FILE_ANY_ACCESS)
#define FILE_READ_ACCESS          ( 0x0001 )    // file & pipe
#define FILE_WRITE_ACCESS         ( 0x0002 )    // file & pipe

Method：最大3，两个位，0，1位
#define METHOD_BUFFERED                 0
#define METHOD_IN_DIRECT                1
#define METHOD_OUT_DIRECT               2
#define METHOD_NEITHER                  3

Function：占12个位，2-13位reserved
Function codes 0-2047(0x000-0x7FF) are reserved for Microsoft;
codes 2048-4095(0x800-0xFFF) are reserved for OEMs and IHVs


HDEVINFO SetupDiGetClassDevsA(
_In_opt_ CONST GUID *ClassGuid,
_In_opt_ PCSTR Enumerator,
_In_opt_ HWND hwndParent,
_In_ DWORD Flags
);
