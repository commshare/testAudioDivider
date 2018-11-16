#pragma once

#include "EnumsAndStructs.h"
#include <string>

//C# TO C++ CONVERTER NOTE: Forward class declarations:
namespace AudioDivider { namespace WASAPI { class IMMDeviceCollection; } }
namespace AudioDivider { namespace WASAPI { class IMMDevice; } }
namespace AudioDivider { namespace WASAPI { class IMMNotificationClient; } }
namespace AudioDivider { namespace WASAPI { class IMMPropertyStore; } }
namespace AudioDivider { namespace WASAPI { class PropertyKey; } }
namespace AudioDivider { namespace WASAPI { class PropVariant; } }
namespace AudioDivider { namespace WASAPI { class IAudioSessionControl; } }
namespace AudioDivider { namespace WASAPI { class ISimpleAudioVolume; } }
namespace AudioDivider { namespace WASAPI { class IAudioSessionEnumerator; } }
namespace AudioDivider { namespace WASAPI { class IAudioSessionNotification; } }
namespace AudioDivider { namespace WASAPI { class IAudioVolumeDuckNotification; } }
namespace AudioDivider { namespace WASAPI { class IAudioSessionEvents; } }
namespace AudioDivider { namespace WASAPI { class WaveFormatEx; } }


namespace AudioDivider
{
	namespace WASAPI
	{

		// #################### COM classes ####################

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [ComImport, Guid("BCDE0395-E52F-467C-8E3D-C4579291692E")] class DeviceEnumerator
		class DeviceEnumerator
		{

		};

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [ComImport, Guid("870af99c-171d-4f9e-af0d-e63df40c2bc9")] class IPolicyConfigClass
		class IPolicyConfigClass
		{

		};

		// #################### COM Interfaces ####################

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [ComImport, Guid("A95664D2-9614-4F35-A746-DE8DB63617E6"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)] public interface IMMDeviceEnumerator
		class IMMDeviceEnumerator
		{
		public:
			virtual IMMDeviceCollection *EnumAudioEndpoints(EDataFlow dataFlow, DeviceStatemask dwStateMask) = 0;

			virtual IMMDevice *GetDefaultAudioEndpoint(EDataFlow dataFlow, ERole role) = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: IMMDevice GetDevice([MarshalAs(UnmanagedType.LPWStr)] string pwstrId);
			virtual IMMDevice *GetDevice(const std::wstring &pwstrId) = 0;

			virtual void RegisterEndpointNotificationCallback(IMMNotificationClient *pClient) = 0;

			virtual void UnregisterEndpointNotificationCallback(IMMNotificationClient *pClient) = 0;
		};

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [ComImport, Guid("0BD7A1BE-7A1A-44DB-8397-CC5392387B5E"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)] public interface IMMDeviceCollection
		class IMMDeviceCollection
		{
		public:
			virtual unsigned int GetCount() = 0;

			virtual IMMDevice *Item(unsigned int nDevice) = 0;

		};

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [ComImport, Guid("D666063F-1587-4E43-81F1-B948E807363F"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)] public interface IMMDevice
		class IMMDevice
		{
		public:
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [return: MarshalAs(UnmanagedType.IUnknown)] object Activate(ref Guid iid, int dwClsCtx, IntPtr pActivationParams);
			virtual void *Activate(Guid &iid, int dwClsCtx, IntPtr pActivationParams) = 0;

			virtual IMMPropertyStore *OpenPropertyStore(ProperyStoreMode stgmAccess) = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [return: MarshalAs(UnmanagedType.LPWStr)] string GetId();
			virtual std::wstring GetId() = 0;

			virtual int GetState() = 0;

		};

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [ComImport, Guid("886d8eeb-8cf2-4446-8d02-cdba1dbdcf99"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)] public interface IMMPropertyStore
		class IMMPropertyStore
		{
		public:
			virtual int GetCount() = 0;

			virtual PropertyKey GetAt(int iProp) = 0;

			virtual PropVariant GetValue(PropertyKey &key) = 0;

			virtual void SetValue(PropertyKey &key, PropVariant &propvar) = 0;

			virtual void Commit() = 0;

		};

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [ComImport, Guid("BFA971F1-4D5E-40BB-935E-967039BFBEE4"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)] public interface IAudioSessionManager
		class IAudioSessionManager
		{
		public:
			virtual IAudioSessionControl *GetAudioSessionControl(Guid &AudioSessionGuid, int StreamFlags) = 0;

			virtual ISimpleAudioVolume *GetSimpleAudioVolume(Guid &AudioSessionGuid, int StreamFlags) = 0;
		};

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [ComImport, Guid("77AA99A0-1BD6-484F-8BC7-2C654C9A9B6F"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)] public interface IAudioSessionManager2
		class IAudioSessionManager2
		{
		public:
			virtual IAudioSessionControl *GetAudioSessionControl(Guid &AudioSessionGuid, int StreamFlags) = 0;

			virtual ISimpleAudioVolume *GetSimpleAudioVolume(Guid &AudioSessionGuid, int StreamFlags) = 0;



			virtual IAudioSessionEnumerator *GetSessionEnumerator() = 0;

			virtual void RegisterSessionNotification(IAudioSessionNotification *SessionNotification) = 0;

			virtual void UnregisterSessionNotification(IAudioSessionNotification *SessionNotification) = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: void RegisterDuckNotification([MarshalAs(UnmanagedType.LPWStr)] string sessionID, IAudioVolumeDuckNotification duckNotification);
			virtual void RegisterDuckNotification(const std::wstring &sessionID, IAudioVolumeDuckNotification *duckNotification) = 0;

			virtual void UnregisterDuckNotification(IAudioVolumeDuckNotification *duckNotification) = 0;
		};

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [ComImport, Guid("bfb7ff88-7239-4fc9-8fa2-07c950be9c6d"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)] public interface IAudioSessionControl2
		class IAudioSessionControl2
		{
		public:
			virtual AudioSessionState GetState() = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [return: MarshalAs(UnmanagedType.LPWStr)] string GetDisplayName();
			virtual std::wstring GetDisplayName() = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: void SetDisplayName([MarshalAs(UnmanagedType.LPWStr)] string Value, ref Guid EventContext);
			virtual void SetDisplayName(const std::wstring &Value, Guid &EventContext) = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [return: MarshalAs(UnmanagedType.LPWStr)] string GetIconPath();
			virtual std::wstring GetIconPath() = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: void SetIconPath([MarshalAs(UnmanagedType.LPWStr)] string Value, ref Guid EventContext);
			virtual void SetIconPath(const std::wstring &Value, Guid &EventContext) = 0;

			virtual void GetGroupingParam(Guid &pRetVal) = 0;

			virtual void SetGroupingParam(Guid &Override, Guid &EventContext) = 0;

			virtual void RegisterAudioSessionNotification(IAudioSessionEvents *NewNotifications) = 0;

			virtual void UnregisterAudioSessionNotification(IAudioSessionEvents *NewNotifications) = 0;



//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [return: MarshalAs(UnmanagedType.LPWStr)] string GetSessionIdentifier();
			virtual std::wstring GetSessionIdentifier() = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [return: MarshalAs(UnmanagedType.LPWStr)] string GetSessionInstanceIdentifier();
			virtual std::wstring GetSessionInstanceIdentifier() = 0;

			virtual int GetProcessId() = 0;

			virtual void IsSystemSoundsSession() = 0;

			virtual void SetDuckingPreference(int optOut) = 0;
		};

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [ComImport, Guid("F4B1A599-7266-4319-A8CA-E70ACB11E8CD"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)] public interface IAudioSessionControl
		class IAudioSessionControl
		{
		public:
			virtual AudioSessionState GetState() = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [return: MarshalAs(UnmanagedType.LPWStr)] string GetDisplayName();
			virtual std::wstring GetDisplayName() = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: void SetDisplayName([MarshalAs(UnmanagedType.LPWStr)] string Value, ref Guid EventContext);
			virtual void SetDisplayName(const std::wstring &Value, Guid &EventContext) = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [return: MarshalAs(UnmanagedType.LPWStr)] string GetIconPath();
			virtual std::wstring GetIconPath() = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: void SetIconPath([MarshalAs(UnmanagedType.LPWStr)] string Value, ref Guid EventContext);
			virtual void SetIconPath(const std::wstring &Value, Guid &EventContext) = 0;

			virtual void GetGroupingParam(Guid &pRetVal) = 0;

			virtual void SetGroupingParam(Guid &Override, Guid &EventContext) = 0;

			virtual void RegisterAudioSessionNotification(IAudioSessionEvents *NewNotifications) = 0;

			virtual void UnregisterAudioSessionNotification(IAudioSessionEvents *NewNotifications) = 0;
		};

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [ComImport, Guid("E2F5BB11-0570-40CA-ACDD-3AA01277DEE8"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)] public interface IAudioSessionEnumerator
		class IAudioSessionEnumerator
		{
		public:
			virtual int GetCount() = 0;

			virtual IAudioSessionControl *GetSession(int SessionCount) = 0;
		};

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: [ComImport, Guid("f8679f50-850a-41cf-9c72-430f290290c8"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)] interface IPolicyConfig
		class IPolicyConfig
		{

		public:
//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: void GetMixFormat([MarshalAs(UnmanagedType.LPWStr)] string param1, out object param2);
			virtual void GetMixFormat(const std::wstring &param1, void *&param2) = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: void GetDeviceFormat([MarshalAs(UnmanagedType.LPWStr)] string param1, int param2, out object param3);
			virtual void GetDeviceFormat(const std::wstring &param1, int param2, void *&param3) = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: void ResetDeviceFormat([MarshalAs(UnmanagedType.LPWStr)] string param1);
			virtual void ResetDeviceFormat(const std::wstring &param1) = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: void SetDeviceFormat([MarshalAs(UnmanagedType.LPWStr)] string param1, ref WaveFormatEx param2, ref WaveFormatEx param3);
			virtual void SetDeviceFormat(const std::wstring &param1, WaveFormatEx &param2, WaveFormatEx &param3) = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: void GetProcessingPeriod([MarshalAs(UnmanagedType.LPWStr)] string param1, int param2, IntPtr param4, IntPtr param5);
			virtual void GetProcessingPeriod(const std::wstring &param1, int param2, IntPtr param4, IntPtr param5) = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: void SetProcessingPeriod([MarshalAs(UnmanagedType.LPWStr)] string param1, IntPtr param2);
			virtual void SetProcessingPeriod(const std::wstring &param1, IntPtr param2) = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: void GetShareMode([MarshalAs(UnmanagedType.LPWStr)] string param1, ref DeviceShareMode param2);
			virtual void GetShareMode(const std::wstring &param1, DeviceShareMode &param2) = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: void SetShareMode([MarshalAs(UnmanagedType.LPWStr)] string param1, ref DeviceShareMode param2);
			virtual void SetShareMode(const std::wstring &param1, DeviceShareMode &param2) = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: void GetPropertyValue([MarshalAs(UnmanagedType.LPWStr)] string param1, ref PropertyKey param2, ref PropVariant param3);
			virtual void GetPropertyValue(const std::wstring &param1, PropertyKey &param2, PropVariant &param3) = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: void SetPropertyValue([MarshalAs(UnmanagedType.LPWStr)] string param1, ref PropertyKey param2, ref PropVariant param3);
			virtual void SetPropertyValue(const std::wstring &param1, PropertyKey &param2, PropVariant &param3) = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: void SetDefaultEndpoint([MarshalAs(UnmanagedType.LPWStr)] string wszDeviceId, ERole eRole);
			virtual void SetDefaultEndpoint(const std::wstring &wszDeviceId, ERole eRole) = 0;

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in native C++:
//ORIGINAL LINE: void SetEndpointVisibility([MarshalAs(UnmanagedType.LPWStr)] string param1, int param2);
			virtual void SetEndpointVisibility(const std::wstring &param1, int param2) = 0;
		};

		// #################### COM Interfaces (unimplemented) ####################

		class IMMNotificationClient
		{

		};

		class ISimpleAudioVolume
		{

		};

		class IAudioSessionNotification
		{

		};

		class IAudioVolumeDuckNotification
		{

		};

		class IAudioSessionEvents
		{
		};
	}
}
