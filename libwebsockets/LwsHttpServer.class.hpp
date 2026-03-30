#ifndef LIBWEBSOCKETS_EXAMPLE_LWSHTTPSERVER_CLASS_HPP
#define LIBWEBSOCKETS_EXAMPLE_LWSHTTPSERVER_CLASS_HPP

#include <libwebsockets.h>
#include <string>
#include <cstring>
#include <csignal>
#include <iostream>
#include <mutex>

enum SwitchType
{
	LWS_SW_H2_PRIOR_KNOWLEDGE,
	LWS_SW_D,
	LWS_SW_HELP
};

class LwsHttpServer
{
	private:
		bool interrupted;
		lws_context * context;
		lws_http_mount mount{};
		lws_context_creation_info info{};
		int log_level;

		LwsHttpServer();

		void setupMount();

		void setupContextInfo();

	public:
		LwsHttpServer ( const LwsHttpServer & ) = delete;
		LwsHttpServer & operator= ( const LwsHttpServer & ) = delete;

		static LwsHttpServer& getInstance();

		static void signalHandler(int sig);

		void interrupt();

		bool isInterrupted() const;

		void setLogLevel(int level);

		void enableH2PriorKnowledge(bool enable);

		bool initialize();

		int run() const;

		void shutdown();

		~LwsHttpServer();

};

#endif //LIBWEBSOCKETS_EXAMPLE_LWSHTTPSERVER_CLASS_HPP
