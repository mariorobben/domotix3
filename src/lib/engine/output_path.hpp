#ifndef __OUTPUT_PATH_H_
#define __OUTPUT_PATH_H_

#include <functional>
#include <vector>
#include <numeric>

namespace domotix3
{

	typedef ::std::function<double(double)> output_path;

	output_path fixed_output_path(double value);
	output_path linear_path(double from, double to);
	output_path ramp_up_output_path();
	output_path ramp_down_output_path();
	output_path duty_cycle_output_path(double duty_cycle);
	output_path repeat_output_path(output_path path, size_t number_cycles);
	output_path inverse_output_path(output_path path);
	//output_path reverse_output_path(output_path path);

	class weighted_output_path
	{
	public:
		weighted_output_path(output_path output_path, size_t weight = 1);

		double operator()(double progress) const;

		size_t get_weight() const;

	private:
		output_path output_path_;
		size_t weight_;
	};

	class output_paths
	{
	public:
		output_paths(const ::std::vector<weighted_output_path>& paths);

		double operator()(double progress) const;

	private:
		::std::vector<weighted_output_path> paths_;
		::std::vector<::std::pair< double, double>> weight_offsets_;
	};

};

#endif
