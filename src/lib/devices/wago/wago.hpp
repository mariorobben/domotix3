#ifndef __WAGO_HPP__
#define __WAGO_HPP__

#include <cstdint>
#include <vector>
#include <stdexcept>

#include <engine.hpp>

#include "io_bindings.hpp"

namespace wago
{

const int32_t wago_io_size = 2040;

class status
{
public:
	status(int32_t bit_count, int32_t terminal_count, int32_t bit_count_analog_input, int32_t bit_count_analog_output, int32_t bit_count_digital_input, int32_t bit_count_digital_output);

	int32_t get_bit_count() const;
	int32_t get_terminal_count() const;
	int32_t get_bit_count_analog_input() const;
	int32_t get_bit_count_analog_output() const;
	int32_t get_bit_count_digital_input() const;
	int32_t get_bit_count_digital_output() const;

private:
	int32_t bit_count_;
	int32_t terminal_count_;
	int32_t bit_count_analog_input_;
	int32_t bit_count_analog_output_;
	int32_t bit_count_digital_input_;
	int32_t bit_count_digital_output_;
};

class terminal
{
public:
	terminal(int32_t position, int32_t type, int32_t bit_offset_out, int32_t bit_size_out, int32_t bit_offset_in, int32_t bit_size_in, int32_t channels, int32_t pi_format);
	
	int32_t get_position() const;
	int32_t get_type() const;
	int32_t get_bit_offset_out() const;
	int32_t get_bit_size_out() const;
	int32_t get_bit_offset_in() const;
	int32_t get_bit_size_in() const;
	int32_t get_channels() const;
	int32_t get_pi_format() const;

private:
	int32_t position_;
	int32_t type_;
	int32_t bit_offset_out_;
	int32_t bit_size_out_;
	int32_t bit_offset_in_;
	int32_t bit_size_in_;
	int32_t channels_;
	int32_t pi_format_;
};

typedef std::vector<uint8_t> bytes_type;
typedef std::vector<terminal> terminals_type;
typedef std::pair<int32_t, bytes_type> write_bytes_region_type;
typedef std::pair<int32_t, int32_t> read_bytes_region_type;

class wago_device
{
public:
	virtual status get_status() = 0;
	virtual terminals_type get_terminals() = 0;
	virtual void write_bytes(const write_bytes_region_type& regions) = 0;
	virtual bytes_type read_bytes(const read_bytes_region_type& regions) = 0;
};

class wago_exception : public std::runtime_error
{
public:
	wago_exception(const char* message);
};

class wago_io_bindings : public domotix3::io_bindings
{
public:
	wago_io_bindings(wago_device& wago_device);

	wago_device& get_wago_device();

private:
	wago_device& wago_device_;
};

class wago_io_bindings_engine : public wago_io_bindings, public domotix3::module, public domotix3::input_scan_participant, public domotix3::output_scan_participant
{
public:
	wago_io_bindings_engine(domotix3::engine& engine, wago_device & wago_device);

	void input_scan();
	void output_scan();
};

};

#endif
