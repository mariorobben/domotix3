#include <iostream>
#include <chrono>
#include <thread>

#include "wago.hpp"

using namespace std;
using namespace wago;

unique_ptr<wago_device> create_wago_device()
{
	return std::make_unique<wago_http_client>();
}

int main(int argc, char** argv)
{
	auto wago_device = create_wago_device();

	auto status = wago_device->get_status();
	cout << "status" << endl;
	cout << "get_bit_count: " << status.get_bit_count() << endl;
	cout << "get_terminal_count: " << status.get_terminal_count() << endl;
	cout << "get_bit_count_analog_input: " << status.get_bit_count_analog_input() << endl;
	cout << "get_bit_count_analog_output: " << status.get_bit_count_analog_output() << endl;
	cout << "get_bit_count_digital_input: " << status.get_bit_count_digital_input() << endl;
	cout << "get_bit_count_digital_output: " << status.get_bit_count_digital_output() << endl;
	cout << endl;

	auto terminals = wago_device->get_terminals();
	cout << "terminals" << endl;
	for (auto terminal : terminals)
	{
		cout << "get_position: " << terminal.get_position() << endl;
		cout << "get_type: " << terminal.get_type() << endl;
		cout << "get_bit_offset_out: " << terminal.get_bit_offset_out() << endl;
		cout << "get_bit_size_out: " << terminal.get_bit_size_out() << endl;
		cout << "get_bit_offset_in: " << terminal.get_bit_offset_in() << endl;
		cout << "get_bit_size_in: " << terminal.get_bit_size_in() << endl;
		cout << "get_channels: " << terminal.get_channels() << endl;
		cout << "get_pi_format: " << terminal.get_pi_format() << endl;
		cout << endl;
	}

	int i = 0;
	while (true)
	{
		auto bts = read_bytes(*wago_device, 0, 1);
		write_bytes(*wago_device, 8, bts);
		cout << "." << flush;

		this_thread::sleep_for(chrono::milliseconds(10));
		if (i == 0) { cout << "*" << flush; }
		i = (i + 1) % 100;
	}

	return 0;
}
