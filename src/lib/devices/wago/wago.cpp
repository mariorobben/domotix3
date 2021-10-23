#include "wago.hpp"

namespace wago
{

using std::string;
using std::make_pair;
using std::runtime_error;
using std::exception;

#pragma region dtos

status::status(int32_t bit_count, int32_t terminal_count, int32_t bit_count_analog_input, int32_t bit_count_analog_output, int32_t bit_count_digital_input, int32_t bit_count_digital_output)
	: bit_count_{bit_count}
	, terminal_count_{terminal_count}
	, bit_count_analog_input_{bit_count_analog_input}
	, bit_count_analog_output_{bit_count_analog_output}
	, bit_count_digital_input_{bit_count_digital_input}
	, bit_count_digital_output_{bit_count_digital_output}
{
}

int32_t status::get_bit_count() const
{
	return bit_count_;
}

int32_t status::get_terminal_count() const
{
	return terminal_count_;
}

int32_t status::get_bit_count_analog_input() const
{
	return bit_count_analog_input_;
}

int32_t status::get_bit_count_analog_output() const
{
	return bit_count_analog_output_;
}

int32_t status::get_bit_count_digital_input() const
{
	return bit_count_digital_input_;
}

int32_t status::get_bit_count_digital_output() const
{
	return bit_count_digital_output_;
}

terminal::terminal(int32_t position, int32_t type, int32_t bit_offset_out, int32_t bit_size_out, int32_t bit_offset_in, int32_t bit_size_in, int32_t channels, int32_t pi_format)
	: position_{position}
	, type_{type}
	, bit_offset_out_{bit_offset_out}
	, bit_size_out_{bit_size_out}
	, bit_offset_in_{bit_offset_in}
	, bit_size_in_{bit_size_in}
	, channels_{channels}
	, pi_format_{pi_format}
{
}

int32_t terminal::get_position() const
{
	return position_;
}

int32_t terminal::get_type() const
{
	return type_;
}

int32_t terminal::get_bit_offset_out() const
{
	return bit_offset_out_;
}

int32_t terminal::get_bit_size_out() const
{
	return bit_size_out_;
}

int32_t terminal::get_bit_offset_in() const
{
	return bit_offset_in_;
}

int32_t terminal::get_bit_size_in() const
{
	return bit_size_in_;
}

int32_t terminal::get_channels() const
{
	return channels_;
}

int32_t terminal::get_pi_format() const
{
	return pi_format_;
}

#pragma endregion

#pragma region exceptions

wago_exception::wago_exception(const char* message)
	: runtime_error(message)
{
}

#pragma endregion

#pragma region extension methods

void write_bytes(wago_device& wago_device, int32_t offset, const bytes_type& bytes)
{
	write_bytes_region_type regions;
	regions.push_back(make_pair(offset, bytes));
	wago_device.write_bytes(regions);
}

bytes_type read_bytes(wago_device& wago_device, int32_t offset, int32_t size)
{
	read_bytes_region_type regions;
	regions.push_back(make_pair(offset, size));
	auto result = wago_device.read_bytes(regions);
	return result.front();
}

#pragma endregion

};
