#if 0

#pragma once

#include <engine.hpp>

#include <bitset>
#include <analog_output.hpp>

namespace domotix3
{
	template <size_t N, typename T>
	class outputs_with_dimmer : public module, public logic_execution_participant
	{
	public:
		outputs_with_dimmer(engine& engine, analog_output<T>& dimmer)
			: module{engine}
			, dimmer_{dimmer}
			, outputs{}
		{
		}

		void toggle_output(size_t light)
		{
		}

		void set_output(size_t light, bool val = true)
		{
			this->outputs_.set(light, val);
		}

		bool is_output_set(size_t light) const
		{
			return this->outputs_.test(light);
		}

	protected:
		void logic_execution()
		{
		}

	private:
		std::bitset<N> outputs_;
		analog_output<T>& dimmer_;
	};

};

#endif

