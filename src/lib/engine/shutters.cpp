#include "shutters.hpp"

namespace domotix3
{

shutter::shutter(engine& engine, const std::chrono::milliseconds& up_duration, const std::chrono::milliseconds& down_duration, binary_setter output1, binary_setter output2, shutter_types shutter_type, shutter_initial_position initial_position)
	: module(engine)
	, up_duration_(up_duration)
	, down_duration_(down_duration)
	, output1_(output1)
	, output2_(output2)
	, type_(shutter_type)
{
	switch (initial_position)
	{
		case shutter_initial_position::initial_position_unknown:
			state_ = shutter_states::position_unknown;
			break;

		case shutter_initial_position::initial_position_up:
			state_ = shutter_states::up;
			break;

		case shutter_initial_position::initial_position_down:
			state_ = shutter_states::down;
			break;
	}
}

std::chrono::milliseconds shutter::get_up_duration()
{
	return up_duration_;
}

const std::chrono::milliseconds& shutter::get_up_duration() const
{
	return up_duration_;
}

std::chrono::milliseconds shutter::get_down_duration()
{
	return down_duration_;
}

const std::chrono::milliseconds& shutter::get_down_duration() const
{
	return down_duration_;
}

void shutter::logic_execution()
{
	if (state_ == shutter_states::moving_up ||
		state_ == shutter_states::moving_down)
	{
		auto time_already_moving = std::chrono::duration_cast<std::chrono::milliseconds>(engine::context()->steady_clock() - from_time_point_);
		if (state_ == shutter_states::moving_up && time_already_moving >= up_duration_)
		{
			state_ = shutter_states::up;
			output1_(false);
			output2_(false);
		}
		else if (state_ == shutter_states::moving_down && time_already_moving >= down_duration_)
		{
			state_ = shutter_states::down;
			output1_(false);
			output2_(false);
		}
	}
}

bool shutter::move_up()
{
	if (state_ != shutter_states::moving_up && state_ != shutter_states::up)
	{
		from_time_point_ = engine::context()->steady_clock();
		state_ = shutter_states::moving_up;
		switch (type_)
		{
			case shutter_types::enable_direction_outputs:
				output1_(true);
				output2_(true);
				break;
			case shutter_types::up_down_outputs:
				output1_(true);
				output2_(false);
				break;
		}

		return true;
	}

	return false;
}

bool shutter::move_down()
{
	if (state_ != shutter_states::moving_down && state_ != shutter_states::down)
	{
		from_time_point_ = engine::context()->steady_clock();
		state_ = shutter_states::moving_down;
		switch (type_)
		{
			case shutter_types::enable_direction_outputs:
				output1_(true);
				output2_(false);
				break;
			case shutter_types::up_down_outputs:
				output1_(false);
				output2_(true);
				break;
		}

		return true;
	}

	return false;
}

bool shutter::stop()
{
	switch (state_)
	{
		case shutter_states::moving_up:
			state_ = shutter_states::was_moving_up;
			output1_(false);
			output2_(false);
			return true;

		case shutter_states::moving_down:
			state_ = shutter_states::was_moving_down;
			output1_(false);
			output2_(false);
			return true;
	}

	return false;
}

shutter_states shutter::get_state() const
{
	return state_;
}

shutter_controller_base::shutter_controller_base(shutter& shutter, const std::chrono::milliseconds& short_press_time)
	: shutter_(shutter)
	, short_press_time_(short_press_time)
{
}

shutter_1_button_controller::shutter_1_button_controller(shutter& shutter, const std::chrono::milliseconds& short_press_time)
	: shutter_controller_base(shutter, short_press_time)
{
}

void shutter_1_button_controller::start_move_up_down()
{
	if (!move_uo_down_started_at_)
	{
		switch (shutter_.get_state())
		{
			case shutter_states::position_unknown:
			case shutter_states::up:
			case shutter_states::was_moving_up:
				shutter_.move_down();
				break;

			case shutter_states::down:
			case shutter_states::was_moving_down:
				shutter_.move_up();
				break;

			case shutter_states::moving_up:
			case shutter_states::moving_down:
				shutter_.stop();
				return;

		}
		move_uo_down_started_at_ = engine::context()->steady_clock();
	}
}

void shutter_1_button_controller::stop_move_up_down()
{
	if (move_uo_down_started_at_)
	{
		if (move_uo_down_started_at_.get() + short_press_time_ < engine::context()->steady_clock())
		{
			shutter_.stop();
		}

		move_uo_down_started_at_ = boost::none;
	}
}

shutter_2_buttons_controller::shutter_2_buttons_controller(shutter& shutter, const std::chrono::milliseconds& short_press_time)
	: shutter_controller_base(shutter, short_press_time)
{
}

void shutter_2_buttons_controller::start_move_up()
{
	if (!move_up_started_at_ && !move_down_started_at_)
	{
		switch (shutter_.get_state())
		{
			case shutter_states::position_unknown:
			case shutter_states::was_moving_up:
			case shutter_states::moving_down:
			case shutter_states::was_moving_down:
			case shutter_states::down:
				shutter_.move_up();
				break;

			case shutter_states::up:
				return;

			case shutter_states::moving_up:
				shutter_.stop();
				return;
		}

		move_up_started_at_ = engine::context()->steady_clock();
	}
}

void shutter_2_buttons_controller::stop_move_up()
{
	if (move_up_started_at_)
	{
		if (move_up_started_at_.get() + short_press_time_ < engine::context()->steady_clock())
		{
			shutter_.stop();
		}

		move_up_started_at_ = boost::none;
	}
}

void shutter_2_buttons_controller::start_move_down()
{
	if (!move_down_started_at_ && !move_up_started_at_)
	{
		switch (shutter_.get_state())
		{
			case shutter_states::position_unknown:
			case shutter_states::was_moving_up:
			case shutter_states::moving_up:
			case shutter_states::was_moving_down:
			case shutter_states::up:
				shutter_.move_down();
				break;

			case shutter_states::down:
				return;

			case shutter_states::moving_down:
				shutter_.stop();
				return;
		}

		move_down_started_at_ = engine::context()->steady_clock();
	}
}

void shutter_2_buttons_controller::stop_move_down()
{
	if (move_down_started_at_)
	{
		if (move_down_started_at_.get() + short_press_time_ < engine::context()->steady_clock())
		{
			shutter_.stop();
		}

		move_down_started_at_ = boost::none;
	}
}

};
