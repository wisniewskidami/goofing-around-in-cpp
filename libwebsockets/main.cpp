
#include <iostream>
#include <ostream>

#include "WebSockets.hpp"
#include "Protocol.hpp"
#include <string>

int main ()
{
	WebSockets webSockets;

	const auto & func = [] ( const std::string & json, const std::string & protocol_name ) -> bool
	{
		std::cout << protocol_name << "\n" << json << std::endl;
		return true;
	};

	const auto & runAfterConnection = [&webSockets](void) -> void
	{
		(void) webSockets["bitstamp"].write(R"({ "event": "bts:subscribe","data": { "channel": "diff_order_book_btcusd"}})");
		(void) webSockets["bitstamp2"].write(R"({ "event": "bts:subscribe","data": { "channel": "diff_order_book_ltcusd"}})");
	};
	webSockets.addProtocol("bitstamp", "ws.bitstamp.net", "/", 443,false,  func);
	webSockets.addProtocol("bitstamp2", "ws.bitstamp.net", "/", 443,false,  func);

	webSockets.connect(runAfterConnection);

	webSockets.run();
}