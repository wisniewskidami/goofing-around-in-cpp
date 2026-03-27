#include "ChatService.hpp"

ChatService::ChatService ( )
	: worker  ( &ChatService::run, this )
	, running ( true )
{}

ChatService::~ChatService ( )
{
	this->running = false;
	this->condition_variable.notify_one();
	this->worker.join();
}

void ChatService::newClient( const uint32_t id )
{
	std::lock_guard lock( this->client_lock );
	this->clients.insert ( id );
}

void ChatService::message ( uint32_t id, const std::string & msg )
{
	std::lock_guard lock ( this->queue_lock );
	this->queue.emplace_back ( msg );
	this->condition_variable.notify_one();
}

void ChatService::closeClient ( const uint32_t id )
{
	std::lock_guard lock ( this->queue_lock );
	this->clients.erase ( id );
}

void ChatService::run()
{
	std::mutex				signal;
	std::unique_lock		lock0 ( signal );

	while ( this->running )
	{
		this->condition_variable.wait ( lock0 , [this]
		{
			return this->queue.empty() && this->running;
		} );

		if ( !this->running ) return;

		std::deque < std::string > toSend;
		{
			std::lock_guard lock1 ( this->queue_lock );
			std::swap ( toSend, this->queue );
		}

		{
			std::lock_guard lock2 (this->client_lock );

			for ( const auto & i : this->clients )
			{
				for ( const auto & msg : toSend )
				{
					std::cout << "Telling: " << msg << " to " << i << std::endl;

					this->send ( i , msg );
				}
			}
		}

	}
}


