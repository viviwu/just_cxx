
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
