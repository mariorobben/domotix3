#ifndef __WAGO_HTTP_CLIENT_HPP__
#define __WAGO_HTTP_CLIENT_HPP__

#include "wago.hpp"

namespace wago
{

	class wago_http_client : public wago_device
	{
	public:
		wago_http_client();
		~wago_http_client();

		status get_status();
		terminals_type get_terminals();
		void write_bytes(const write_bytes_region_type& regions);
		read_bytes_vector_type read_bytes(const read_bytes_region_type& regions);
	};

};

#endif
