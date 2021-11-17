#include "io_bindings.hpp"

#include "engine.hpp"

namespace domotix3
{

io_bindings::io_bindings()
	: number_analog_inputs_(0)
	, number_digital_inputs_(0)
	, number_analog_outputs_(0)
	, number_digital_outputs_(0)
	, input_image_(0)
	, output_image_(0)
{
}

io_bindings::io_bindings(int32_t number_analog_inputs, int32_t number_digital_inputs, int32_t number_analog_outputs, int32_t number_digital_outputs)
	: number_analog_inputs_(number_analog_inputs)
	, number_digital_inputs_(number_digital_inputs)
	, number_analog_outputs_(number_analog_outputs)
	, number_digital_outputs_(number_digital_outputs)
	, input_image_((number_analog_inputs + 7) / 8 + (number_digital_inputs + 7) / 8)
	, output_image_((number_analog_outputs + 7) / 8 + (number_digital_outputs + 7) / 8)
{
}

void io_bindings::resize(int32_t number_analog_inputs, int32_t number_digital_inputs, int32_t number_analog_outputs, int32_t number_digital_outputs)
{
	number_analog_inputs_ = number_analog_inputs;
	number_digital_inputs_ = number_digital_inputs;
	number_analog_outputs_ = number_analog_outputs;
	number_digital_outputs_ = number_digital_outputs;
	input_image_.resize((number_analog_inputs + 7) / 8 + (number_digital_inputs + 7) / 8);
	output_image_.resize((number_analog_outputs + 7) / 8 + (number_digital_outputs + 7) / 8);
}

int32_t io_bindings::get_number_analog_inputs() const
{
	return number_analog_inputs_;
}

int32_t io_bindings::get_number_digital_inputs() const
{
	return number_digital_inputs_;
}

int32_t io_bindings::get_number_analog_outputs() const
{
	return number_analog_outputs_;
}

int32_t io_bindings::get_number_digital_outputs() const
{
	return number_digital_outputs_;
}

const std::vector<uint8_t>& io_bindings::get_input_image() const
{
	return input_image_;
}

std::vector<uint8_t>& io_bindings::get_input_image()
{
	return input_image_;
}

const std::vector<uint8_t>& io_bindings::get_output_image() const
{
	return output_image_;
}

std::vector<uint8_t>& io_bindings::get_output_image()
{
	return output_image_;
}

template<> bool io_bindings::get_input<bool>(int32_t index) const
{
	return (input_image_[(number_analog_inputs_ + 7) / 8 + index / 8] & (1 << (index % 8))) != 0;
}

template<> int8_t io_bindings::get_input<int8_t>(int32_t index) const
{
	return input_image_[index];
}

template<> uint8_t io_bindings::get_input<uint8_t>(int32_t index) const
{
	return input_image_[index];
}

template<> int16_t io_bindings::get_input<int16_t>(int32_t index) const
{
	return ntohs(*(int16_t *)&input_image_[index]);
}

template<> uint16_t io_bindings::get_input<uint16_t>(int32_t index) const
{
	return ntohs(*(int16_t *)&input_image_[index]);
}

template<> int32_t io_bindings::get_input<int32_t>(int32_t index) const
{
	return ntohl(*(int32_t *)&input_image_[index]);
}

template<> uint32_t io_bindings::get_input<uint32_t>(int32_t index) const
{
	return ntohl(*(int32_t *)&input_image_[index]);
}

template<> void io_bindings::set_output(int32_t index, bool value)
{
	if (value == true)
	{
		output_image_[(number_analog_outputs_ + 7) / 8 + index / 8] |= (1 << (index % 8));
	}
	else
	{
		output_image_[(number_analog_outputs_ + 7) / 8 + index / 8] &= ~(1 << (index % 8));
	}
}

template<> void io_bindings::set_output(int32_t index, int8_t value)
{
	output_image_[index] = value;
}

template<> void io_bindings::set_output(int32_t index, uint8_t value)
{
	output_image_[index] = value;
}

template<> void io_bindings::set_output(int32_t index, int16_t value)
{
	*(int16_t *)&output_image_[index] = htons(value);
}

template<> void io_bindings::set_output(int32_t index, uint16_t value)
{
	*(int16_t *)&output_image_[index] = htons(value);
}

template<> void io_bindings::set_output(int32_t index, int32_t value)
{
	*(int32_t *)&output_image_[index] = htonl(value);
}

template<> void io_bindings::set_output(int32_t index, uint32_t value)
{
	*(int32_t *)&output_image_[index] = htonl(value);
}

};
