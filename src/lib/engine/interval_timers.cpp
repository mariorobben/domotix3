#include "interval_timers.h"

namespace domotix3
{

	using namespace std::chrono;

	interval_timer::interval_timer(engine& engine, const std::chrono::milliseconds& duration1)
		: module(engine)
		, push_signal_()
		, interval_signal_()
		, release_signal_()
		, durations_()
	{
		durations_.push_back(duration1);
	}

	interval_timer::interval_timer(engine& engine, const std::chrono::milliseconds& duration1, const std::chrono::milliseconds& duration2)
		: module(engine)
		, push_signal_()
		, interval_signal_()
		, release_signal_()
		, durations_()
	{
		durations_.push_back(duration1);
		durations_.push_back(duration2);
	}

	interval_timer::interval_timer(engine& engine, const std::chrono::milliseconds& duration1, const std::chrono::milliseconds& duration2, const std::chrono::milliseconds& duration3)
		: module(engine)
		, push_signal_()
		, interval_signal_()
		, release_signal_()
		, durations_()
	{
		durations_.push_back(duration1);
		durations_.push_back(duration2);
		durations_.push_back(duration3);
	}

	interval_timer::~interval_timer()
	{
	}

	void interval_timer::logic_execution()
	{
		if (started_at_)
		{
			if (next_deadline_ && next_deadline_ < engine::context()->steady_clock())
			{
				interval_signal_(++duration_index_);
				if (duration_index_ < durations_.size())
				{
					next_deadline_ = started_at_.get() + durations_[duration_index_];
				}
				else
				{
					next_deadline_.reset();
				}
			}
		}
	}

	void interval_timer::start()
	{
		if (!started_at_)
		{
			started_at_ = engine::context()->steady_clock();
			duration_index_ = 0;
			next_deadline_ = started_at_.get() + durations_[duration_index_];

			push_signal_();
		}
	}

	void interval_timer::stop()
	{
		if (started_at_)
		{
			release_signal_(duration_index_);

			started_at_ = boost::none;
		}
	}

	interval_timer::push_signal& interval_timer::on_push()
	{
		return push_signal_;
	}

	interval_timer::interval_signal& interval_timer::on_interval()
	{
		return interval_signal_;
	}

	interval_timer::release_signal& interval_timer::on_release()
	{
		return release_signal_;
	}

	delay_timer::delay_timer(engine& engine, const std::chrono::milliseconds& duration)
		: module(engine)
		, duration_(duration)
		, next_deadline_(boost::none)
		, on_signal_()
		, off_signal_()
	{
	}

	void delay_timer::logic_execution()
	{
		if (next_deadline_)
		{
			if (next_deadline_ < engine::context()->steady_clock())
			{
				next_deadline_ = boost::none;

				off_signal_();
			}
		}
	}

	void delay_timer::reset()
	{
		if (!next_deadline_)
		{
			on_signal_();
		}

		next_deadline_ = engine::context()->steady_clock() + duration_;
	}

	delay_timer::on_signal& delay_timer::on_on()
	{
		return on_signal_;
	}

	delay_timer::off_signal& delay_timer::on_off()
	{
		return off_signal_;
	}

};
