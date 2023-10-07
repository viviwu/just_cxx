
IP_ADAPTER_INFO是一个结构体，它提供了有关计算机网络适配器的信息，包括其IP地址、MAC地址、子网掩码、默认网关等。下面是IP_ADAPTER_INFO结构体中各字段的含义：

1. `ComboIndex`: 适配器的组合索引。如果计算机有多个适配器，这个值可以帮助区分它们。
2. `AdapterName`: 适配器的名称。
3. `Description`: 适配器的描述。
4. `AddressLength`: 适配器地址的字节数。
5. `Address`: 适配器的物理地址（MAC地址）。
6. `Index`: 适配器的索引。
7. `Type`: 适配器的类型，如以太网、无线网卡等。
8. `DhcpEnabled`: 指示适配器是否启用了DHCP。
9. `CurrentIpAddress`: 适配器的当前IP地址。
10. `IpAddressList`: 适配器的IP地址列表。
11. `GatewayList`: 适配器的默认网关列表。
12. `DhcpServer`: 分配DHCP地址的服务器的IP地址。
13. `HaveWins`: 指示适配器是否使用了WINS服务器。
14. `PrimaryWinsServer`: 适配器的主要WINS服务器的IP地址。
15. `SecondaryWinsServer`: 适配器的次要WINS服务器的IP地址。
16. `LeaseObtained`: DHCP租约的起始时间。
17. `LeaseExpires`: DHCP租约的到期时间。

这些字段提供了有关计算机网络适配器的详细信息，可以帮助开发人员编写网络应用程序。


在使用 WMI 查询获取硬件信息时，需要对 WMI 服务设置安全权限，以确保程序可以访问所需的 WMI 类。在 Windows 操作系统中，可以使用 CoSetProxyBlanket 函数来设置安全权限。
该函数有四个参数，分别是：
pProxy：指向需要设置安全权限的 COM 代理的指针。
dwAuthnSvc：指定身份验证服务的 ID。
dwAuthzSvc：指定授权服务的 ID。
pServerPrincName：指向服务器的主体名称的指针。

要获取管理员权限，可以使用 Windows 的 UAC（User Account Control）功能，通过弹出 UAC 对话框来请求用户允许程序以管理员身份运行。具体来说，可以使用以下步骤来请求管理员权限：

1. 在应用程序清单文件（.manifest 文件）中添加以下内容：

   ````xml
   <requestedExecutionLevel level="requireAdministrator" uiAccess="false" />
   ```

   这个清单文件需要被嵌入到你的可执行文件中，可以使用 Visual Studio 等开发工具来创建和嵌入清单文件。

2. 在程序启动时，使用 Windows API 函数 `ShellExecute()` 来启动自己的程序，同时指定 `runas` 参数，表示以管理员身份运行：

   ````c++
   #include <Windows.h>

   int main() {
       ShellExecute(NULL, L"runas", L"your_program.exe", NULL, NULL, SW_SHOWNORMAL);
       return 0;
   }
   ```

   注意，如果用户拒绝了 UAC 对话框，`ShellExecute()` 函数将返回错误码 `ERROR_CANCELLED`。

这样，当用户运行你的程序时，如果程序需要管理员权限，Windows 将弹出 UAC 对话框来请求用户授权，用户授权后程序就可以以管理员身份运行了。

#include <Windows.h>

int wmain() { // 注意这里使用 wmain() 函数，以支持 Unicode 字符集
LPCWSTR lpOperation = L"runas"; // 指定以管理员身份运行
LPCWSTR lpFile = L"notepad.exe"; // 要运行的程序
LPCWSTR lpParameters = NULL; // 程序参数（可选）
LPCWSTR lpDirectory = NULL; // 程序工作目录（可选）
INT nShowCmd = SW_SHOWNORMAL; // 窗口显示方式（可选）

    HINSTANCE hInst = ShellExecute(NULL, lpOperation, lpFile, lpParameters, lpDirectory, nShowCmd);
    if ((DWORD_PTR)hInst <= 32) {
        // 调用失败，输出错误信息
        LPVOID lpMsgBuf;
        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
        wprintf(L"Failed to execute program. Error code: %d. Error message: %s\n", GetLastError(), lpMsgBuf);
        LocalFree(lpMsgBuf);
        return 1;
    }

    return 0;
}