#ifndef __DIMMERS_H__
#define __DIMMERS_H__

#include <functional>
#include <chrono>

#include <boost/optional.hpp>
#include <boost/static_assert.hpp>

#include "engine.hpp"
#include "binary_input.hpp"
#include "output_path.hpp"
#include <memory>

namespace domotix3
{

	template<typename T>
	class analog_output : public module, public logic_execution_participant
	{
		BOOST_STATIC_ASSERT(::std::is_integral<T>::value);

	public:
		analog_output(engine& engine, T min_value, T max_value, ::std::function<void (T)> setter)
			: module(engine)
			, min_value_(min_value)
			, max_value_(max_value)
			, value_(T())
			, setter_(setter)
			, started_at_()
		{
		}

		T get_min_value() const
		{
			return min_value_;
		}

		T get_max_value() const
		{
			return max_value_;
		}

		T get_value() const
		{
			return value_;
		}

		void set_value(T value)
		{
			if (value > max_value_ || (value > 0 && value < min_value_))
			{
				throw ::std::range_error("invalid value in set_value");
			}

			if (value_ != value)
			{
				value_ = value;
				setter_(value_);
			}

			path_.reset();
			path_finished_();
		}

		void play(output_path path, T end_value, const ::std::chrono::milliseconds& duration, std::function<void()> path_finished, bool repeat = false, const ::std::chrono::milliseconds& offset = ::std::chrono::milliseconds(0))
		{
			if (end_value > max_value_ || (end_value > 0 && end_value < min_value_))
			{
				throw ::std::range_error("invalid end_value in play_path");
			}

			path_ = ::std::unique_ptr<output_path>(new output_path(path));
			path_finished_ = path_finished;
			end_value_ = end_value;
			started_at_ = engine::context()->steady_clock() - offset;
			duration_ = duration;
			repeat_ = repeat;
		}

		void logic_execution()
		{
			if (path_)
			{
				auto progress = ::std::chrono::duration_cast<::std::chrono::milliseconds>(engine::context()->steady_clock() - started_at_);
				if (repeat_)
				{
					progress %= duration_.count();
				}
				if (progress < duration_)
				{
					value_ = translate_to_physical((*path_)(static_cast<double>(progress.count()) / static_cast<double>(duration_.count())));
					setter_(value_);
				}
				else
				{
					value_ = end_value_;
					setter_(value_);
					path_.reset();
					path_finished_();
				}
			}
		}

		void stop()
		{
			path_.reset();
			path_finished_();
		}

		T translate_to_physical(double logical)
		{
			if (logical == 0.)
			{
				return 0;
			}

			return (((logical - ::std::nextafter(0., 1.)) * (max_value_ - min_value_)) / (1. - ::std::nextafter(0., 1.))) + min_value_;
		}

		double translate_to_logical(T physical)
		{
			if (physical == 0)
			{
				return 0.;
			}

			return (((physical - min_value_) * (1. - ::std::nextafter(0., 1.))) / (max_value_ - min_value_)) + ::std::nextafter(0., 1.);
		}

	private:
		T min_value_;
		T max_value_;
		T value_;
		::std::function<void (T)> setter_;
		::std::unique_ptr<output_path> path_;
		::std::function<void()> path_finished_;
		T end_value_;
		::std::chrono::steady_clock::time_point started_at_;
		::std::chrono::milliseconds duration_;
		bool repeat_;
	};

template<typename T>
class dimmer_controller_base
{
public:
	dimmer_controller_base(analog_output<T>& analog_output, const std::chrono::milliseconds& total_fade_time, const std::chrono::milliseconds& short_press_time)
		: dimmer_(analog_output)
		, total_fade_time_(total_fade_time)
		, short_press_time_(short_press_time)
	{
	}

protected:
	analog_output<T>& dimmer_;
	std::chrono::milliseconds total_fade_time_;
	std::chrono::milliseconds short_press_time_;
};

template<typename T>
class dimmer_1_button_controller : public dimmer_controller_base<T>
{
public:
	dimmer_1_button_controller(binary_input& binary_input, analog_output<T>& analog_output, const std::chrono::milliseconds& total_fade_time, const std::chrono::milliseconds& short_press_time)
		: dimmer_controller_base<T>(analog_output, total_fade_time, short_press_time)
		, new_value_(analog_output.get_max_value())
		, binary_input_(binary_input)
	{
		binary_input_.on_set().connect([this] { start_fade_in_out(); });
		binary_input_.on_clear().connect([this] { stop_fade_in_out(); });
	}

	void start_fade_in_out()
	{
		if (!fade_in_out_started_at_)
		{
			fade_in_out_started_at_ = engine::context()->steady_clock();

			if (this->dimmer_.get_value() == 0)
			{
				turn_off_after_short_release_ = false;
				is_fading_in_ = true;
				this->dimmer_.play(ramp_up_output_path(), this->dimmer_.get_max_value(), this->total_fade_time_, [this] { this->fade_in_out_started_at_.reset(); });
			}
			else if (this->dimmer_.get_value() == this->dimmer_.get_min_value())
			{
				turn_off_after_short_release_ = true;
				is_fading_in_ = true;
				this->dimmer_.play(ramp_up_output_path(), this->dimmer_.get_max_value(), this->total_fade_time_, [this] { this->fade_in_out_started_at_.reset(); });
			}
			else if (this->dimmer_.get_value() == this->dimmer_.get_max_value())
			{
				turn_off_after_short_release_ = true;
				is_fading_in_ = false;
				this->dimmer_.play(ramp_up_output_path(), this->dimmer_.get_min_value(), this->total_fade_time_, [this] { this->fade_in_out_started_at_.reset(); });
			}
			else
			{
				turn_off_after_short_release_ = true;
				is_fading_in_ = fade_in_;
				if (is_fading_in_ == true)
				{
					this->dimmer_.play(ramp_up_output_path(), this->dimmer_.get_max_value(), this->total_fade_time_, [this] { this->fade_in_out_started_at_.reset(); });
				}
				else
				{
					this->dimmer_.play(ramp_up_output_path(), this->dimmer_.get_min_value(), this->total_fade_time_, [this] { this->fade_in_out_started_at_.reset(); });
				}
			}
		}
	}

	void stop_fade_in_out()
	{
		if (fade_in_out_started_at_)
		{
			if (fade_in_out_started_at_.get() + this->short_press_time_ < engine::context()->steady_clock())
			{
				this->dimmer_.stop();

				// remember fade direction and current value
				fade_in_ = !fade_in_;
				new_value_ = this->dimmer_.get_value();
			}
			else
			{
				this->dimmer_.set_value(turn_off_after_short_release_ == true ? 0 : new_value_);
			}
			fade_in_out_started_at_ = boost::none;
		}
	}

private:
	T new_value_;
	bool is_fading_in_;
	bool fade_in_;
	bool turn_off_after_short_release_;
	boost::optional< std::chrono::steady_clock::time_point > fade_in_out_started_at_;
	binary_input& binary_input_;
};

template<typename T>
class dimmer_2_buttons_controller : public dimmer_controller_base<T>
{
public:
	dimmer_2_buttons_controller(analog_output<T>& analog_output, const std::chrono::milliseconds& total_fade_time, const std::chrono::milliseconds& short_press_time)
		: dimmer_controller_base<T>(analog_output, total_fade_time, short_press_time)
	{
	}

	void start_fade_out()
	{
		if (!fade_in_started_at_ && !fade_out_started_at_)
		{
			fade_out_started_at_ = engine::context()->steady_clock();
			if (this->dimmer_.get_value() == 0)
			{
				this->dimmer_.set_value(this->dimmer_.get_min_value());
				process_down_button_short_release_ = false;
			}
			else if (this->dimmer_.get_value() == this->dimmer_.get_min_value())
			{
				this->dimmer_.set_value(0);
				process_down_button_short_release_ = false;
			}
			else
			{
				this->dimmer_.fade_value(this->dimmer_.get_min_value(), this->dimmer_.get_fade_out_time(this->total_fade_time_));
				process_down_button_short_release_ = true;
			}
		}
	}

	void stop_fade_out()
	{
		if (fade_out_started_at_)
		{
			if (fade_out_started_at_.get() + std::chrono::milliseconds(this->short_press_time_.total_milliseconds()) < engine::context()->steady_clock())
			{
				this->dimmer_.stop_fade();
			}
			else
			{
				if (process_down_button_short_release_ == true)
				{
					this->dimmer_.set_value(0);
				}
			}

			fade_out_started_at_ = boost::none;
		}
	}

	void start_fade_in()
	{
		if (!fade_in_started_at_ && !fade_out_started_at_)
		{
			fade_in_started_at_ = engine::context()->steady_clock();
			this->dimmer_.fade_value(this->dimmer_.get_max_value(), this->total_fade_time_);
		}
	}

	void stop_fade_in()
	{
		if (fade_in_started_at_)
		{
			if (fade_in_started_at_.get() + std::chrono::milliseconds(this->short_press_time_.total_milliseconds()) < engine::context()->steady_clock())
			{
				this->dimmer_.stop_fade();
			}
			else
			{
				this->dimmer_.set_value(this->dimmer_.get_max_value());
			}

			fade_in_started_at_ = boost::none;
		}
	}

private:
	bool process_down_button_short_release_;
	boost::optional< std::chrono::steady_clock::time_point > fade_in_started_at_;
	boost::optional< std::chrono::steady_clock::time_point > fade_out_started_at_;
};

};

#endif
