#include "WebSocket.hpp"

#include <iostream>
#include <cstring>

#ifndef REQUEST_ON_THREAD_2
# define CANCEL_ON_THREAD_2
#endif

namespace
{
	constexpr uint32_t unique = 0;
	std::map<lws_callback_reasons, std::string> readableEnums = {
        {LWS_CALLBACK_ESTABLISHED," LWS_CALLBACK_ESTABLISHED"},
        {LWS_CALLBACK_CLIENT_CONNECTION_ERROR," LWS_CALLBACK_CLIENT_CONNECTION_ERROR"},
        {LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH," LWS_CALLBACK_CLIENT_FILTER_PRE_ESTABLISH"},
        {LWS_CALLBACK_CLIENT_ESTABLISHED," LWS_CALLBACK_CLIENT_ESTABLISHED"},
        {LWS_CALLBACK_CLOSED," LWS_CALLBACK_CLOSED"},
        {LWS_CALLBACK_CLOSED_HTTP," LWS_CALLBACK_CLOSED_HTTP"},
        {LWS_CALLBACK_RECEIVE," LWS_CALLBACK_RECEIVE"},
        {LWS_CALLBACK_CLIENT_RECEIVE," LWS_CALLBACK_CLIENT_RECEIVE"},
        {LWS_CALLBACK_CLIENT_RECEIVE_PONG," LWS_CALLBACK_CLIENT_RECEIVE_PONG"},
        {LWS_CALLBACK_CLIENT_WRITEABLE," LWS_CALLBACK_CLIENT_WRITEABLE"},
        {LWS_CALLBACK_SERVER_WRITEABLE," LWS_CALLBACK_SERVER_WRITEABLE"},
        {LWS_CALLBACK_HTTP," LWS_CALLBACK_HTTP"},
        {LWS_CALLBACK_HTTP_FILE_COMPLETION," LWS_CALLBACK_HTTP_FILE_COMPLETION"},
        {LWS_CALLBACK_HTTP_WRITEABLE," LWS_CALLBACK_HTTP_WRITEABLE"},
        {LWS_CALLBACK_FILTER_NETWORK_CONNECTION," LWS_CALLBACK_FILTER_NETWORK_CONNECTION"},
        {LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION," LWS_CALLBACK_FILTER_PROTOCOL_CONNECTION"},
        {LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS," LWS_CALLBACK_OPENSSL_LOAD_EXTRA_CLIENT_VERIFY_CERTS"},
        {LWS_CALLBACK_OPENSSL_LOAD_EXTRA_SERVER_VERIFY_CERTS," LWS_CALLBACK_OPENSSL_LOAD_EXTRA_SERVER_VERIFY_CERTS"},
        {LWS_CALLBACK_OPENSSL_PERFORM_CLIENT_CERT_VERIFICATION," LWS_CALLBACK_OPENSSL_PERFORM_CLIENT_CERT_VERIFICATION"},
        {LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER," LWS_CALLBACK_CLIENT_APPEND_HANDSHAKE_HEADER"},
        {LWS_CALLBACK_CONFIRM_EXTENSION_OKAY," LWS_CALLBACK_CONFIRM_EXTENSION_OKAY"},
        {LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED," LWS_CALLBACK_CLIENT_CONFIRM_EXTENSION_SUPPORTED"},
        {LWS_CALLBACK_PROTOCOL_INIT," LWS_CALLBACK_PROTOCOL_INIT"},
        {LWS_CALLBACK_PROTOCOL_DESTROY," LWS_CALLBACK_PROTOCOL_DESTROY"},
        {LWS_CALLBACK_WSI_CREATE," LWS_CALLBACK_WSI_CREATE"},
        {LWS_CALLBACK_WSI_DESTROY," LWS_CALLBACK_WSI_DESTROY"},
        {LWS_CALLBACK_GET_THREAD_ID," LWS_CALLBACK_GET_THREAD_ID"},
        {LWS_CALLBACK_ADD_POLL_FD," LWS_CALLBACK_ADD_POLL_FD"},
        {LWS_CALLBACK_DEL_POLL_FD," LWS_CALLBACK_DEL_POLL_FD"},
        {LWS_CALLBACK_CHANGE_MODE_POLL_FD," LWS_CALLBACK_CHANGE_MODE_POLL_FD"},
        {LWS_CALLBACK_LOCK_POLL," LWS_CALLBACK_LOCK_POLL"},
        {LWS_CALLBACK_UNLOCK_POLL," LWS_CALLBACK_UNLOCK_POLL"}
    };

	int callback_http ( lws * wsi,
		lws_callback_reasons reason, void * data, void * in, size_t len )
	{
		std::cout << readableEnums[reason] << " callback_http" << std::endl;

		if (reason == LWS_CALLBACK_GET_THREAD_ID)
		{
			return static_cast<int>(std::hash<std::thread::id>()(std::this_thread::get_id()));
		}
		return 0;
	}

	int callback_web ( lws *wsi,
		lws_callback_reasons reason, void* user, void * in, size_t len )
	{
		std::cout << readableEnums[reason] << " callback_web" << std::endl;

		int & id = *static_cast < int * > ( user );

		lws_context * context = lws_get_context ( wsi );

		if ( reason == LWS_CALLBACK_ESTABLISHED )
		{
			id = unique+1;
			static_cast < WebSocket * > ( lws_context_user (context) )->newClient ( id, wsi );
		}

		if ( reason == LWS_CALLBACK_RECEIVE )
		{
			std::string message { static_cast<char *>(in), len };
			static_cast < WebSocket * > (lws_context_user (context) )->message ( id, message );
		}

		if ( reason == LWS_CALLBACK_SERVER_WRITEABLE )
		{
			std::queue < std::string > toSend = static_cast < WebSocket * > ( lws_context_user ( context ) )
																				->fetch_message_queue ( id );

			std::cout << "Got callback" << std::endl;

			while ( !toSend.empty() )
			{
				std::string & msg = toSend.front();

				auto * buf = static_cast< unsigned char * >
					( std::malloc (LWS_SEND_BUFFER_PRE_PADDING + msg.size() + LWS_SEND_BUFFER_POST_PADDING ) );

				std::memcpy ( buf + LWS_SEND_BUFFER_PRE_PADDING, msg.c_str(), msg.size() );
				lws_write ( wsi, buf + LWS_SEND_BUFFER_PRE_PADDING, msg.size(), LWS_WRITE_TEXT );
				std::free ( buf );

				toSend.pop();
			}

		}

		if ( reason == LWS_CALLBACK_CLOSED )
		{
			static_cast < WebSocket * > ( lws_context_user (context) )->closeClient ( id );
		}

		return 0;
	}

	constexpr lws_protocols protocols[] =
	{
		{
			 "http-only",
			 callback_http,
			sizeof(int)
		},
		{
			"web-protocol",
			 callback_web,
			sizeof(int)
		}
	};

	lws_context_creation_info make_info ( const uint16_t port, WebSocket * socket )
	{
		return {
			.protocols = protocols,
			.port = port,
			.gid = static_cast < gid_t > ( -1 ),
			.uid = static_cast < uid_t > ( -1 ),
			.options = 0,
			.user = socket,
		};
	}
}

void WebSocket::Delegate::send ( const uint32_t id, const std::string & msg ) const
{
	this->socket->send ( id , msg );
}

void WebSocket::Delegate::setSocket ( WebSocket * socket )
{
	this->socket = socket;
}

bool WebSocket::init ()
{
	this->delegate.setSocket (this);
	return true;
}

WebSocket::WebSocket ( Delegate & delegate, const uint16_t port )
	: delegate			( delegate )
	, port				( port )
	, socket_info		( make_info (port, this) )
	, context			( lws_create_context ( &this->socket_info ) )
	, shouldService		( this->init ( ) )
	, service_thread	( &WebSocket::service, this )
{}

WebSocket::~WebSocket()
{
	this->shouldService = false;
	lws_cancel_service ( this->context );
	this->service_thread.join ( );
	lws_context_destroy ( this->context );
}

void WebSocket::join ( )
{
	this->service_thread.join ( );
}

void WebSocket::newClient ( const uint32_t id, lws * wsi )
{
	this->clients[id] = wsi;
	this->delegate.newClient ( id );
}

void WebSocket::message ( const uint32_t id, const std::string & message ) const
{
	this->delegate.message ( id, message );
}

void WebSocket::closeClient( const uint32_t id )
{
	this->delegate.closeClient(id);
	{
		std::lock_guard lock(this->queue_lock);
		this->message_queue.erase ( id );
	}
	this->clients.erase ( id );
}

void WebSocket::send ( const uint32_t id, const std::string & msg )
{
	{
		std::lock_guard lock ( this->queue_lock );
		this->message_queue[id].push ( msg );
	}
#ifdef REQUEST_ON_THREAD_2
	lws_callback_on_writable ( this->context, this->clients[id] );
#endif
#ifdef CANCEL_ON_THREAD_2
	lws_cancel_service ( this->context );
#endif
}

std::queue<std::string> WebSocket::fetch_message_queue(uint32_t id)
{
	std::queue < std::string > queue;
	{
		std::lock_guard lock ( this->queue_lock );
		std::swap ( queue, this->message_queue [id] );
	}
	return queue;
}

void WebSocket::service()
{
	while ( this->shouldService )
	{
#ifdef CANCEL_ON_THREAD_2
		{
			std::lock_guard lock ( this->queue_lock );
			for ( auto socket : this->message_queue )
			{
				if ( !socket.second.empty ( ) )
				{
					lws_callback_on_writable ( this->clients[socket.first] );
				}
			}
		}
#endif
		lws_service (this->context, std::numeric_limits<int>::max());
	}
}
