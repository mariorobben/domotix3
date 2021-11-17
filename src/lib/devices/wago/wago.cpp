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

wago_io_bindings::wago_io_bindings(wago_device& wago_device)
	: domotix3::io_bindings()
	, wago_device_(wago_device)
{
	auto status = wago_device_.get_status();
	resize(status.get_bit_count_analog_input(), status.get_bit_count_digital_input(), status.get_bit_count_analog_output(), status.get_bit_count_digital_output());
}

wago_device& wago_io_bindings::get_wago_device()
{
	return wago_device_;
}

wago_io_bindings_engine::wago_io_bindings_engine(domotix3::engine& engine, wago_device& wago_device)
	: wago_io_bindings(wago_device)
	, module(engine)
{
}

void wago_io_bindings_engine::input_scan()
{
	auto result = get_wago_device().read_bytes(read_bytes_region_type(0, get_input_image().size()));
	std::copy(result.begin(), result.end(), get_input_image().begin());
}

void wago_io_bindings_engine::output_scan()
{
	get_wago_device().write_bytes(write_bytes_region_type(0, bytes_type(get_output_image().begin(), get_output_image().end())));
}

};
