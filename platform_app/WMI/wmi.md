### Windows Management Instrumentation（WMI）
是微软提供的一组系统管理技术，它以 COM 接口的形式暴露出来，可以通过编程语言（如 C++、C#、VBScript、PowerShell 等）来访问。WMI 基于标准的 Distributed Management Task Force （DMTF）规范，提供了一种标准化的接口， 可以访问操作系统、应用程序和网络设备等管理信息。
WMI 提供了一种基于对象的编程模型，将系统管理信息封装成对象，使得开发人员可以通过对象的属性、方法和事件等方式来访问和控制系统管理信息。
WMI 可以用于管理各种 Windows 系统，包括本地计算机和远程计算机。它可以访问各种系统信息，例如硬件、软件、网络配置、系统安全等，以及可以执行各种系统管理任务，例如启动/停止服务、安装/卸载软件、创建/删除用户等。通过 WMI，管理员可以编写脚本和程序来自动化各种管理任务，提高工作效率。
WMI 的核心是 WMI Provider，它是一个 COM 组件，负责向 WMI 提供系统管理信息。Windows 操作系统和许多应用程序都提供了 WMI Provider，管理员可以使用 WMI 编程语言来访问这些 Provider，获取系统管理信息和执行管理任务。此外，开发人员也可以创建自己的 WMI Provider，向 WMI 提供自定义的系统管理信息。
总的来说，WMI 是一种功能强大的系统管理技术，可以帮助管理员更轻松地管理 Windows 系统，提高工作效率。

### IWbemServices 
是 Microsoft Windows Management Instrumentation (WMI) API 中的一个接口，它提供了一组用于管理 WMI 命名空间中的对象和执行 WMI 查询的方法。
WMI 是一种管理和监视 Windows 操作系统及其组件的技术，它使用一组标准化的命名空间和对象模型来表示系统资源和状态。IWbemServices 接口允许应用程序通过编程方式访问和操作 WMI 数据，并提供了一些常见的管理任务的实现，例如启动进程、查询事件日志、配置服务等。

