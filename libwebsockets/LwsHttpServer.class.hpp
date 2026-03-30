#ifndef LIBWEBSOCKETS_EXAMPLE_LWSHTTPSERVER_CLASS_HPP
#define LIBWEBSOCKETS_EXAMPLE_LWSHTTPSERVER_CLASS_HPP

#include <libwebsockets.h>

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
		/*
		 * @brief it's just a type holder for void * that will be instantiated in runtime
		 *		  after you provide creation information.
		 */
		lws_context * context;
		/*
		 * @brief  : lws_http_mount struct is kinda self-explanatory
		 * @detail : it mounts entries that we put in, see setupMount inmpl.
		 */
		lws_http_mount mount{};
		/*
		 * @brief : look at definition and read
		 * @copy  : parameters to create context and/ or vhost with
		 *
		 *			this is also used to create vhost ... if LWS_SERVER_OPTION_EXPLICIT_VHOSTS
		 *			is not given, then for backwards compatibility one vhost is created at
		 *			context-creation time using the info from this struct
		 *
		 *			if LWS_SERVER_OPTION_EXPLICIT_VHOST is given, then no vhost are created
		 *			at the same time as context, they are expected to be created afterward.
		 *
		 * @detail: you define interface, protocols, extensions, log_filepath, mounts (if used)
		 *			port, proxy port (if used), ssl cert (if used), tls session (if used),
		 *			gid uid options...
		 */
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
