#pragma once

#include <engine.hpp>

namespace domotix3
{
	class manual_pir_light : public module, public logic_execution_participant
	{
	public:
		enum class states
		{
			off,
			manual_temp_on,
			manual_on,
			pir_temp_on,
			manual_temp_off
		};

		enum class light_states
		{
			off,
			on_pir,
			on_manual
		};

		typedef boost::signals2::signal<void(light_states)> light_signal;

	public:
		manual_pir_light(engine& engine, const std::chrono::milliseconds& manual_duration, const std::chrono::milliseconds& temp_off_duration, const std::chrono::milliseconds& pir_duration);
		~manual_pir_light();

		states get_state() const;

		// signals
		light_signal& on_light();

		// inputs
		void light_on_temporary_or_light_off();
		void light_on_forever();
		void pir_triggered();

		// actions
		void off();

	protected:
		void logic_execution();

	private:
		void manual_temp_off();

		std::chrono::milliseconds manual_duration_;
		std::chrono::milliseconds temp_off_duration_;
		std::chrono::milliseconds pir_duration_;

		light_signal light_signal_;

		states state_;

		boost::optional<std::chrono::steady_clock::time_point> timer_expires_at_;
	};

};
