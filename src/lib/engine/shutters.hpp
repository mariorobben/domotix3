#ifndef __SHUTTERS_HPP__
#define __SHUTTERS_HPP__

#include <stdint.h>
#include <boost/static_assert.hpp>
#include <boost/chrono.hpp>
#include <boost/date_time.hpp>
#include <boost/optional.hpp>

#include "engine.hpp"
#include "binary_output.hpp"

namespace domotix3
{
	
	enum class shutter_types
	{
		enable_direction_outputs,
		up_down_outputs
	};

	enum class shutter_states
	{
		position_unknown,
		moving_up,
		was_moving_up,
		up,
		moving_down,
		was_moving_down,
		down
	};

	enum class shutter_initial_position
	{
		initial_position_unknown,
		initial_position_up,
		initial_position_down
	};

	class shutter : public module, public logic_execution_participant
	{
	public:
		shutter(engine& engine, const std::chrono::milliseconds& up_duration, const std::chrono::milliseconds& down_duration, binary_setter output1, binary_setter output2, shutter_types shutter_type = shutter_types::enable_direction_outputs, shutter_initial_position initial_position = shutter_initial_position::initial_position_unknown);
	
		std::chrono::milliseconds get_up_duration();
		const std::chrono::milliseconds& get_up_duration() const;
		std::chrono::milliseconds get_down_duration();
		const std::chrono::milliseconds& get_down_duration() const;

		void logic_execution();

		shutter_states get_state() const;

		bool move_up();
		bool move_down();
		bool stop();

	private:
		std::chrono::milliseconds up_duration_;
		std::chrono::milliseconds down_duration_;
		binary_setter output1_;
		binary_setter output2_;
		shutter_types type_;
		shutter_states state_;

		std::chrono::steady_clock::time_point from_time_point_;
	};

	class shutter_controller_base
	{
	public:
		shutter_controller_base(shutter& shutter, const std::chrono::milliseconds& short_press_time);

	protected:
		shutter& shutter_;
		std::chrono::milliseconds short_press_time_;
	};

	class shutter_1_button_controller : protected shutter_controller_base
	{
	public:
		shutter_1_button_controller(shutter& shutter, const std::chrono::milliseconds& short_press_time);

		void start_move_up_down();
		void stop_move_up_down();

	private:
		boost::optional< std::chrono::steady_clock::time_point > move_uo_down_started_at_;
	};

	class shutter_2_buttons_controller : protected shutter_controller_base
	{
	public:
		shutter_2_buttons_controller(shutter& shutter, const std::chrono::milliseconds& short_press_time);
	
		void start_move_up();
		void stop_move_up();
		void start_move_down();
		void stop_move_down();

	private:
		boost::optional< std::chrono::steady_clock::time_point > move_up_started_at_;
		boost::optional< std::chrono::steady_clock::time_point > move_down_started_at_;
	};

};

#endif
