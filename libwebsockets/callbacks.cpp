#include "callbacks.h"
#include "Protocol.hpp"
#include "WebSockets.hpp"

int callback_dumb_increment ( struct lws * wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len )
{
	int n;
	Protocol * p = lws_get_protocol(wsi) ? (Protocol *)lws_get_protocol(wsi) : nullptr;

	switch ( reason )
	{
		case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
			lwsl_err("CLIENT_CONNECTION_ERROR: %s\n", in ? static_cast<char *>(in) : "(null)");
			p = (Protocol *)lws_get_protocol(wsi);
			break;
		case LWS_CALLBACK_CLIENT_ESTABLISHED:
			lwsl_user("LWS_CALLBACK_CLIENT_ESTABLISHED\n", __func__);
			p = (Protocol *)lws_get_protocol(wsi);
			p->setConnected(true);
			break;
		case LWS_CALLBACK_CLIENT_RECEIVE_PONG:
			printf ("LWS_CALLBACK_CLIENT_RECEIVE_PONG\n");
			break;
		case LWS_CALLBACK_CLIENT_WRITEABLE:
			/*
				{
					WebSockets * web_sockets = static_cast<WebSockets *>(lws_get_protocol(wsi)->user);
					std::string msg = web_sockets->();
					char c[255];
					c[0] = 0;
					strncpy (c, msg.c_str(), 255);
					c[msg.size()] = 0;
					n = lws_write(wsi, reinterpret_cast<unsigned char *>(c), strlen(c), LWS_WRITE_TEXT);
				}
			*/
			break;
		case LWS_CALLBACK_CLIENT_RECEIVE:
			time(&p->last_update);

			if (lws_is_first_fragment(wsi))
				p->clearMessage();

			p->messageAppend(static_cast<char *>(in), len);

			if (lws_is_final_fragment(wsi))
			{
				bool res = p->callback();
				if (res == false)
					return -1;
			}
			break;
		case LWS_CALLBACK_CLIENT_CLOSED:
			p->clearWsi();
			break;
		default:
			break;
	}

	return lws_callback_http_dummy (wsi, reason, user, in, len);
}
