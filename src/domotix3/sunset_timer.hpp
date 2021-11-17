#pragma once

#include "boost/date_time.hpp" 
#include "boost/date_time/local_time/local_time.hpp"

boost::posix_time::time_duration calc_local_sunset_time(boost::gregorian::date date, boost::local_time::time_zone_ptr time_zone, double latidude, double longitude);
boost::posix_time::time_duration calc_local_sunrise_time(boost::gregorian::date date, boost::local_time::time_zone_ptr time_zone, double latidude, double longitude);
