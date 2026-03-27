#include "Protocol.hpp"

#include <cstring>
#include <utility>
#include "callbacks.h"


Protocol::Protocol ( std::string name, lws_protocols * protocol, Protocol * user )
	:
	last_update(0),
	M_name(std::move(name)),
	M_connected(false),
	M_port_name({})
{
	*protocol = {
		strdup(reinterpret_cast<const char *>(&this->M_name)),
		callback_dumb_increment,
		0,
		0,
		0,
		user,
		0
	};

	this->M_protocol = protocol;
}

void Protocol::clearWsi ( )
{
	this->M_client_wsi = nullptr;
}

lws * Protocol::getWsi ( ) const
{
	return this->M_client_wsi;
}

void Protocol::setAddress (
	const std::string& address, const std::string& path, const int port,
	const std::function<bool (std::string json, std::string name)> & callback
)
{
	this->setAddress(address, path , port, true, callback);
}

void Protocol::setAddress (
	const std::string& address, const std::string& path,  int port, bool ssl_connection,
	const std::function<bool(std::string json, std::string name)> & callback
)
{
	this->M_address = address;
	this->M_path = path;
	std::memset(&this->M_client_info, 0, sizeof(this->M_client_info));
	this->M_client_info.port = port;
	this->M_client_info.address = strdup(address.c_str());
	this->M_client_info.path = strdup(path.c_str());
	this->M_client_info.host = this->M_client_info.address;
	this->M_client_info.origin = this->M_client_info.address;
	this->M_client_info.ssl_connection = ssl_connection;
	this->M_client_info.protocol = this->M_port_name.data();
	this->M_client_info.pwsi = &this->M_client_wsi;
	this->M_callback = callback;
}

void Protocol::connect ( lws_context * ctx )
{
	this->M_client_info.context = ctx;
	this->M_client_wsi = lws_client_connect_via_info(&this->M_client_info);
}

int Protocol::write(const std::string & message) const
{
	return lws_write(this->M_client_wsi,
		reinterpret_cast<unsigned char *>(const_cast<char *>(message.data())),
		message.size(),
		LWS_WRITE_TEXT);
}

int Protocol::write(const unsigned char * msg, const std::size_t message_length) const
{
	const std::string str(reinterpret_cast<const char*>(msg), message_length);
	return this->write(str);
}

void Protocol::setConnected(const bool c)
{
	this->M_connected = c;
}

bool Protocol::isConnected () const
{
	return this->M_connected;
}

bool Protocol::callback() const
{
	if (this->M_callback)
		return this->M_callback(this->M_message, this->M_name);
	return false;
}

void Protocol::clearMessage()
{
	this->M_message.clear();
}

void Protocol::messageAppend(const char * in, const std::size_t len)
{
	this->M_message.append(in, len);
}