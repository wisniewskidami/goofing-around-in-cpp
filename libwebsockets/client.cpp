#include <libwebsockets.h>

namespace goofy
{
	class Client
	{
	public:
		static int callback_example(lws *wsi, lws_callback_reasons reason, void * user, void * in, size_t len);

		Client(int argc, char ** argv) 
			: info((lws_context_creation_info) 
				{ 
					.protocols = (lws_protocols[])
					{
						{
								.name = "example-protocol",
								.callback = callback_example,
								.per_session_data_size = 0,
								.rx_buffer_size = 0,
								.id = 0,
								.user = nullptr,
								.tx_packet_size = 0
						},
					},
					.port = CONTEXT_PORT_NO_LISTEN,
					.gid = static_cast<unsigned int>(-1),
					.uid = static_cast<unsigned int>(-1),
				})
			, argc(argc)
			, argv(argv)
		{
			this->context = lws_create_context(&this->info);
		};

	private:
		lws_context_creation_info info;
		lws_context * context;
		int argc;
		char ** argv;
	};
};
