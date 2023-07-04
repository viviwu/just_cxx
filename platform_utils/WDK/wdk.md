WDK��Windows Driver Kit�� 

Windows��������
-DeviceIoControl �����﷨ 
***
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
***
�ڶ�������dwIoControlCode������CTL_CODE�궨���

> #### #define CTL_CODE(DeviceType, Function, Method, Access) (  ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method))

CTL_CODE�����ڴ���һ��Ψһ��32λϵͳI/O���ƴ��룬������ƴ������4������ɣ�
DeviceType���豸���ͣ���16λ��16-31λ������
Access���������ƣ�14-15λ����
Function������2-13λ����
Method��I/O�����ڴ�ʹ�÷�ʽ����

DeviceType���ӣ�16-31λ
***
#### #define FILE_DEVICE_HOLOGRAPHIC         0x0000005b
#### #define FILE_DEVICE_SDFXHCI             0x0000005c
#### #define FILE_DEVICE_UCMUCSI             0x0000005d
***
Access���ӣ����2������λ�͹���14��15λ
***
#### #define FILE_ANY_ACCESS                 0
#### #define FILE_SPECIAL_ACCESS    (FILE_ANY_ACCESS)
#### #define FILE_READ_ACCESS          ( 0x0001 )    // file & pipe
#### #define FILE_WRITE_ACCESS         ( 0x0002 )    // file & pipe
***
Method�����3������λ��0��1λ
***
#### #define METHOD_BUFFERED                 0
#### #define METHOD_IN_DIRECT                1
#### #define METHOD_OUT_DIRECT               2
#### #define METHOD_NEITHER                  3
***
Function��ռ12��λ��2-13λreserved
Function codes 0-2047(0x000-0x7FF) are reserved for Microsoft;
codes 2048-4095(0x800-0xFFF) are reserved for OEMs and IHVs


HDEVINFO SetupDiGetClassDevsA(
_In_opt_ CONST GUID *ClassGuid,
_In_opt_ PCSTR Enumerator,
_In_opt_ HWND hwndParent,
_In_ DWORD Flags
);

***
### ClassGuid: 
> һ���ض����GUID����Ҫ��ѯע�����ָ�룬��������� DIGCF_ALLCLASSES��ǣ��ò��������ԣ����������������豸��Ϣ��
�������GUID���豸��װ���GUID���豸�ӿ����GUID����

|Identifier | Class GUID | Header |
|:---|:---|:---:|
|GUID_DEVINTERFACE_USB_DEVICE | {A5DCBF10-6530-11D2-901F-00C04FB951ED} | Usbiodef.h |
|GUID_DEVINTERFACE_USB_HOST_CONTROLLER | {3ABF6F2D-71C4-462A-8A92-1E6861E6AF27} | Usbiodef.h |
|GUID_DEVINTERFACE_USB_HUB | {F18A0E88-C30C-11D0-8815-00A0C906BED8} | Usbiodef.h |
|GUID_DEVINTERFACE_NET | {CAC88484-7515-4C03-82E6-71A87ABAC361} | Ndisguid.h |
|GUID_DEVINTERFACE_MODEM | {2C7089AA-2E0E-11D1-B114-00C04FC2AAE4} | Ntddmodm.h |
|GUID_DEVINTERFACE_DISK | {53F56307-B6BF-11D0-94F2-00A0C91EFB8B} | Ntddstor.h |
|GUID_DEVINTERFACE_VOLUME | {53F5630D-B6BF-11D0-94F2-00A0C91EFB8B} | Ntddstor.h |
|GUID_DEVINTERFACE_MEDIUMCHANGER | {53F56310-B6BF-11D0-94F2-00A0C91EFB8B} | Ntddstor.h |
|GUID_DEVINTERFACE_CDROM | {53F56308-B6BF-11D0-94F2-00A0C91EFB8B} | Ntddstor.h |
|GUID_DEVINTERFACE_PARTITION | {53F5630A-B6BF-11D0-94F2-00A0C91EFB8B} | Ntddstor.h |
|GUID_DEVINTERFACE_HID | {4D1E55B2-F16F-11CF-88CB-001111000030} | Hidclass.h |
|GUID_DEVINTERFACE_KEYBOARD | {884B96C3-56EF-11D1-BC8C-00A0C91405DD} | Ntddkbd.h |
|GUID_DEVINTERFACE_MOUSE | {378DE44C-56EF-11D1-BC8C-00A0C91405DD} | Ntddmou.h |
|GUID_DEVINTERFACE_DISPLAY_ADAPTER | {5B45201D-F2F2-4F3B-85BB-30FF1F953599} | Ntddvdeo.h |
|GUID_DEVINTERFACE_IMAGE | {6BDD1FC6-810F-11D0-BEC7-08002BE2092F} | Wiaintfc.h |
|GUID_DEVINTERFACE_MONITOR | {E6F07B5F-EE97-4a90-B076-33F57BF4EAA7} | Ntddvdeo.h |
|GUID_DEVINTERFACE_BRIGHTNESS | {FDE5BBA4-B3F9-46FB-BDAA-0728CE3100B4} | Dispmprt.h |
|GUID_DEVINTERFACE_I2C | {2564AA4F-DDDB-4495-B497-6AD4A84163D7} | Dispmprt.h |
|GUID_BTHPORT_DEVICE_INTERFACE | {0850302A-B344-4fda-9BE9-90576B8D46F0} | Bthdef.h |
|GUID_DEVINTERFACE_COMPORT | {86E0D1E0-8089-11D0-9CE4-08003E301F73} | Ntddser.h |
|GUID_DEVINTERFACE_PARCLASS | {811FC6A5-F728-11D0-A537-0000F8753ED1} | Ntddpar. |

***