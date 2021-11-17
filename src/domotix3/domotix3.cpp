#include <boost/progress.hpp>
#define BOOST_USE_WINDOWS_H

#include <conio.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>

#include "wago_http_client.hpp"
#include "httplib.h"

using namespace std;
using namespace wago;

#include <iostream>
#include <boost/array.hpp>
#include "robben_engine.hpp"

shared_ptr<wago_device> create_wago_device()
{
	return std::make_shared<wago_http_client>("wago", "localhost", 5000);
}

using boost::asio::ip::tcp;

int main(int argc, char** argv)
{
	/*WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	auto wago_device = create_wago_device();

	while (true)
	{
		read_bytes_region_type rb(0, 1);
		auto io = wago_device->read_bytes(rb);
		cout << (int)io[0] << endl;

		bytes_type bytes{ io[0] };
		write_bytes_region_type wb(8, bytes);
		wago_device->write_bytes(wb);

		Sleep(10);
	}
	return 0;*/

	auto wago_device = create_wago_device();
	domotix3::test_engine engine(wago_device);
	engine.start(std::chrono::milliseconds(50));

	cin.get();

	engine.stop();
}