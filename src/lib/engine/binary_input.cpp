#include "binary_input.hpp"

namespace domotix3
{

	binary_getter not_binary_getter(const binary_getter& rhs)
	{
		return [rhs]() { return !rhs(); };
	}

	binary_getter and_binary_getter(const binary_getter& lhs, const binary_getter& rhs)
	{
		return [lhs, rhs]() { return lhs() | rhs(); };
	}

	binary_getter or_binary_getter(const binary_getter& lhs, const binary_getter& rhs)
	{
		return [lhs, rhs]() { return lhs() & rhs(); };
	}

	exclusive_binary_getter::exclusive_binary_getter(const std::vector<binary_getter>& binary_getters)
		: binary_getters_(binary_getters)
	{
	}

	bool exclusive_binary_getter::operator()() const
	{
		if (locked_getter_)
		{
			if ((*locked_getter_)())
			{
				return true;
			}
			locked_getter_ = boost::none;
		}

		for (auto& getter : binary_getters_)
		{
			if (getter() == true)
			{
				locked_getter_ = getter;
				return true;
			}
		}

		return false;
	}

	binary_input::binary_input(engine& engine, binary_getter getter, toggle_types toggle_type)
		: binary_input(engine, getter, toggle_type, std::vector<std::chrono::milliseconds>())
	{
	}

	binary_input::binary_input(engine& engine, binary_getter getter, const std::vector<::std::chrono::milliseconds>& intervals)
		: binary_input(engine, getter, toggle_types::toggle_when_no_interval_is_reached, intervals)
	{
	}

	binary_input::binary_input(engine& engine, binary_getter getter, toggle_types toggle_type, const std::vector<std::chrono::milliseconds>& intervals)
		: module(engine)
		, getter_(getter)
		, toggle_type_(toggle_type)
		, intervals_(intervals)
		, set_signal_()
		, clear_signal_()
		, toggle_signal_()
		, signal_high_(false)
		, interval_index_(0)
		, next_interval_()
	{
		if (intervals_.empty() && toggle_type_ == toggle_types::toggle_when_no_interval_is_reached)
		{
			throw ::std::runtime_error("toggle_when_no_interval_is_reached only allowed when intervals are used");
		}
	}

	binary_input::~binary_input()
	{
	}

	void binary_input::logic_execution()
	{
		set_signal_high(getter_());
	}

	void binary_input::set_signal_high(bool signal_high)
	{
		if (signal_high_ != signal_high)
		{
			signal_high_ = signal_high;

			if (signal_high_ == true)
			{
				set_signal_();
			}
			else
			{
				clear_signal_();
			}

			switch (toggle_type_)
			{
			case toggle_types::toggle_on_rising_edge:
				if (signal_high_ == true)
				{
					toggle_signal_();
				}
				break;

			case toggle_types::toggle_on_falling_edge:
				if (signal_high_ == false)
				{
					toggle_signal_();
				}
				break;

			case toggle_types::toggle_on_both_edges:
				toggle_signal_();
				break;
			}

			if (signal_high_ && !intervals_.empty())
			{
				started_at_ = engine::context()->steady_clock();
				interval_index_ = 0;
				next_interval_ = started_at_ + intervals_[interval_index_];
			}
			else if (toggle_type_ == toggle_types::toggle_when_no_interval_is_reached && interval_index_ == 0)
			{
				toggle_signal_();
			}
		}
		else if (signal_high_)
		{
			while (next_interval_ && next_interval_ < engine::context()->steady_clock())
			{
				interval_signal_(interval_index_++);
				if (interval_index_ < intervals_.size())
				{
					next_interval_ = started_at_ + intervals_[interval_index_];
				}
				else
				{
					next_interval_.reset();
				}
			}
		}
	}

	bool binary_input::is_signal_high() const
	{
		return signal_high_;
	}

	binary_input::set_signal& binary_input::on_set()
	{
		return set_signal_;
	}

	binary_input::clear_signal& binary_input::on_clear()
	{
		return clear_signal_;
	}

	binary_input::toggle_signal& binary_input::on_toggle()
	{
		return toggle_signal_;
	}

	binary_input::interval_signal& binary_input::on_interval()
	{
		return interval_signal_;
	}

};
