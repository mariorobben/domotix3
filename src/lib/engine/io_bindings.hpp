#ifndef _IO_BINDINGS_H_
#define _IO_BINDINGS_H_

#include <stdint.h>

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/array.hpp>

//#include "common.hpp"

namespace domotix3
{

class io_bindings
{
public:
	io_bindings();
	io_bindings(int32_t number_analog_inputs, int32_t number_digital_inputs, int32_t number_analog_outputs, int32_t number_digital_outputs);

	template<typename T> boost::function<T()> register_input(int32_t index) { return boost::bind(&io_bindings::get_input<T>, this, index); }
	template< std::size_t N > boost::function< boost::array< uint8_t, N >()> register_inputs(int32_t index) const { return boost::bind(&io_bindings::get_inputs<N>, this, index); }

	template<typename T> boost::function<void(T)> register_output(int32_t index) { return boost::bind(&io_bindings::set_output<T>, this, index, _1); }
	template< std::size_t N > boost::function<void(boost::array< uint8_t, N >)> register_outputs(int32_t index) { return boost::bind(&io_bindings::set_outputs<N>, this, index, _1); }

	void resize(int32_t number_analog_inputs, int32_t number_digital_inputs, int32_t number_analog_outputs, int32_t number_digital_outputs);

	int32_t get_number_analog_inputs() const;
	int32_t get_number_digital_inputs() const;
	int32_t get_number_analog_outputs() const;
	int32_t get_number_digital_outputs() const;

	const std::vector<uint8_t>& get_input_image() const;
	std::vector<uint8_t>& get_input_image();

	const std::vector<uint8_t>& get_output_image() const;
	std::vector<uint8_t>& get_output_image();

private:
	template<typename T> T get_input(int32_t index) const;

	template< std::size_t N > boost::array< uint8_t, N > get_inputs(int32_t index) const
	{
		boost::array< uint8_t, N > result;
		std::copy(input_image_.begin() + index, input_image_.begin() + index + N, result.begin());
		return result;
	}

	template<typename T> void set_output(int32_t index, T value);

	template< std::size_t N > void set_outputs(int32_t index, const boost::array< uint8_t, N >& value)
	{
		std::copy(value.begin(), value.end(), output_image_.begin() + index);
	}

private:
	int32_t number_analog_inputs_;
	int32_t number_digital_inputs_;
	int32_t number_analog_outputs_;
	int32_t number_digital_outputs_;

	std::vector<uint8_t> input_image_;
	std::vector<uint8_t> output_image_;
};

};

#endif
