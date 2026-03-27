#ifndef GOOFING_AROUND_IN_CPP_PROTOCOL_HPP
#define GOOFING_AROUND_IN_CPP_PROTOCOL_HPP

#include <libwebsockets.h>
#include <array>
#include <string>
#include <functional>

class Protocol
{
	public:
		time_t last_update;
	private:
		std::string M_name;
		std::string M_address;
		std::string M_path;
		lws_protocols * M_protocol{};
		lws * M_client_wsi = nullptr;
		lws_context * M_context{};
		lws_client_connect_info M_client_info{};
		bool M_connected;
		std::array<char, 16> M_port_name;
		std::function < bool ( std::string json , std::string name ) > M_callback;
		std::string M_message;
		std::string M_out_message;

	public:
		Protocol ( ) = default;
		Protocol ( std::string name, lws_protocols * protocols, Protocol * user );
		~Protocol ( ) = default;

		void clearWsi();
		lws * getWsi () const;

		void setAddress (
			const std::string& address, const std::string& path,
			int port, const std::function<bool(std::string json, std::string name)> & callback
		);

		void setAddress (
			const std::string& address, const std::string& path, int port,
			bool ssl_connection, const std::function<bool(std::string json, std::string name)> & callback
		);

		void connect ( lws_context * ctx );

		int write ( const std::string & message ) const;
		int write ( const unsigned char * msg, std::size_t message_length ) const;

		void setConnected ( bool c );

		bool isConnected ( ) const;

		bool callback ( ) const;

		void clearMessage ( );

		void messageAppend (const char * in, std::size_t len );
};



#endif //GOOFING_AROUND_IN_CPP_PROTOCOL_HPP
