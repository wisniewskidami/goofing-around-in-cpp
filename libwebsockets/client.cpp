#include <libwebsockets.h>
#include <string>
#include <vector>

namespace goofy
{
	class Convert
	{
		public:
			static std::vector<std::string> argv ( int argc, char ** argv )
			{
				std::vector<std::string> result;

				for (size_t i{}; i < argc; i++)
				{
					result.emplace_back(argv[i]);
				}

				return result;
			}
	};

	class Client
	{
	public:
		static int callback_example ( lws *wsi, lws_callback_reasons reason, void * user, void * in, size_t len );

		Client ( const int argc, char ** argv )
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
					.port = CONTEXT_PORT_NO_LISTEN, // -1
					.gid = static_cast <gid_t> ( -1 ),
					.uid = static_cast <gid_t> ( -1 ),
				})
			, argc ( argc )
			, argv ( Convert::argv ( argc, argv ) )
		{
			this->context = lws_create_context ( &this->info );
		}

	private:
		lws_context_creation_info info;
		lws_context * context;
		const int argc;
		const std::vector<std::string> argv;
	};
}
