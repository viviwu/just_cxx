
IP_ADAPTER_INFO��һ���ṹ�壬���ṩ���йؼ������������������Ϣ��������IP��ַ��MAC��ַ���������롢Ĭ�����صȡ�������IP_ADAPTER_INFO�ṹ���и��ֶεĺ��壺

1. `ComboIndex`: ��������������������������ж�������������ֵ���԰����������ǡ�
2. `AdapterName`: �����������ơ�
3. `Description`: ��������������
4. `AddressLength`: ��������ַ���ֽ�����
5. `Address`: �������������ַ��MAC��ַ����
6. `Index`: ��������������
7. `Type`: �����������ͣ�����̫�������������ȡ�
8. `DhcpEnabled`: ָʾ�������Ƿ�������DHCP��
9. `CurrentIpAddress`: �������ĵ�ǰIP��ַ��
10. `IpAddressList`: ��������IP��ַ�б�
11. `GatewayList`: ��������Ĭ�������б�
12. `DhcpServer`: ����DHCP��ַ�ķ�������IP��ַ��
13. `HaveWins`: ָʾ�������Ƿ�ʹ����WINS��������
14. `PrimaryWinsServer`: ����������ҪWINS��������IP��ַ��
15. `SecondaryWinsServer`: �������Ĵ�ҪWINS��������IP��ַ��
16. `LeaseObtained`: DHCP��Լ����ʼʱ�䡣
17. `LeaseExpires`: DHCP��Լ�ĵ���ʱ�䡣

��Щ�ֶ��ṩ���йؼ������������������ϸ��Ϣ�����԰���������Ա��д����Ӧ�ó���


��ʹ�� WMI ��ѯ��ȡӲ����Ϣʱ����Ҫ�� WMI �������ð�ȫȨ�ޣ���ȷ��������Է�������� WMI �ࡣ�� Windows ����ϵͳ�У�����ʹ�� CoSetProxyBlanket ���������ð�ȫȨ�ޡ�
�ú������ĸ��������ֱ��ǣ�
pProxy��ָ����Ҫ���ð�ȫȨ�޵� COM �����ָ�롣
dwAuthnSvc��ָ�������֤����� ID��
dwAuthzSvc��ָ����Ȩ����� ID��
pServerPrincName��ָ����������������Ƶ�ָ�롣

Ҫ��ȡ����ԱȨ�ޣ�����ʹ�� Windows �� UAC��User Account Control�����ܣ�ͨ������ UAC �Ի����������û���������Թ���Ա������С�������˵������ʹ�����²������������ԱȨ�ޣ�

1. ��Ӧ�ó����嵥�ļ���.manifest �ļ���������������ݣ�

   ````xml
   <requestedExecutionLevel level="requireAdministrator" uiAccess="false" />
   ```

   ����嵥�ļ���Ҫ��Ƕ�뵽��Ŀ�ִ���ļ��У�����ʹ�� Visual Studio �ȿ���������������Ƕ���嵥�ļ���

2. �ڳ�������ʱ��ʹ�� Windows API ���� `ShellExecute()` �������Լ��ĳ���ͬʱָ�� `runas` ��������ʾ�Թ���Ա������У�

   ````c++
   #include <Windows.h>

   int main() {
       ShellExecute(NULL, L"runas", L"your_program.exe", NULL, NULL, SW_SHOWNORMAL);
       return 0;
   }
   ```

   ע�⣬����û��ܾ��� UAC �Ի���`ShellExecute()` ���������ش����� `ERROR_CANCELLED`��

���������û�������ĳ���ʱ�����������Ҫ����ԱȨ�ޣ�Windows ������ UAC �Ի����������û���Ȩ���û���Ȩ�����Ϳ����Թ���Ա��������ˡ�

#include <Windows.h>

int wmain() { // ע������ʹ�� wmain() ��������֧�� Unicode �ַ���
LPCWSTR lpOperation = L"runas"; // ָ���Թ���Ա�������
LPCWSTR lpFile = L"notepad.exe"; // Ҫ���еĳ���
LPCWSTR lpParameters = NULL; // �����������ѡ��
LPCWSTR lpDirectory = NULL; // ������Ŀ¼����ѡ��
INT nShowCmd = SW_SHOWNORMAL; // ������ʾ��ʽ����ѡ��

    HINSTANCE hInst = ShellExecute(NULL, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);
    if ((DWORD_PTR)hInst <= 32) {
        // ����ʧ�ܣ����������Ϣ
        LPVOID lpMsgBuf;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
        wprintf(L"Failed to execute program. Error code: %d. Error message: %s\n", GetLastError(), lpMsgBuf);
        LocalFree(lpMsgBuf);
        return 1;
    }

    return 0;
}