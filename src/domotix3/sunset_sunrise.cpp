#include "sunset_sunrise.hpp"

#include "sunset_timer.hpp"

namespace domotix3
{
	sunset_sunrise::sunset_sunrise()
		: europe_brussels_timezone_( new boost::local_time::posix_time_zone("CET+01CEST+01,M3.5.0/02:00,M10.5.0/03:00") )
	{
	}

	bool sunset_sunrise::is_sun_up() const
	{
		calculate_sunrise_and_sunset_time_for_current_day();
		return engine::context()->local_time().time_of_day() >= sunrise_at_ && engine::context()->local_time().time_of_day() < sunset_at_;
	}

	void sunset_sunrise::calculate_sunrise_and_sunset_time_for_current_day() const
	{
		if (date_ != engine::context()->local_time().date())
		{
			date_ = engine::context()->local_time().date();
			sunrise_at_ = calc_local_sunrise_time(date_, europe_brussels_timezone_, europe_brussels_latitude_, europe_brussels_longitude_);
			sunset_at_ = calc_local_sunset_time(date_, europe_brussels_timezone_, europe_brussels_latitude_, europe_brussels_longitude_);
		}
	}
};
