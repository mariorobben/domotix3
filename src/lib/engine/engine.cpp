#include "engine.hpp"

#include <iostream>
#include <exception>

#include <boost/chrono.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "boost/date_time/c_local_time_adjustor.hpp"

#include <stdio.h>


namespace domotix3
{

using boost::thread;
using boost::bind;
using namespace std::chrono;

using std::exception;

boost::thread_specific_ptr<domotix3::engine_context> domotix3::engine::context_;

engine_context* engine::context()
{
	return context_.get();
}

engine_context::engine_context()
{
}

engine_context::~engine_context()
{
}

engine* engine_context::get_engine()
{
	return engine_;
}

boost::posix_time::ptime engine_context::utc_time()
{
	return utc_time_;
}

boost::posix_time::ptime engine_context::local_time()
{
	return local_time_;
}

boost::posix_time::ptime engine_context::prev_utc_time()
{
	return prev_utc_time_;
}

boost::posix_time::ptime engine_context::prev_local_time()
{
	return prev_local_time_;
}

std::chrono::steady_clock::time_point engine_context::steady_clock()
{
	return steady_clock_;
}

module::module(engine& engine)
	: engine_(engine)
{	
	engine_.modules_.push_back(this);
}

module::~module()
{
	engine_.modules_.erase(std::remove(engine_.modules_.begin(), engine_.modules_.end(), this), engine_.modules_.end());
}

engine& module::get_engine() const
{
	return engine_;
}

engine::engine()
	: io_service_()
	, work_(io_service_)
	, timer_(io_service_)
{
}

engine::~engine()
{
}

void engine::start(const std::chrono::milliseconds& scan_time)
{
	scan_time_ = scan_time;

	thread_ = thread(bind(&engine::run, this));
}

void engine::stop()
{
	io_service_.stop();

	thread_.join();
}

void engine::run()
{       
	typedef boost::date_time::c_local_adjustor<boost::posix_time::ptime> local_adj;

	context_.reset(new engine_context());

	context_.get()->engine_ = this;
	context_.get()->utc_time_ = context_.get()->prev_utc_time_ = boost::posix_time::microsec_clock::universal_time(),
	context_.get()->local_time_ = context_.get()->prev_local_time_ = local_adj::utc_to_local(context_.get()->utc_time_);
	context_.get()->steady_clock_ = steady_clock::now();

	for (auto module : modules_)
	{
		if (dynamic_cast<start_stop_participant*>(module)) start_stop_participants_.push_back(dynamic_cast<start_stop_participant*>(module));
		if (dynamic_cast<input_scan_participant*>(module)) input_scan_participants_.push_back(dynamic_cast<input_scan_participant*>(module));
		if (dynamic_cast<logic_execution_participant*>(module)) logic_execution_participants_.push_back(dynamic_cast<logic_execution_participant*>(module));
		if (dynamic_cast<output_scan_participant*>(module)) output_scan_participants_.push_back(dynamic_cast<output_scan_participant*>(module));
		if (dynamic_cast<datetime_changed_participant*>(module)) datetime_changed_participants_.push_back(dynamic_cast<datetime_changed_participant*>(module));
	}

	try
	{
		on_before_start();
	}
	catch (exception& ex)
	{
		log_exception(ex);
	}

	for (auto ssp : start_stop_participants_)
	{
		try
		{
			ssp->start();
		}
		catch (exception& ex)
		{
			log_exception(ex);
		}
	}

	try
	{
		on_after_start();
	}
	catch (exception& ex)
	{
		log_exception(ex);
	}
	
	context_.get()->engine_ = this;
	context_.get()->utc_time_ = boost::posix_time::microsec_clock::universal_time();
	context_.get()->local_time_ = local_adj::utc_to_local(context_.get()->utc_time_);

	process_cycle();

	io_service_.run();

	try
	{
		on_before_stop();
	}
	catch (exception& ex)
	{
		log_exception(ex);
	}

	for (auto ssp : start_stop_participants_)
	{
		try
		{
			ssp->stop();
		}
		catch (exception& ex)
		{
			log_exception(ex);
		}
	}

	try
	{
		on_after_stop();
	}
	catch (exception& ex)
	{
		log_exception(ex);
	}
}

void engine::process_cycle()
{
	typedef boost::date_time::c_local_adjustor<boost::posix_time::ptime> local_adj;

	static int i = 1;
	std::cout << "process cycle " << i++ << std::endl;

	context_.get()->steady_clock_ = steady_clock::now();

	// schedule next process_cycle
	timer_.expires_from_now(scan_time_);
	timer_.async_wait(boost::bind( &engine::process_cycle, this));

	try
	{
		on_before_input_scan();
	}
	catch (exception& ex)
	{
		log_exception(ex);
	}

	for (auto isp : input_scan_participants_)
	{
		try
		{
			isp->input_scan();
		}
		catch (exception& ex)
		{
			log_exception(ex);
		}
	}

	try
	{
		on_after_input_scan();
	}
	catch (exception& ex)
	{
		log_exception(ex);
	}
	try
	{
		on_before_logic_execution();
	}
	catch (exception& ex)
	{
		log_exception(ex);
	}

	for (auto lep : logic_execution_participants_)
	{
		try
		{
			lep->logic_execution();
		}
		catch (exception& ex)
		{
			log_exception(ex);
		}
	}

	try
	{
		on_after_logic_execution();
	}
	catch (exception& ex)
	{
		log_exception(ex);
	}

	try
	{
		on_before_output_scan();
	}
	catch (exception& ex)
	{
		log_exception(ex);
	}

	for (auto osp : output_scan_participants_)
	{
		try
		{
			osp->output_scan();
		}
		catch (exception& ex)
		{
			log_exception(ex);
		}
	}

	try
	{
		on_after_output_scan();
	}
	catch (exception& ex)
	{
		log_exception(ex);
	}

	auto new_utc_time = boost::posix_time::microsec_clock::universal_time();
	auto new_local_time = local_adj::utc_to_local(new_utc_time);
	auto steady_clock = steady_clock::now();
	auto utc_offset = (new_utc_time - context_.get()->utc_time_).total_milliseconds() - duration_cast<milliseconds>(steady_clock - context_.get()->steady_clock_).count();
	auto local_offset = (new_local_time - context_.get()->local_time_).total_milliseconds() - duration_cast<milliseconds>(steady_clock - context_.get()->steady_clock_).count();
	
	context_.get()->engine_ = this;
	context_.get()->prev_utc_time_ = context_.get()->utc_time_;
	context_.get()->prev_local_time_ = context_.get()->local_time_;
	context_.get()->utc_time_ = new_utc_time;
	context_.get()->local_time_ = new_local_time;

	if (abs(utc_offset) > 1000 ||
		abs(local_offset) > 1000)
	{
		try
		{
			on_before_datetime_changed();
		}
		catch (exception& ex)
		{
			log_exception(ex);
		}

		for (auto dtcp : datetime_changed_participants_)
		{
			try
			{
				dtcp->datetime_changed();
			}
			catch (exception& ex)
			{
				log_exception(ex);
			}
		}
			
		try
		{
			on_after_datetime_changed();
		}
		catch (exception& ex)
		{
			log_exception(ex);
		}
	}
}

void engine::on_before_start()
{
}

void engine::on_after_start()
{
}


void engine::on_before_stop()
{
}

void engine::on_after_stop()
{
}

void engine::on_before_input_scan()
{
}

void engine::on_after_input_scan()
{
}

void engine::on_before_logic_execution()
{
}

void engine::on_after_logic_execution()
{
}

void engine::on_before_output_scan()
{
}

void engine::on_after_output_scan()
{
}

void engine::on_before_datetime_changed()
{
}

void engine::on_after_datetime_changed()
{
}

void engine::log_exception(std::exception& ex)
{
}

};
