#include "wago_http_client.hpp"

#include <ArduinoJson-v6.18.5.h>
#include <Base64.h>

#include <iostream>

namespace wago
{
	wago_http_client::wago_http_client(const std::string& deviceName, const std::string& host, int port)
		: _deviceName{ deviceName }
		, _client{ host, port }
		, _headers{{"Content-Type", "application/json"}}
		, _doc{8192}
	{
	}

	wago_http_client::~wago_http_client()
	{
	}

	status wago_http_client::get_status()
	{
		std::ostringstream url;
		url << "/devices/wago/status/" << _deviceName;
		auto res = _client.Get(url.str().c_str());
		if (res->status != 200)
		{
			return status(0, 0, 0, 0, 0, 0);
		}
		deserializeJson(_doc, res->body);
		return status(_doc["bitCount"].as<int>(), _doc["terminalCount"].as<int>(), _doc["bitCountAnalogInput"].as<int>(), _doc["bitCountAnalogOutput"].as<int>(), _doc["bitCountDigitalInput"].as<int>(), _doc["bitCountDigitalOutput"].as<int>());
	}

	terminals_type wago_http_client::get_terminals()
	{
		terminals_type result;
		std::ostringstream url;
		url << "/devices/wago/terminals/" << _deviceName;
		auto res = _client.Get(url.str().c_str());
		if (res->status == 200)
		{
			deserializeJson(_doc, res->body);
			JsonArray array = _doc.as<JsonArray>();
			for (auto v : array)
			{
				result.emplace_back(v["position"].as<int>(),
					v["type"].as<int>(),
					v["bitOffsetOut"].as<int>(),
					v["bitSizeOut"].as<int>(),
					v["bitOffsetIn"].as<int>(),
					v["bitSizeIn"].as<int>(),
					v["channels"].as<int>(),
					v["piFormat"].as<int>());
			}
		}
		return result;
	}

	void wago_http_client::write_bytes(const write_bytes_region_type& region)
	{
		std::ostringstream url;
		url << "/devices/wago/io/" << _deviceName << "?offset=" << region.first;
		_doc.set(Base64::base64_encode(std::string(region.second.begin(), region.second.end())));
		std::ostringstream body;
		serializeJson(_doc, body);
		_client.Put(url.str().c_str(), body.str(), "application/json");
	}

	bytes_type wago_http_client::read_bytes(const read_bytes_region_type& region)
	{
		std::ostringstream url;
		url << "/devices/wago/io/" << _deviceName << "?offset=" << region.first << "&size=" << region.second;
		auto res = _client.Get(url.str().c_str(), _headers);
		if (res->status == 200)
		{
			deserializeJson(_doc, res->body);
			auto bytes = Base64::base64_decode(_doc.as<std::string>());
			return bytes_type(bytes.begin(), bytes.end());
		}
		return bytes_type();
	}

};
