#include "read_from_stream.hpp"
#include <iostream>
#include <fstream>

auto
read_stream (const std::string & filePath, Json::Value & into) -> bool
{
	std::ifstream ifs;
	JSONCPP_STRING errs;
	Json::CharReaderBuilder builder;

	ifs.open(filePath);
	builder["collectComments"] = true;
	if ( !Json::parseFromStream ( builder, ifs, &into, &errs ) )
	{
		std::cout << errs << '\n';
		return (false);
	}

	std::cout << into << "\n";
	return (true);
}
