#ifndef __BINARY_OUTPUTS_HPP__
#define __BINARY_OUTPUTS_HPP__


#include <functional>
#include <chrono>
#include <boost/optional.hpp>

#include "engine.hpp"

#include "output_path.hpp"

namespace domotix3
{

	typedef std::function<void(bool)> binary_setter;

	binary_setter not_binary_setter(const binary_setter& rhs);

	class binary_setters
	{
	public:
		binary_setters(const std::vector<binary_setter>& binary_setters);

		void operator()(bool);

	private:
		std::vector<binary_setter> binary_setters_;
	};

	class binary_output : public module, public logic_execution_participant, ::std::enable_shared_from_this<binary_output>
	{
	public:
		binary_output(engine& engine, binary_setter setter);

		bool get_value() const;

		void set_value(bool value);

		bool toggle_value();

		void play_path(output_path path, bool end_value, const ::std::chrono::milliseconds& duration, bool repeat = false, const ::std::chrono::milliseconds& offset = ::std::chrono::milliseconds(0));

		void logic_execution();

		void stop_playing_path();

	private:
		bool value_;
		binary_setter setter_;
		::std::unique_ptr<output_path> path_;
		bool end_value_;
		::std::chrono::steady_clock::time_point started_at_;
		::std::chrono::milliseconds duration_;
		bool repeat_;
	};

};

#endif
