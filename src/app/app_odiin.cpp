#include "app_odiin.h"

#include "app_error.h"
#include "app_timer.h"
#include "bsp.h"
#include "nrf_drv_clock.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "usb/usb.h"
#include "global/global_data.h"

namespace App
{
	//////////////////////////////////////////////////////////////////////////
	// Public interface

	Odiin* Odiin::GetInstance()
	{
		static Odiin instance;
		return &instance;
	}

	Files::SdCard& Odiin::GetSdCard()
	{
		return *(GetInstance()->sdCard);
	}

	Odiin::~Odiin()
	{

	}

	void Odiin::Update()
	{
        NRF_LOG_FLUSH();

		Usb::Device::Update();
		screen->Update();
	}


	void Odiin::SetNfcTagPayload(nfc_tag_emulation::Payload* pl)
	{
		nfcTagEmulator->SetPayload(pl);
	}

	void Odiin::SetNfcTagEnabled(bool enabled)
	{
		if (enabled)
		{
			nfcTagEmulator->EnableTag();
		}
		else
		{
			nfcTagEmulator->DisableTag();
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// Private interface

	// forward declarations of file-local functions
	namespace
	{
		void BspEventHandler(bsp_event_t event);
	}

	//////////////////////////////////////////////////////////////////////////
	// Application component initilization

	Odiin::Odiin()
	{
		NRF_LOG_RAW_INFO("=====================================");
		NRF_LOG_INFO("Odiin initializing...");

		InitializeLogs();
		InitializeBsp();
		InitializeUsbDevice();
		InitializeSdCard();
		InitializeInput();
		InitializeScreen();
		InitializeNfcTag();

		NRF_LOG_INFO("Odiin initialization complete. Ready.");
		NRF_LOG_RAW_INFO("=====================================");
	}

	void Odiin::InitializeLogs()
	{
		ret_code_t err_code = NRF_LOG_INIT(app_usbd_sof_timestamp_get);
		APP_ERROR_CHECK(err_code);

		NRF_LOG_DEFAULT_BACKENDS_INIT();
	}

	void Odiin::InitializeClocks()
	{
		NRF_CLOCK->LFCLKSRC            = (CLOCK_LFCLKSRC_SRC_Xtal << CLOCK_LFCLKSRC_SRC_Pos);
		NRF_CLOCK->EVENTS_LFCLKSTARTED = 0;
		NRF_CLOCK->TASKS_LFCLKSTART    = 1;

		while (NRF_CLOCK->EVENTS_LFCLKSTARTED == 0)
		{
			// Do nothing.
		}

		// initialize
		ret_code_t ret = nrf_drv_clock_init();
		APP_ERROR_CHECK(ret);
		nrf_drv_clock_lfclk_request(NULL);
	}

	void Odiin::InitializeTimers()
	{
		ret_code_t err_code = app_timer_init();
		APP_ERROR_CHECK(err_code);
	}

	void Odiin::InitializeBsp()
	{
		uint32_t err_code;

		const uint32_t bspFlags = BSP_INIT_LEDS | BSP_INIT_BUTTONS;

		err_code = bsp_init(bspFlags, BspEventHandler);
		APP_ERROR_CHECK(err_code);
	}

	void Odiin::InitializeUsbDevice()
	{
		Usb::Device::Initialize();
		static Usb::MassStorageClass msc;
		usbMassStorageClass = &msc;
		usbMassStorageClass->RegisterClass();
		Usb::Device::Enable();
	}

	void Odiin::InitializeSdCard()
	{
		static Files::SdCard sdc;
		sdCard = &sdc;

		// we will go ahead and start mounted
		sdCard->Mount();

		// SDCARD will want to listen for USB events.
		Usb::Device::RegisterListener(sdCard);
	}

	void Odiin::InitializeInput()
	{
		static Input::Keypad kp;
		keypad = &kp;
	}

	void Odiin::InitializeScreen()
	{
		static Display::Screen scr(keypad);
		screen = &scr;
	}

	void Odiin::InitializeNfcTag()
	{
		NfcTagEmulator tagEmu;
		nfcTagEmulator = &tagEmu;

		nfcTagEmulator->Initialize();
	}

	namespace
	{
		void BspEventHandler(bsp_event_t event)
		{
			switch (event)
			{
				case BSP_EVENT_KEY_0:
					NRF_LOG_INFO("Board button pressed. No event handler yet.");
					break;
				default:
					NRF_LOG_DEBUG("[BSP] Unhandled event: %d", event);
					break;
			}
		}
	}

} // namespace App
