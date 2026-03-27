#include "WebSockets.hpp"

#include <ranges>
#include <thread>
#include <uuid/uuid.h>

WebSockets::WebSockets() :
	M_info(nullptr),
	M_context(nullptr),
	index(0)
{
	for (size_t i {}; i < 256; ++i)
	{
		lws_protocols protocol = {};
		this->M_vector_protocols.emplace_back(protocol);
	}
}

WebSockets::~WebSockets()
{
	if (this->M_context)
		lws_context_destroy(this->M_context);
}

void WebSockets::connect(const std::function<void()> & callback)
{
	this->createContext();
	if (!this->M_context)
		throw WebSocketsException("cannot create context", -9);

	for (auto & value : this->M_map_protocols | std::views::values)
		value.connect(this->M_context);

	while (!this->connected())
	{
		this->runStep();
		std::this_thread::yield();
	}

	callback();
}

void WebSockets::addProtocol(
	const std::string name, const std::string path, const std::string address, int port, bool ssl_certificate,
	const std::function<bool(std::string json, std::string name)> & callback
)
{
	Protocol * p = &this->M_map_protocols[name];
	*p = Protocol {name, &this->M_vector_protocols[index++], p};
	p->setAddress(address, path, port, ssl_certificate, callback);
}

std::string WebSockets::getId()
{
	uuid_t uuid;
	char uuid_str[64] = {};
	uuid_generate(uuid);
	uuid_unparse_lower(uuid, uuid_str);

	return {uuid_str};
}

void WebSockets::runStep()
{
	time_t rx_time;

	std::this_thread::yield();
	lws_service(this->M_context, 0);
	time(&rx_time);
	for (auto & value : this->M_map_protocols | std::views::values)
	{
		if ( value.isConnected ( ) && value.getWsi ( ) || value.last_update && rx_time > 300 )
		{
			lws_close_reason ( value.getWsi(), LWS_CLOSE_STATUS_NOSTATUS, nullptr, 0 );
			value.clearWsi ( );
			value.connect ( this->M_context );
		}
	}
}

void WebSockets::run()
{
	this->isRunning = true;
	while ( this->isRunning )
		this->runStep ( );
}

void WebSockets::stop()
{
	this->isRunning = false;
}

bool WebSockets::connected()
{
	for (auto & value : this->M_map_protocols | std::views::values)
		if (!value.isConnected())
			return false;
	return true;
}

void WebSockets::createContext()
{
	memset(&this->M_info, 0, sizeof(this->M_info));
	this->M_info.options = LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT;
	this->M_info.port = CONTEXT_PORT_NO_LISTEN;
	this->M_info.protocols = this->M_vector_protocols.data();
	this->M_info.timeout_secs = 10;
	this->M_info.fd_limit_per_thread = 8;

	this->M_context = lws_create_context(&this->M_info);

	if (!this->M_context)
	{
		lwsl_err("lws init failed\n");
		throw WebSocketsException("lws init failed", -9);
	}

	lws_set_log_level ( LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE , nullptr );
}
