#ifndef _SUNSET_SUNRISE_HPP_
#define _SUNSET_SUNRISE_HPP_

#include <engine.hpp>
#include <binary_input.hpp>
#include "boost/date_time/local_time/local_time.hpp"

namespace domotix3
{
	class sunset_sunrise
	{
	public:
		sunset_sunrise();

		bool is_sun_up() const;

	private:
		void calculate_sunrise_and_sunset_time_for_current_day() const;

		mutable boost::posix_time::time_duration sunrise_at_;
		mutable boost::posix_time::time_duration sunset_at_;
		mutable boost::gregorian::date date_;
		mutable bool is_sun_up_;

		boost::local_time::time_zone_ptr europe_brussels_timezone_;
		const static int32_t europe_brussels_latitude_ = 50;
		const static int32_t europe_brussels_longitude_ = -4;
	};
};

#endif
