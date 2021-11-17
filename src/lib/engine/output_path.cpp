#include "output_path.hpp"

#include <cmath>
#include <stdexcept>

namespace domotix3
{

	output_path fixed_output_path(double value)
	{
		return [value](double progress) { return value; };
	}

	output_path linear_path(double from, double to)
	{
		return [from, to](double progress) { return (progress * (to - from)) + from; };
	}

	output_path ramp_up_output_path()
	{
		return [](double progress) { return progress; };
	}

	output_path ramp_down_output_path()
	{
		return [](double progress) { return 1. - progress; };
	}

	output_path duty_cycle_output_path(double duty_cycle)
	{
		return [duty_cycle](double progress) { return progress < duty_cycle; };
	}

	output_path repeat_output_path(output_path path, size_t number_cycles)
	{
		return [path, number_cycles](double progress) { return path(::std::fmod(progress * static_cast<double>(number_cycles), 1.)); };
	}

	output_path inverse_output_path(output_path path)
	{
		return [path](double progress) { return 1. - path(progress); };
	}

	output_path reverse_output_path(output_path path)
	{
		return [path](double progress) { return path(::std::nextafter(1., 0.) - progress); };
	}

	weighted_output_path::weighted_output_path(output_path output_path, size_t weight)
		: output_path_(output_path)
		, weight_(weight)
	{
		if (weight < 1)
		{
			throw ::std::out_of_range("weight");
		}
	}

	double weighted_output_path::operator()(double progress) const
	{
		return output_path_(progress);
	}

	size_t weighted_output_path::get_weight() const
	{
		return weight_;
	}

	output_paths::output_paths(const std::vector<weighted_output_path>& paths)
		: paths_(paths)
	{
		auto total_weight = ::std::accumulate(
			paths_.begin(),
			paths_.end(),
			0,
			[](int sum, const weighted_output_path& elem) { return sum + elem.get_weight(); });

		size_t weight = 0;
		for (auto p : paths_)
		{
			weight_offsets_.emplace_back(static_cast<double>(weight) / static_cast<double>(total_weight), static_cast<double>(total_weight) / static_cast<double>(p.get_weight()));
			weight += p.get_weight();
		}
	}

	double output_paths::operator()(double progress) const
	{
		if (!weight_offsets_.empty())
		{
			for (int i = weight_offsets_.size() - 1; i >= 0; i--)
			{
				if (progress >= weight_offsets_[i].first)
				{
					return paths_[i]((progress - weight_offsets_[i].first) * weight_offsets_[i].second);
				}
			}
		}

		return 0.;
	}

};
