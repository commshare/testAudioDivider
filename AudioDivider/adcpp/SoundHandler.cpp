#include "SoundHandler.h"

using namespace AudioDivider::WASAPI;

namespace AudioDivider
{

	void SoundHandler::switchDefaultDevice()
	{
		std::vector<SoundInfoDevice*> devices = getSoundInfo();
		if (devices.size() >= 2)
		{
			DeviceEnumerator tempVar();
			IMMDeviceEnumerator *enumerator = static_cast<IMMDeviceEnumerator*>(&tempVar);
			std::wstring id = enumerator->GetDefaultAudioEndpoint(EDataFlow::eRender, ERole::eMultimedia)->GetId();

			IPolicyConfigClass tempVar2();
			IPolicyConfig *policyConfig = static_cast<IPolicyConfig*>(&tempVar2);
			//默认设备要设置么？
			policyConfig->SetDefaultEndpoint(id, ERole::eMultimedia);
		}
	}

	std::vector<SoundInfoDevice*> SoundHandler::getSoundInfo()
	{
		std::vector<SoundInfoDevice*> soundInfoDevices;

		DeviceEnumerator *enumerator = new DeviceEnumerator();
		IMMDeviceEnumerator *deviceEnumerator = static_cast<IMMDeviceEnumerator*>(enumerator);
		IMMDeviceCollection *deviceCollection = deviceEnumerator->EnumAudioEndpoints(EDataFlow::eRender, DeviceStatemask::DEVICE_STATE_ACTIVE);
		unsigned int deviceCount = deviceCollection->GetCount();

		for (unsigned int i = 0; i < deviceCount; i++)
		{
			SoundInfoDevice *soundInfoDevice = new SoundInfoDevice();
			soundInfoDevices.push_back(soundInfoDevice);

			IMMDevice *device = deviceCollection->Item(i);
			std::wstring deviceId = device->GetId();
			soundInfoDevice->ID = deviceId;
			IMMPropertyStore *propertyStore = device->OpenPropertyStore(ProperyStoreMode::STGM_READ);
			PropertyKey propertyKeyDeviceDesc = PropertyKey();
			propertyKeyDeviceDesc.fmtid = Guid(0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0);
			propertyKeyDeviceDesc.pid = 2;
			PropVariant deviceNamePtr = propertyStore->GetValue(propertyKeyDeviceDesc);
			std::wstring deviceName = Marshal::PtrToStringUni(deviceNamePtr.pszVal);
			soundInfoDevice->name = deviceName;

			Guid guidAudioSessionManager2 = Guid(L"77AA99A0-1BD6-484F-8BC7-2C654C9A9B6F");
			IAudioSessionManager2 *audioSessionManager = static_cast<IAudioSessionManager2*>(device->Activate(guidAudioSessionManager2, static_cast<int>(ClsCtx::CLSCTX_ALL), void*::Zero));


			IAudioSessionEnumerator *sessionEnumerator = audioSessionManager->GetSessionEnumerator();

			int sessionCount = sessionEnumerator->GetCount();
			for (int j = 0; j < sessionCount; j++)
			{

				IAudioSessionControl *audioSessionControl = sessionEnumerator->GetSession(j);
				IAudioSessionControl2 *audioSessionControl2 = static_cast<IAudioSessionControl2*>(audioSessionControl);
				AudioSessionState state = audioSessionControl->GetState();
				if (state == AudioSessionState::AudioSessionStateActive)
				{
					SoundInfoSession *soundInfoSession = new SoundInfoSession();
					soundInfoDevice->sessions.push_back(soundInfoSession);

					std::wstring displayName = audioSessionControl->GetDisplayName();
					std::wstring iconPath = audioSessionControl->GetIconPath();
					int processId = audioSessionControl2->GetProcessId();
					std::wstring processName = Process::GetProcessById(processId)->MainWindowTitle;

					soundInfoSession->pid = processId;
					soundInfoSession->windowName = processName;
				}
			}
		}

		return soundInfoDevices;
	}
}
