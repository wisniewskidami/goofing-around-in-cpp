#include "write_into_stream.hpp"
#include <fstream>

auto
stream_write (std::ofstream & into) -> void
{
	Json::Value root;

	root["last-name"] = "Iwakura";
	root["first-name"] = "Lain";
	
	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = "   ";

	const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());

	writer->write(root, &into);
};
