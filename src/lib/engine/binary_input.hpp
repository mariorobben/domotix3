#ifndef __BINARY_INPUT_HPP__
#define __BINARY_INPUT_HPP__

#include <stdint.h>
#include <boost/signals2.hpp>
#include <boost/optional.hpp>
#include "engine.hpp"

#include <functional>
#include <vector>

namespace domotix3
{

	typedef std::function<bool ()> binary_getter;

	binary_getter not_binary_getter(binary_getter rhs);
	binary_getter and_binary_getter(binary_getter lhs, binary_getter rhs);
	binary_getter or_binary_getter(binary_getter lhs, binary_getter rhs);

	class exclusive_binary_getter
	{
	public:
		exclusive_binary_getter(const std::vector<binary_getter>& binary_getters);

		bool operator()() const;

	private:
		mutable boost::optional<binary_getter> locked_getter_;
		std::vector<binary_getter> binary_getters_;
	};

	enum class toggle_types
	{
		toggle_on_rising_edge,
		toggle_on_falling_edge,
		toggle_on_both_edges,
		toggle_when_no_interval_is_reached
	};

	class binary_input : public module, public logic_execution_participant
	{
	public:
		typedef boost::signals2::signal<void()> set_signal;
		typedef boost::signals2::signal<void()> clear_signal;
		typedef boost::signals2::signal<void ()> toggle_signal;
		typedef boost::signals2::signal<void(size_t)> interval_signal;

		binary_input(engine& engine, binary_getter getter, toggle_types toggle_type = toggle_types::toggle_on_rising_edge);
		binary_input(engine& engine, binary_getter getter, const std::vector<std::chrono::milliseconds>& intervals);
		binary_input(engine& engine, binary_getter getter, toggle_types toggle_type, const std::vector<std::chrono::milliseconds>& intervals);

		~binary_input();
	
		void logic_execution();

		bool is_signal_high() const;

		set_signal& on_set();
		clear_signal& on_clear();
		toggle_signal& on_toggle();
		interval_signal& on_interval();

	private:
		void set_signal_high(bool signal_high);

		binary_getter getter_;
		toggle_types toggle_type_;

		bool signal_high_;

		std::vector<::std::chrono::milliseconds> intervals_;
		::std::chrono::steady_clock::time_point started_at_;
		boost::optional< ::std::chrono::steady_clock::time_point > next_interval_;
		int32_t interval_index_;

		// signals
		set_signal set_signal_;
		clear_signal clear_signal_;
		toggle_signal toggle_signal_;
		interval_signal interval_signal_;
	};

};

#endif
