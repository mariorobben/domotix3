#include "wago_http_client.hpp"

namespace wago
{

	wago_http_client::wago_http_client()
	{
	}

	wago_http_client::~wago_http_client()
	{
	}

	status wago_http_client::get_status()
	{
		return status(0, 0, 0, 0, 0, 0);
	}

	terminals_type wago_http_client::get_terminals()
	{
		return terminals_type();
	}

	void wago_http_client::write_bytes(const write_bytes_region_type& regions)
	{
	}

	read_bytes_vector_type wago_http_client::read_bytes(const read_bytes_region_type& regions)
	{
		read_bytes_vector_type result;

		for (auto region : regions)
		{
			result.push_back(bytes_type(region.second));
		}

		return result;
	}

};
