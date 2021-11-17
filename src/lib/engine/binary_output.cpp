#include "binary_output.hpp"

namespace domotix3
{

	binary_setters::binary_setters(const std::vector<binary_setter>& binary_setters)
		: binary_setters_(binary_setters)
	{
		
	}

	void binary_setters::operator()(bool value)
	{
		for (auto& setter : binary_setters_)
		{
			setter(value);
		}
	}

	binary_output::binary_output(engine& engine, binary_setter setter)
		: module(engine)
		, value_()
		, setter_(setter)
		, started_at_()
		, end_value_()
		, path_()
		, repeat_()
	{
	}

	bool binary_output::get_value() const
	{
		return value_;
	}

	void binary_output::set_value(bool value)
	{
		if (value_ != value)
		{
			value_ = value;
			setter_(value);
		}

		path_.reset();
	}

	bool binary_output::toggle_value()
	{
		value_ = !value_;
		setter_(value_);
		path_.reset();
		return value_;
	}

	void binary_output::play_path(output_path path, bool end_value, const ::std::chrono::milliseconds& duration, bool repeat, const ::std::chrono::milliseconds& offset)
	{
		path_ = ::std::unique_ptr<output_path>(new output_path(path));
		end_value_ = end_value;
		started_at_ = engine::context()->steady_clock() - offset;
		duration_ = duration;
		repeat_ = repeat;
	}

	void binary_output::logic_execution()
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
				value_ = (*path_)(static_cast<double>(progress.count()) / static_cast<double>(duration_.count())) >= 0.5;
				setter_(value_);
			}
			else
			{
				value_ = end_value_;
				setter_(value_);
				path_.reset();
			}
		}
	}

	void binary_output::stop_playing_path()
	{
		path_.reset();
	}

};
