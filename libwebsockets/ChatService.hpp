#ifndef GOOFING_AROUND_IN_CPP_CHATSERVICE_HPP
#define GOOFING_AROUND_IN_CPP_CHATSERVICE_HPP
#include "WebSocket.hpp"
#include "WebSocket.hpp"

#include <condition_variable>
#include <mutex>
#include <deque>
#include <thread>
#include <set>
#include <iostream>

class ChatService : public WebSocket::Delegate
{
		std::mutex					client_lock;
		std::condition_variable		condition_variable;
		std::set < uint32_t >		clients;
		std::mutex					queue_lock;
		std::deque < std::string >	queue;
		std::thread					worker;
		bool						running;

	public:
		ChatService ( );
		~ChatService ( );

		void run ( );

		void newClient ( uint32_t id ) override;
		void message ( uint32_t id, const std::string & msg ) override;
		void closeClient ( uint32_t id ) override;
};

#endif //GOOFING_AROUND_IN_CPP_CHATSERVICE_HPP
