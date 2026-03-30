#include "LwsHttpServer.class.hpp"

#include <cstring>
#include <iostream>

LwsHttpServer::LwsHttpServer()
	: interrupted ( false )
	, context ( nullptr )
	, log_level ( LLL_USER | LLL_ERR | LLL_WARN | LLL_NOTICE )
{
	this->setupMount();
}

LwsHttpServer::~LwsHttpServer()
{
	this->shutdown();
}

void LwsHttpServer::setupMount()
{
	// setup mountpoint for the websock to be at / if you ask through the websock
	mount.mountpoint = "/";
	// in our case we mount the origin as mount-origin directory, it's insides
	// will be visible for the end-user.
	mount.origin = "./mount-origin";
	// our entry for "/" is index.html
	mount.def = "index.html";
	// the mount origin is one of the enum in lws_mount_protocols
	// LWSMPRO_{
	//	HTTP , HTTPS , FILE , CGI , REDIR_HTTP , REDIR_HTTPS, CALLBACK
	//  NO_MOUNT
	// }
	mount.origin_protocol = LWSMPRO_FILE;
	// length of mountpoint string
	mount.mountpoint_len = 1;
}

void LwsHttpServer::setupContextInfo()
{
	std::memset(&this->info, 0, sizeof(this->info));
	// here we setup context information for the creation of this->context
	// port could be given in ctor, but that's okay too
	info.port = 7682;
	// we have mounts that need to be defined beforehand and are during ctor
	info.mounts = &this->mount;
	// our error page for mounts is 404.html, in case something goes bad
	// Internal errors I think are handled by the lib websockets themselves.
	info.error_document_404 = "/404.html";
	// Send lws default http headers recommended by Mozilla Observatory for Security.
	// there are other options or self defined ones, if you want to go in detail,
	// you must read their docs.
	info.options = LWS_SERVER_OPTION_HTTP_HEADERS_SECURITY_BEST_PRACTICES_ENFORCE;
}

void LwsHttpServer::signalHandler(int sig)
{
	getInstance().interrupt();
}

LwsHttpServer& LwsHttpServer::getInstance()
{
	static LwsHttpServer instance;
	return instance;
}

void LwsHttpServer::interrupt()
{
	this->interrupted = true;
}

bool LwsHttpServer::isInterrupted() const
{
	return this->interrupted;
}

void LwsHttpServer::setLogLevel(const int level)
{
	this->log_level = level;
	lws_set_log_level(log_level, nullptr);
}


void LwsHttpServer::enableH2PriorKnowledge(bool enable)
{
	if (enable)
	{
		info.options |= LWS_SERVER_OPTION_H2_PRIOR_KNOWLEDGE;
	}
}

bool LwsHttpServer::initialize()
{
	this->setupContextInfo();
	// Here we create the context
	this->context = lws_create_context( &this->info );
	if (!this->context)
	{
		std::cerr << "lws init failed" << std::endl;
		return false;
	}
	return true;
}

int LwsHttpServer::run() const
{
	if (!this->context)
	{
		std::cerr << "Context not initialized. Call initialize() first." << std::endl;
		return 1;
	}

	std::cout << "LWS minimal http server | visit http://localhost:" << this->info.port << std::endl;

	int n = 0;
	while (n >= 0 && !this->interrupted)
	{
		// We run the service yupii
		n = lws_service( this->context, 0 );
	}
	return 0;
}

void LwsHttpServer::shutdown()
{
	if (this->context)
	{
		lws_context_destroy(this->context);
		this->context = nullptr;
	}
}
