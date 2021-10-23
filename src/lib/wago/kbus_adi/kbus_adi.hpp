#ifndef __KBUS_ADI_HPP__
#define __KBUS_ADI_HPP__

#include "wago.hpp"

extern "C" {
#include <dal/adi_application_interface.h>
}

namespace wago
{

class adi_kbus_device : public wago_device
{
public:
	adi_kbus_device();
	~adi_kbus_device();
	
	status get_status();
	terminals_type get_terminals();
	void write_bytes(const write_bytes_region_type& regions);
	read_bytes_vector_type read_bytes(const read_bytes_region_type& regions);

private:
	tApplicationDeviceInterface* adi_;
	tDeviceId kbus_device_id_;
    tApplicationStateChangedEvent event_;
	uint32_t task_id_;
};

};

#endif
