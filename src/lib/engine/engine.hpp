#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <windows.h>
#define BOOST_ASIO_NO_WIN32_LEAN_AND_MEAN

#include <chrono>

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/signals2.hpp>

#include <set>

namespace domotix3
{

class engine;

class module
{
public:
	module(engine& engine);
	virtual ~module();

	engine& get_engine() const;
	
protected:
	engine& engine_;
};

class start_stop_participant
{
public:
	virtual void start() = 0;
	virtual void stop() = 0;
};

class input_scan_participant
{
public:
	virtual void input_scan() = 0;
};

class logic_execution_participant
{
public:
	virtual void logic_execution() = 0;
};

class output_scan_participant
{
public:
	virtual void output_scan() = 0;
};

class datetime_changed_participant
{
public:
	virtual void datetime_changed() = 0;
};

class engine_context
{
	friend class engine;

public:
	engine_context();
	~engine_context();

	engine* get_engine();
	boost::posix_time::ptime utc_time();
	boost::posix_time::ptime local_time();
	std::chrono::steady_clock::time_point steady_clock();
	boost::posix_time::ptime prev_utc_time();
	boost::posix_time::ptime prev_local_time();

private:
	engine* engine_;
	boost::posix_time::ptime utc_time_;
	boost::posix_time::ptime local_time_;
	std::chrono::steady_clock::time_point steady_clock_;
	boost::posix_time::ptime prev_utc_time_;
	boost::posix_time::ptime prev_local_time_;
};

class engine
{
	friend class module;

	typedef boost::signals2::signal<void(module*)> on_state_changed;

protected:
	engine();

public:
	~engine();

	void start(const std::chrono::milliseconds& scan_time);
	void stop();

	static engine_context* context();

protected:
	virtual void on_before_start();
	virtual void on_after_start();
	virtual void on_before_stop();
	virtual void on_after_stop();
	virtual void on_before_input_scan();
	virtual void on_after_input_scan();
	virtual void on_before_logic_execution();
	virtual void on_after_logic_execution();
	virtual void on_before_output_scan();
	virtual void on_after_output_scan();
	virtual void on_before_datetime_changed();
	virtual void on_after_datetime_changed();

	virtual void log_exception(std::exception& ex);

private:
	void run();
	void process_cycle();

	boost::asio::io_service io_service_;
	boost::asio::io_service::work work_;
	boost::asio::basic_waitable_timer<std::chrono::steady_clock> timer_;
	std::chrono::milliseconds scan_time_;
	boost::thread thread_;
	std::vector< module* > modules_;
	std::vector< start_stop_participant* > start_stop_participants_;
	std::vector< input_scan_participant* > input_scan_participants_;
	std::vector< logic_execution_participant* > logic_execution_participants_;
	std::vector< output_scan_participant* > output_scan_participants_;
	std::vector< datetime_changed_participant* > datetime_changed_participants_;
	static boost::thread_specific_ptr<engine_context> context_;

	// states
	on_state_changed state_changed_signal_;
};

};

#endif
