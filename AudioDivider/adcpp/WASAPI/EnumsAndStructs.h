#pragma once

namespace AudioDivider
{
	namespace WASAPI
	{
		enum class ERole
		{
			eConsole = 0,
			eMultimedia = 1,
			eCommunications = 2,
			ERole_enum_count = 3
		};

		enum class EDataFlow
		{
			eRender = 0,
			eCapture = 1,
			eAll = 2,
			EDataFlow_enum_count = 3
		};

		enum class DeviceStatemask : int
		{
			DEVICE_STATE_ACTIVE = 0x00000001,
			DEVICE_STATE_DISABLED = 0x00000002,
			DEVICE_STATE_NOTPRESENT = 0x00000004,
			DEVICE_STATE_UNPLUGGED = 0x00000008,
			DEVICE_STATEMASK_ALL = 0x0000000f
		};

		enum class ProperyStoreMode
		{
			STGM_READ = 0x00000000,
			STGM_WRITE = 0x00000001,
			STGM_READWRITE = 0x00000002
		};

		enum class ClsCtx
		{
			CLSCTX_ALL = 16 | 4 | 2 | 1
		};

		enum class AudioSessionState
		{
			AudioSessionStateInactive = 0,
			AudioSessionStateActive = 1,
			AudioSessionStateExpired = 2
		};

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [StructLayout(LayoutKind.Sequential)] public struct PropertyKey
		class PropertyKey
		{
		public:
			Guid fmtid;
			int pid = 0;
		};

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [StructLayout(LayoutKind.Explicit)] public struct PropVariant
		class PropVariant
		{
		public:
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(0)] public short vt;
			short vt = 0;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(2)] public short wReserved1;
			short wReserved1 = 0;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(4)] public short wReserved2;
			short wReserved2 = 0;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(6)] public short wReserved3;
			short wReserved3 = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(8)] public sbyte cVal;
			char cVal = 0;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(8)] public byte bVal;
			unsigned char bVal = 0;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(8)] public short iVal;
			short iVal = 0;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(8)] public ushort uiVal;
			unsigned short uiVal = 0;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(8)] public int lVal;
			int lVal = 0;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(8)] public uint ulVal;
			unsigned int ulVal = 0;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(8)] public int intVal;
			int intVal = 0;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(8)] public uint uintVal;
			unsigned int uintVal = 0;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(8)] public long hVal;
			long long hVal = 0;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(8)] public ulong uhVal;
			unsigned long long uhVal = 0;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(8)] public float fltVal;
			float fltVal = 0;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(8)] public double dblVal;
			double dblVal = 0;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(8)] public IntPtr pszVal;
			void* pszVal;
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [FieldOffset(8)] public IntPtr pwszVal;
			void* pwszVal;
		};

		class WaveFormatEx
		{

		};

		enum class DeviceShareMode
		{
		};
	}
}
