#include "WebSocket.hpp"
#include "ChatService.hpp"

#include <iostream>

int main()
{
	ChatService serviceOne;
	WebSocket one (serviceOne, 54321 );

	one.join ( );
}