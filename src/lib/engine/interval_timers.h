#ifndef __INTERVAL_TIMERS_H__
#define __INTERVAL_TIMERS_H__

#include <stdint.h>
#include <boost/signals2.hpp>
#include <boost/function.hpp>
#include "engine.hpp"

#include <algorithm>

namespace domotix3
{
	
class interval_timer : public module, public logic_execution_participant
{
public:
	typedef boost::signals2::signal<void () > push_signal;
	typedef boost::signals2::signal<void (int32_t) > interval_signal;
	typedef boost::signals2::signal<void (int32_t) > release_signal;
	
	interval_timer(engine& engine, const std::chrono::milliseconds& duration1);
	interval_timer(engine& engine, const std::chrono::milliseconds& duration1, const std::chrono::milliseconds& duration2);
	interval_timer(engine& engine, const std::chrono::milliseconds& duration1, const std::chrono::milliseconds& duration2, const std::chrono::milliseconds& duration3);

	template<typename ForwardIterator >
	interval_timer(engine& engine, ForwardIterator begin, ForwardIterator end)
		: module(engine)
		, push_signal_()
		, interval_signal_()
		, release_signal_()
		, durations_()
	{
		std::copy(begin, end, std::back_inserter(durations_));
	}

	~interval_timer();
	
	void logic_execution();
	
	void start();
	void stop();

	push_signal& on_push();
	interval_signal& on_interval();
	release_signal& on_release();

private:
	std::vector< std::chrono::milliseconds > durations_;
	boost::optional< std::chrono::steady_clock::time_point > started_at_;
	boost::optional< std::chrono::steady_clock::time_point > next_deadline_;

	int32_t duration_index_;

	// signals
	push_signal push_signal_;
	interval_signal interval_signal_;
	release_signal release_signal_;
};

class delay_timer : public module, public logic_execution_participant
{
public:
	typedef boost::signals2::signal<void() > on_signal;
	typedef boost::signals2::signal<void() > off_signal;

	delay_timer(engine& engine, const std::chrono::milliseconds& duration);

	void logic_execution();

	void reset();

	on_signal& on_on();
	off_signal& on_off();

private:
	std::chrono::milliseconds duration_;
	boost::optional< std::chrono::steady_clock::time_point > next_deadline_;

	// signals
	on_signal on_signal_;
	off_signal off_signal_;
};

};

#endif
