#ifndef GOOFING_AROUND_IN_CPP_WEBSOCKETS_HPP
#define GOOFING_AROUND_IN_CPP_WEBSOCKETS_HPP

#include <string>
#include <functional>
#include <libwebsockets.h>
#include <map>

#include "Protocol.hpp"
#include "Runnable.hpp"

struct WebSocketsException : std::exception
{
	WebSocketsException ( const std::string & message, const int id )
		: message(message)
		, id(id)
	{}

	const char * what ( ) const throw () override
	{
		return this->message.c_str();
	}

	std::string message;
	int id;
};

class WebSockets : public Runnable
{
		lws_context_creation_info M_info;
		lws_context * M_context;
		std::vector < lws_protocols > M_vector_protocols;
		std::map < std::string, Protocol> M_map_protocols;
		int index;

	public:
		WebSockets();
		~WebSockets() override;

		WebSockets (const WebSockets &) = delete;
		void operator = (const WebSockets &) = delete;

		void connect (const std::function < void ( ) > & callback );

		void addProtocol ( std::string name, std::string path, std::string address
			, int port, bool ssl_certificate
			, const std::function < bool ( std::string json, std::string name ) > & callback = {});

		//Protocol & operator [] ( const std::string & name );

		static std::string getId ( );

		void runStep ( );

		void run ( ) override;

		void stop ( ) override;

		bool connected ( ) override;

		Protocol & operator[] ( const std::string & name )
		{
			return this->M_map_protocols[name];
		}

	private:
		void createContext ( );


};



#endif //GOOFING_AROUND_IN_CPP_WEBSOCKETS_HPP
