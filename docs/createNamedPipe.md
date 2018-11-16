# CreateNamedPipe http://www.cs.rpi.edu/courses/fall01/os/CreateNamedPipe
- The CreateNamedPipe function creates an instance of a named pipe and returns a handle for subsequent pipe operations. A named pipe server process uses this function either to create the first instance of a specific named pipe and establish its basic attributes or to create a new instance of an existing named pipe.
-
HANDLE CreateNamedPipe(
  LPCTSTR lpName,         // pointer to pipe name
  DWORD dwOpenMode,       // pipe open mode
  DWORD dwPipeMode,       // pipe-specific modes
  DWORD nMaxInstances,    // maximum number of instances
  DWORD nOutBufferSize,   // output buffer size, in bytes
  DWORD nInBufferSize,    // input buffer size, in bytes
  DWORD nDefaultTimeOut,  // time-out time, in milliseconds
  LPSECURITY_ATTRIBUTES lpSecurityAttributes  // pointer to security attributes
);

Parameters
lpName
Pointer to the null-terminated string that uniquely identifies the pipe. The string must have the following form:
\\.\pipe\pipename

The pipename part of the name can include any character other than a backslash, including numbers and special characters. The entire pipe name string can be up to 256 characters long. Pipe names are not case sensitive.

dwOpenMode
Specifies the pipe access mode, the overlapped mode, the write-through mode, and the security access mode of the pipe handle.
CreateNamedPipe fails if dwOpenMode specifies any flags other than those listed in the following tables.

This parameter must specify one of the following pipe access mode flags. The same mode must be specified for each instance of the pipe:
Mode	Description
PIPE_ACCESS_DUPLEX	The pipe is bidirectional; both server and client processes can read from and write to the pipe. This mode gives the server the equivalent of GENERIC_READ | GENERIC_WRITE access to the pipe. The client can specify GENERIC_READ or GENERIC_WRITE, or both, when it connects to the pipe using the CreateFile function.
PIPE_ACCESS_INBOUND	The flow of data in the pipe goes from client to server only. This mode gives the server the equivalent of GENERIC_READ access to the pipe. The client must specify GENERIC_WRITE access when connecting to the pipe.
PIPE_ACCESS_OUTBOUND	The flow of data in the pipe goes from server to client only. This mode gives the server the equivalent of GENERIC_WRITE access to the pipe. The client must specify GENERIC_READ access when connecting to the pipe.

This parameter can also include either or both of the following flags, which enable write-through mode and overlapped mode. These modes can be different for different instances of the same pipe.
Mode	Description
FILE_FLAG_WRITE_THROUGH	Write-through mode is enabled. This mode affects only write operations on byte-type pipes and, then, only when the client and server processes are on different computers. If this mode is enabled, functions writing to a named pipe do not return until the data written is transmitted across the network and is in the pipe's buffer on the remote computer. If this mode is not enabled, the system enhances the efficiency of network operations by buffering data until a minimum number of bytes accumulate or until a maximum time elapses.
FILE_FLAG_OVERLAPPED	Overlapped mode is enabled. If this mode is enabled, functions performing read, write, and connect operations that may take a significant time to be completed can return immediately. This mode enables the thread that started the operation to perform other operations while the time-consuming operation executes in the background. For example, in overlapped mode, a thread can handle simultaneous input and output (I/O) operations on multiple instances of a pipe or perform simultaneous read and write operations on the same pipe handle. If overlapped mode is not enabled, functions performing read, write, and connect operations on the pipe handle do not return until the operation is finished. The ReadFileEx and WriteFileEx functions can only be used with a pipe handle in overlapped mode. The ReadFile, WriteFile, ConnectNamedPipe, and TransactNamedPipe functions can execute either synchronously or as overlapped operations.

This parameter can include any combination of the following security access mode flags. These modes can be different for different instances of the same pipe. They can be specified without concern for what other dwOpenMode modes have been specified.
Mode	Description
WRITE_DAC	The caller will have write access to the named pipe's discretionary access control list (ACL).
WRITE_OWNER	The caller will have write access to the named pipe's owner.
ACCESS_SYSTEM_SECURITY	The caller will have write access to the named pipe's SACL. For more information, see Access-Control Lists (ACLs) and SACL Access Right.

dwPipeMode
Specifies the type, read, and wait modes of the pipe handle.
One of the following type mode flags can be specified. The same type mode must be specified for each instance of the pipe. If you specify zero, the parameter defaults to byte-type mode.
Mode	Description
PIPE_TYPE_BYTE	Data is written to the pipe as a stream of bytes. This mode cannot be used with PIPE_READMODE_MESSAGE.
PIPE_TYPE_MESSAGE	Data is written to the pipe as a stream of messages. This mode can be used with either PIPE_READMODE_MESSAGE or PIPE_READMODE_BYTE.

One of the following read mode flags can be specified. Different instances of the same pipe can specify different read modes. If you specify zero, the parameter defaults to byte-read mode.
Mode	Description
PIPE_READMODE_BYTE	Data is read from the pipe as a stream of bytes. This mode can be used with either PIPE_TYPE_MESSAGE or PIPE_TYPE_BYTE.
PIPE_READMODE_MESSAGE	Data is read from the pipe as a stream of messages. This mode can be only used if PIPE_TYPE_MESSAGE is also specified.

One of the following wait mode flags can be specified. Different instances of the same pipe can specify different wait modes. If you specify zero, the parameter defaults to blocking mode.
Mode	Description
PIPE_WAIT	Blocking mode is enabled. When the pipe handle is specified in the ReadFile, WriteFile, or ConnectNamedPipe function, the operations are not completed until there is data to read, all data is written, or a client is connected. Use of this mode can mean waiting indefinitely in some situations for a client process to perform an action.
PIPE_NOWAIT	Nonblocking mode is enabled. In this mode, ReadFile, WriteFile, and ConnectNamedPipe always return immediately. Note that nonblocking mode is supported for compatibility with Microsoft LAN Manager version 2.0 and should not be used to achieve asynchronous I/O with named pipes.

nMaxInstances
Specifies the maximum number of instances that can be created for this pipe. The same number must be specified for all instances. Acceptable values are in the range 1 through PIPE_UNLIMITED_INSTANCES. If this parameter is PIPE_UNLIMITED_INSTANCES, the number of pipe instances that can be created is limited only by the availability of system resources.
nOutBufferSize
Specifies the number of bytes to reserve for the output buffer. For a discussion on sizing named pipe buffers, see the following Remarks section.
nInBufferSize
Specifies the number of bytes to reserve for the input buffer. For a discussion on sizing named pipe buffers, see the following Remarks section.
nDefaultTimeOut
Specifies the default time-out value, in milliseconds, if the WaitNamedPipe function specifies NMPWAIT_USE_DEFAULT_WAIT. Each instance of a named pipe must specify the same value.
lpSecurityAttributes
Pointer to a SECURITY_ATTRIBUTES structure that specifies a security descriptor for the new named pipe and determines whether child processes can inherit the returned handle. If lpSecurityAttributes is NULL, the named pipe gets a default security descriptor and the handle cannot be inherited.
Return Values
If the function succeeds, the return value is a handle to the server end of a named pipe instance.

If the function fails, the return value is INVALID_HANDLE_VALUE. To get extended error information, call GetLastError. The return value is ERROR_INVALID_PARAMETER if nMaxInstances is greater than PIPE_UNLIMITED_INSTANCES.

Remarks
To create an instance of a named pipe by using CreateNamedPipe, the user must have FILE_CREATE_PIPE_INSTANCE access to the named pipe object. If a new named pipe is being created, the access control list (ACL) from the security attributes parameter defines the discretionary access control for the named pipe.

All instances of a named pipe must specify the same pipe type (byte-type or message-type), pipe access (duplex, inbound, or outbound), instance count, and time-out value. If different values are used, this function fails and GetLastError returns ERROR_ACCESS_DENIED.

The input and output buffer sizes are advisory. The actual buffer size reserved for each end of the named pipe is either the system default, the system minimum or maximum, or the specified size rounded up to the next allocation boundary.

The pipe server should not perform a blocking read operation until the pipe client has started. Otherwise, a race condition can occur. This typically occurs when initialization code, such as the C run-time, needs to lock and examine inherited handles.

An instance of a named pipe is always deleted when the last handle to the instance of the named pipe is closed.

QuickInfo
  Windows NT: Requires version 3.1 or later.
  Windows: Unsupported.
  Windows CE: Unsupported.
  Header: Declared in winbase.h.
  Import Library: Use kernel32.lib.
  Unicode: Implemented as Unicode and ANSI versions on Windows NT.

See Also
Pipes Overview, Pipe Functions, ConnectNamedPipe, CreateFile, ReadFile, ReadFileEx, SECURITY_ATTRIBUTES, TransactNamedPipe, WaitNamedPipe, WriteFile, WriteFileEx



CreateNamedPipe
ClearSeve'BlogHomePreviousNext


CreateNamedPipe函数创建一个命名管道的实例，并返回一个后续管道操作的句柄。命名管道服务器进程使用此函数来创建特定命名管道的第一个实例，并建立其基本属性或创建现有命名管道的新实例。

HANDLE CreateNamedPipe（

LPCTSTR 【lpName】,	//指向管道名称的指针
DWORD 【dwOpenMode】,	//管道打开模式
DWORD 【dwPipeMode】,	//管道特定模式
DWORD 【nMaxInstances】,	//最大实例数
DWORD 【nOutBufferSize】,	//输出缓冲区大小，以字节为单位
DWORD 【nInBufferSize】,	//输入缓冲区大小，以字节为单位
DWORD 【nDefaultTimeOut】,	//超时时间，以毫秒为单位
LPSECURITY_ATTRIBUTES 【lpSecurityAttributes】	//指向安全属性结构的指针
);

参数

【lpName】

指向唯一标识管道的以null结尾的字符串。字符串必须具有以下格式：

\\\\。\\管\\【\\管\\】

该名称的【\\管\\】部分可以包括除反斜杠之外的任何字符，包括数字和特殊字符。整个管道名称字符串最长可达256个字符。管道名称不区分大小写。

【dwOpenMode】

指定管道管理模式，重叠模式，直写模式和安全访问模式。

此参数必须指定以下管道访问模式标志之一。必须为管道的每个实例指定相同的模式：

模式	描述
PIPE_ACCESS_DUPLEX	管道是双向的服务器和客户端进程都可以读取和写入管道。该模式给予服务器等效于GENERIC_READ | GENERIC_WRITE访问管道。当使用CreateFile功能连接到管道时，客户端可以指定GENERIC_READ或GENERIC_WRITE，或两者。
PIPE_ACCESS_INBOUND	管道中的数据流仅从客户机到服务器。此模式使服务器等同于对管道的GENERIC_READ访问。连接到管道时，客户端必须指定GENERIC_WRITE访问。
PIPE_ACCESS_OUTBOUND	管道中的数据流从服务器到客户端。此模式使服务器等同于访问管道的GENERIC_WRITE。连接到管道时，客户端必须指定GENERIC_READ访问。

此参数还可以包括以下标志之一或两者，这些标志使能直写模式和重叠模式。对于同一管道的不同实例，这些模式可能不同。

模式	描述
FILE_FLAG_WRITE_THROUGH
 	直通模式启用。此模式仅影响字节类型管道上的写操作，然后只影响客户端和服务器进程在不同的计算机上。如果启用此模式，则写入命名管道的功能不会返回，直到写入的数据通过网络传输，并且位于远程计算机上的管道缓冲区中。如果未启用此模式，则系统通过缓冲数据来提高网络操作的效率，直到最小数量的字节累积或直到最大时间过去。
FILE_FLAG_OVERLAPPED
 	重叠模式启用。如果启用此模式，执行读取，写入和连接操作的功能可能需要大量时间才能完成，可以立即返回。该模式使得在后台执行耗时的操作时，开始操作的线程可以执行其他操作。例如，在重叠模式下，线程可以处理多个管道实例上的同时输入和输出（I / O）操作，或者在同一个管道句柄上执行同时的读取和写入操作。如果未启用重叠模式，则在操作完成之前，对管道手柄执行读取，写入和连接操作的功能不会返回。ReadFileEx和WriteFileEx功能只能与重叠模式下的管道手柄一起使用。ReadFile，WriteFile，ConnectNamedPipe和TransactNamedPipe功能可以同步或重叠操作执行。

此参数可以包括以下安全访问模式标志的任意组合。对于同一管道的不同实例，这些模式可能不同。可以指定它们，而不用担心指定了其他【dwOpenMode】模式。

模式	描述
WRITE_DAC	调用者将具有对命名管道的自由访问控制列表（ACL）的写入权限。
WRITE_OWNER	呼叫者将具有对命名管道所有者的写入权限。
ACCESS_SYSTEM_SECURITY	调用者将具有对命名管道系统ACL的写访问权限。

【dwPipeMode】

指定管道手柄的类型，读取和等待模式。

可以指定以下类型模式标志之一。必须为管道的每个实例指定相同类型的模式。如果指定为零，则该参数默认为字节型模式。

模式	描述
PIPE_TYPE_BYTE	数据作为字节流写入管道。此模式不能与PIPE_READMODE_MESSAGE一起使用。
PIPE_TYPE_MESSAGE	数据作为消息流写入管道。该模式可以与PIPE_READMODE_MESSAGE或PIPE_READMODE_BYTE一起使用。

可以指定以下读取模式标志之一。相同管道的不同实例可以指定不同的读取模式。如果指定为零，则该参数默认为字节读取模式。

模式	描述
PIPE_READMODE_BYTE	数据作为字节流从管道读取。此模式可与PIPE_TYPE_MESSAGE或PIPE_TYPE_BYTE一起使用。
PIPE_READMODE_MESSAGE	从管道读取数据作为消息流。只有当PIPE_TYPE_MESSAGE也被指定时，才能使用此模式。

可以指定以下等待模式标志之一。相同管道的不同实例可以指定不同的等待模式。如果指定为零，则该参数默认为阻塞模式。

模式	描述
PIPE_WAIT	禁止模式启用。当在ReadFile，WriteFile或ConnectNamedPipe功能中指定管道手柄时，只有在要读取数据，所有数据被写入或客户端连接之后，操作才能完成。使用此模式可能意味着在某些情况下无限期等待客户端进程执行操作。
PIPE_NOWAIT	启用非阻塞模式。在此模式下，ReadFile，WriteFile和ConnectNamedPipe始终立即返回。请注意，与Microsoft LAN Manager 2.0版兼容，支持非阻塞模式，不应用于使用命名管道实现异步I / O。

【nMaxInstances】

指定可以为此管道创建的最大实例数。必须为所有实例指定相同的编号。可接受的值在1到PIPE_UNLIMITED_INSTANCES之间。如果此参数是PIPE_UNLIMITED_INSTANCES，则可以创建的管道实例的数量仅受系统资源的可用性的限制。

【nOutBufferSize】

指定要为输出缓冲区保留的字节数。有关调整命名管道缓冲区大小的讨论，请参阅以下备注部分。

【nInBufferSize】

指定要为输入缓冲区预留的字节数。有关调整命名管道缓冲区大小的讨论，请参阅以下备注部分。

【nDefaultTimeOut】

如果WaitNamedPipe函数指定NMPWAIT_USE_DEFAULT_WAIT，则指定默认超时值（以毫秒为单位）。命名管道的每个实例都必须指定相同的值。

【lpSecurityAttributes】

指向SECURITY_ATTRIBUTES结构的指针，指定新命名管道的安全描述符，并确定子进程是否可以继承返回的句柄。如果【lpSecurityAttributes】为NULL，则命名管道将获取默??认安全描述符，并且该句柄不能被继承。

返回值

如果函数成功，则返回值是指定管道实例的服务器端的句柄。

如果函数失败，返回值为INVALID_HANDLE_VALUE。要获取扩展错误信息，请调用GetLastError.如果【nMaxInstances】大于PIPE_UNLIMITED_INSTANCES，返回值为ERROR_INVALID_PARAMETER。

备注

要使用CreateNamedPipe创建命名管道的实例，用户必须对命名的管道对象具有FILE_CREATE_PIPE_INSTANCE访问权限。如果正在创建新的命名管道，则来自security attributes参数的访问控制列表（ACL）定义了命名管道的自由访问控制。

命名管道的所有实例必须指定相同的管道类型（字节类型或消息类型），管道访问（双工，入站或出站），实例计数和超时值。如果使用不同的值，则此函数失败，GetLastError返回ERROR_ACCESS_DENIED。

输入和输出缓冲区大小是咨询。为命名管道的每一端保留的实际缓冲区大小是系统默认值，系统最小值或最大值，或指定大小向上舍入到下一个分配边界。

当命名管道的实例的最后一个句柄关闭时，总是删除命名管道的实例。

也可以看看

ConnectNamedPipe, CreateFile, ReadFile, ReadFileEx, SECURITY_ATTRIBUTES, TransactNamedPipe, WaitNamedPipe, WriteFile, WriteFileEx