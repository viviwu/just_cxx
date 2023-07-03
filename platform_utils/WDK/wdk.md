WDK��Windows Driver Kit�� 

Windows��������
-DeviceIoControl �����﷨ 
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

�ڶ�������dwIoControlCode������CTL_CODE�궨���
#define CTL_CODE(DeviceType, Function, Method, Access) (  ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))

CTL_CODE�����ڴ���һ��Ψһ��32λϵͳI/O���ƴ��룬������ƴ������4������ɣ�
DeviceType���豸���ͣ���16λ��16-31λ������
Access���������ƣ�14-15λ����
Function������2-13λ����
Method��I/O�����ڴ�ʹ�÷�ʽ����

DeviceType���ӣ�16-31λ
#define FILE_DEVICE_HOLOGRAPHIC         0x0000005b
#define FILE_DEVICE_SDFXHCI             0x0000005c
#define FILE_DEVICE_UCMUCSI             0x0000005d

Access���ӣ����2������λ�͹���14��15λ
#define FILE_ANY_ACCESS                 0
#define FILE_SPECIAL_ACCESS    (FILE_ANY_ACCESS)
#define FILE_READ_ACCESS          ( 0x0001 )    // file & pipe
#define FILE_WRITE_ACCESS         ( 0x0002 )    // file & pipe

Method�����3������λ��0��1λ
#define METHOD_BUFFERED                 0
#define METHOD_IN_DIRECT                1
#define METHOD_OUT_DIRECT               2
#define METHOD_NEITHER                  3

Function��ռ12��λ��2-13λreserved
Function codes 0-2047(0x000-0x7FF) are reserved for Microsoft;
codes 2048-4095(0x800-0xFFF) are reserved for OEMs and IHVs


HDEVINFO SetupDiGetClassDevsA(
_In_opt_ CONST GUID *ClassGuid,
_In_opt_ PCSTR Enumerator,
_In_opt_ HWND hwndParent,
_In_ DWORD Flags
);
