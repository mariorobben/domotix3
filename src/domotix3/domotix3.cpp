#include <iostream>
#ifdef _WIN32

#include "wago.hpp"

int main(int argc, char** argv)
{
	std::cout << "hello windows" << std::endl;
	return 0;
}

#else

#include <iostream>

#include "kbus_adi.hpp"

#include <unistd.h>

using namespace std;
using namespace wago;

int main(int argc, char** argv)
{
	adi_kbus_device wago;

	auto status = wago.get_status();
	cout << "status" << endl;
	cout << "get_bit_count: " << status.get_bit_count() << endl;
	cout << "get_terminal_count: " << status.get_terminal_count() << endl;
	cout << "get_bit_count_analog_input: " << status.get_bit_count_analog_input() << endl;
	cout << "get_bit_count_analog_output: " << status.get_bit_count_analog_output() << endl;
	cout << "get_bit_count_digital_input: " << status.get_bit_count_digital_input() << endl;
	cout << "get_bit_count_digital_output: " << status.get_bit_count_digital_output() << endl;
	cout << endl;

	auto terminals = wago.get_terminals();
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

	while (true)
	{
		//void write_bytes(wago_device& wago_device, int32_t offset, const bytes_type& bytes);
		//bytes_type read_bytes(wago_device&  wago_device, int32_t offset, int32_t size);
		auto bts = read_bytes(wago, 0, 1);
		write_bytes(wago, 8, bts);
		usleep(10000);
		cout << "." << flush;
	}

	return 0;
}

#endif
