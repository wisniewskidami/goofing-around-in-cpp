#include "LwsHttpServer.class.hpp"

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
	mount.mountpoint = "/";
	mount.origin = "./mount-origin";
	mount.def = "index.html";
	mount.origin_protocol = LWSMPRO_FILE;
	mount.mountpoint_len = 1;
}

void LwsHttpServer::setupContextInfo()
{
	std::memset(&this->info, 0, sizeof(this->info));
	info.port = 7682;
	info.mounts = &this->mount;
	info.error_document_404 = "/404.html";
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
