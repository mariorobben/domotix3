#include "manual_pir_light.hpp"

namespace domotix3
{
	manual_pir_light::manual_pir_light(engine& engine, const std::chrono::milliseconds& manual_duration, const std::chrono::milliseconds& temp_off_duration, const std::chrono::milliseconds& pir_duration)
		: module(engine)
		, manual_duration_(manual_duration )
		, temp_off_duration_(temp_off_duration)
		, pir_duration_(pir_duration)
		, light_signal_()
		, state_(states::off)
		, timer_expires_at_()
	{
	}

	manual_pir_light::~manual_pir_light()
	{
	}

	manual_pir_light::states manual_pir_light::get_state() const
	{
		return state_;
	}

	manual_pir_light::light_signal& manual_pir_light::on_light()
	{
		return light_signal_;
	}

	void manual_pir_light::light_on_temporary_or_light_off()
	{
		switch (state_)
		{
		case states::off:
		case states::pir_temp_on:
		case states::manual_temp_off:
			light_signal_(light_states::on_manual);
			timer_expires_at_ = engine::context()->steady_clock() + manual_duration_;
			state_ = states::manual_temp_on;
			break;

		case states::manual_temp_on:
		case states::manual_on:
			manual_temp_off();
			break;
		}
	}

	void manual_pir_light::light_on_forever()
	{
		switch (state_)
		{
		case states::off:
		case states::pir_temp_on:
		case states::manual_temp_off:
			light_signal_(light_states::on_manual);
			timer_expires_at_.reset();
			state_ = states::manual_on;
			break;

		case states::manual_temp_on:
			timer_expires_at_.reset();
			state_ = states::manual_on;
			break;
		}
	}

	void manual_pir_light::pir_triggered()
	{
		switch (state_)
		{
		case states::off:
			light_signal_(light_states::on_pir);
			timer_expires_at_ = engine::context()->steady_clock() + pir_duration_;
			state_ = states::pir_temp_on;
			break;

		case states::manual_temp_on:
		case states::pir_temp_on:
			timer_expires_at_ = engine::context()->steady_clock() + pir_duration_;
			break;
		}
	}

	void manual_pir_light::off()
	{
		switch (state_)
		{
		case states::manual_temp_on:
		case states::manual_on:
		case states::pir_temp_on:
			manual_temp_off();
			break;
		}
	}

	void manual_pir_light::logic_execution()
	{
		if (timer_expires_at_ && timer_expires_at_ < engine::context()->steady_clock())
		{
			// timer expired
			switch (state_)
			{
			case states::manual_temp_on:
			case states::pir_temp_on:
				light_signal_(light_states::off);
				break;

			case states::manual_temp_off:
				break;

			default:
				assert(false);
				break;
			}

			timer_expires_at_.reset();
			state_ = states::off;
		}
	}

	void manual_pir_light::manual_temp_off()
	{
		light_signal_(light_states::off);
		timer_expires_at_ = engine::context()->steady_clock() + temp_off_duration_;
		state_ = states::manual_temp_off;
	}

};
