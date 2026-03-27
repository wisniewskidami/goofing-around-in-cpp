#ifndef GOOFING_AROUND_IN_CPP_WEBSOCKET_HPP
#define GOOFING_AROUND_IN_CPP_WEBSOCKET_HPP

#include <libwebsockets.h>

#include <thread>
#include <mutex>
#include <map>
#include <queue>

class WebSocket
{
	public:
		class Delegate
		{
				WebSocket * socket;
			public:
				virtual void newClient ( uint32_t id ) = 0;
				virtual void message ( uint32_t id, const std::string & msg ) = 0;
				virtual void closeClient ( uint32_t id) = 0;
				void send ( unsigned int id, const std::string & msg ) const;
				void setSocket (WebSocket * socket);
		};

	private:
		std::mutex									queue_lock;
		std::map<uint32_t, std::queue<std::string>> message_queue;
		std::map<uint32_t, lws *>					clients;
		Delegate &									delegate;
		uint16_t									port;
		lws_context_creation_info					socket_info;
		lws_context *								context;
		bool										shouldService;
		std::thread									service_thread;
	public:
		WebSocket ( Delegate & delegate, uint16_t port );
		~WebSocket ( );

		void join ( );

		void newClient ( uint32_t id, lws * wsi );

		void message ( uint32_t id, const std::string & message ) const;

		void closeClient ( uint32_t id );

		void send ( uint32_t id, const std::string & msg );

		std::queue<std::string> fetch_message_queue ( uint32_t id );
	private:
		void service ( );
		bool init ( );
};

#endif //GOOFING_AROUND_IN_CPP_WEBSOCKET_HPP
