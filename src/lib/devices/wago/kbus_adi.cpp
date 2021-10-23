#include "kbus_adi.hpp"
#include "wago.hpp"

#include <cstring>

#include <sched.h>

extern "C" {
//-----------------------------------------------------------------------------
// include files for KBUS WAGO ADI
//-----------------------------------------------------------------------------
#include <dal/adi_application_interface.h>
//-----------------------------------------------------------------------------
// include files for kbus information via dbus
//-----------------------------------------------------------------------------
#include <ldkc_kbus_information.h>
#include <ldkc_kbus_register_communication.h>
}
#define KBUS_MAINPRIO 40
namespace wago
{

adi_kbus_device::adi_kbus_device()
	: adi_(NULL)
	, task_id_(0)
{
    struct sched_param s_param;
	adi_ = adi_GetApplicationInterface();
	adi_->Init();

	try
	{
		adi_->ScanDevices();
		tDeviceInfo device_list[10];
		size_t nr_devices_found;
		size_t nr_kbus_found = -1;
		adi_->GetDeviceList(sizeof(device_list), device_list, &nr_devices_found);
		for (size_t i=0 ; i<nr_devices_found ; i++)
		{
			if (strcmp(device_list[i].DeviceName, "libpackbus") == 0)
			{
				nr_kbus_found = i;
				break;
			}
		}
		if (nr_kbus_found == -1)
		{
			throw wago_exception("no wago device found.");
		}
		
		kbus_device_id_ = device_list[nr_kbus_found].DeviceId;
		if (adi_->OpenDevice(kbus_device_id_) != DAL_SUCCESS)
		{
			throw wago_exception("kbus device open failed.");
		}

		// switch to RT Priority
		s_param.sched_priority = KBUS_MAINPRIO;
		sched_setscheduler(0, SCHED_FIFO, &s_param);

		try
		{
			tApplicationStateChangedEvent event;
			event.State = ApplicationState_Running;

			if (adi_->ApplicationStateChanged(event) != DAL_SUCCESS)
			{
				throw wago_exception("set application state to 'running' failed.");
			}

			if (KbusInfo_Failed == ldkc_KbusInfo_Create())
			{
				throw wago_exception("ldkc_KbusInfo_Create failed.");
			}
		}
		catch (wago_exception &ex)
		{
			adi_->CloseDevice(kbus_device_id_);
			throw;
		}
	}
	catch (wago_exception& ex)
	{
		adi_->Exit();
		throw;
	}
}

adi_kbus_device::~adi_kbus_device()
{
	ldkc_KbusInfo_Destroy();
	adi_->CloseDevice(kbus_device_id_);
	adi_->Exit();
}

status adi_kbus_device::get_status()
{
    tldkc_KbusInfo_Status kbus_status;

    if (KbusInfo_Failed == ldkc_KbusInfo_GetStatus(&kbus_status))
    {
        throw wago_exception("ldkc_KbusInfo_GetStatus() failed.");
    }

    return status(
        kbus_status.KbusBitCount,
        kbus_status.TerminalCount,
        kbus_status.BitCountAnalogInput,
        kbus_status.BitCountAnalogOutput,
        kbus_status.BitCountDigitalInput,
        kbus_status.BitCountDigitalOutput);
}

terminals_type adi_kbus_device::get_terminals()
{
    size_t terminalCount;
    u16 terminals[LDKC_KBUS_TERMINAL_COUNT_MAX];
    tldkc_KbusInfo_TerminalInfo terminalDescription[LDKC_KBUS_TERMINAL_COUNT_MAX];

    if (KbusInfo_Failed == ldkc_KbusInfo_GetTerminalInfo(LDKC_KBUS_TERMINAL_COUNT_MAX, terminalDescription, &terminalCount) )
    {
        throw wago_exception("ldkc_KbusInfo_GetTerminalInfo() failed");
    }

    if (KbusInfo_Failed == ldkc_KbusInfo_GetTerminalList(LDKC_KBUS_TERMINAL_COUNT_MAX, terminals, NULL) )
    {
        throw wago_exception("ldkc_KbusInfo_GetTerminalList() failed");
    }

    terminals_type result;

    for (int idx = 0 ; idx < terminalCount ; idx++)
    {
		int channelCount = terminalDescription[idx].AdditionalInfo.ChannelCount;
		int piFormat = terminalDescription[idx].AdditionalInfo.PiFormat;

        result.emplace_back(
            idx,
            terminals[idx],
            terminalDescription[idx].OffsetOutput_bits,
            terminalDescription[idx].SizeOutput_bits,
            terminalDescription[idx].OffsetInput_bits,
            terminalDescription[idx].SizeInput_bits,
            channelCount,
            piFormat);
    }

    return result;
}

void adi_kbus_device::write_bytes(const write_bytes_region_type& regions)
{
	adi_->WatchdogTrigger();

	adi_->WriteStart(kbus_device_id_, task_id_);

	for (auto region : regions)
	{
		adi_->WriteBytes(kbus_device_id_, task_id_, region.first, region.second.size(), region.second.data());
	}

	adi_->WriteEnd(kbus_device_id_, task_id_);
}

read_bytes_vector_type adi_kbus_device::read_bytes(const read_bytes_region_type& regions)
{
	adi_->WatchdogTrigger();

	uint32_t retval = 0;

	// Use function "libpackbus_Push" to trigger one KBUS cycle.
	if (adi_->CallDeviceSpecificFunction("libpackbus_Push", &retval) != DAL_SUCCESS)
	{
		throw wago_exception("CallDeviceSpecificFunction() failed");
	}

	if (retval != DAL_SUCCESS)
	{
		throw wago_exception("libpackbus_Push() failed");
	}
	
	adi_->ReadStart(kbus_device_id_, task_id_);

	read_bytes_vector_type result;

	for (auto region : regions)
	{
		bytes_type bytes(region.second);
		adi_->ReadBytes(kbus_device_id_, task_id_, region.first, region.second, bytes.data());
		result.push_back(bytes);
	}

	adi_->ReadEnd(kbus_device_id_, task_id_);

	return result;
}

};
