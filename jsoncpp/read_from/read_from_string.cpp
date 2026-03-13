#include "read_from_string.hpp"
#include <iostream>

auto
read_string (const std::string & from, Json::Value & into) -> bool
{
	const auto raw_json_length = static_cast<std::size_t>(from.length());

	JSONCPP_STRING err;
	
	{
		std::cout << "\nOld Way\n";
		Json::Reader reader;
		reader.parse(from, into);
	};
	
	std::cout << into << '\n';

	into.clear();

	{
		std::cout << "\nNew Way\n";
		Json::CharReaderBuilder builder;
		const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());

		if (!reader->parse(from.c_str(), from.c_str() + raw_json_length, &into, &err))
		{
			std::cout << err << '\n';
			return (false);
		};
	};

	std::cout << into << '\n';
	return (true);
};
