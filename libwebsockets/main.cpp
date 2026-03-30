#include "LwsHttpServer.class.hpp"

//
// Source:. https://github.com/warmcat/libwebsockets/blob/main/minimal-examples-lowlevel/http-server/minimal-http-server/minimal-http-server.c
//

#include <libwebsockets.h>
#include <vector>

struct lws_switches
{
	const char *sw;
	const char *doc;
};

void print_help(std::vector<lws_switches> & switches)
{
	for (const auto & [sw, doc] : switches)
	{
		std::cout << sw << doc << std::endl;
	}
}



int main(const int argc, const char ** argv)
{
	std::vector<lws_switches> switches = {
		{"--h2-prior-knowledge  ", "Enable --h2-prior-knowledge feature"},
		{"-d                    ", "debug logs (e.g. -d 15)"},
		{"--help                ", "show this help information"},
	};
	(void) switches;
	if (lws_cmdline_option(argc, argv, switches[LWS_SW_HELP].sw))
	{
		print_help(switches);
		return 0;
	}

	LwsHttpServer & instance = LwsHttpServer::getInstance();

	std::signal(SIGINT, LwsHttpServer::signalHandler);

	const char * p = lws_cmdline_option(argc, argv, switches[LWS_SW_D].sw);

	if (p)
	{
		instance.setLogLevel(std::atoi(p));
	}

	if (!instance.initialize())
	{
		return EXIT_FAILURE;
	}

	if (lws_cmdline_option(argc, argv, switches[LWS_SW_H2_PRIOR_KNOWLEDGE].sw))
	{
		instance.enableH2PriorKnowledge(true);
	}

	const int result = instance.run();
	instance.shutdown();

	return result;
}