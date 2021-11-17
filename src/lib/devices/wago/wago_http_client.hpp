#ifndef __WAGO_HTTP_CLIENT_HPP__
#define __WAGO_HTTP_CLIENT_HPP__

#include <ArduinoJson-v6.18.5.h>
#include "wago.hpp"

#include <string>

#include <httplib.h>

namespace wago
{

	class wago_http_client : public wago_device
	{
	public:
		wago_http_client(const std::string& deviceName, const std::string& host, int port);
		virtual ~wago_http_client();

		status get_status() override;
		terminals_type get_terminals() override;
		void write_bytes(const write_bytes_region_type& regions) override;
		bytes_type read_bytes(const read_bytes_region_type& regions) override;

	private:
		std::string _deviceName;
		httplib::Client _client;
		httplib::Headers _headers;
		DynamicJsonDocument _doc;
	};

};

#endif
