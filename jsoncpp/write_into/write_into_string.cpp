#include "write_into_string.hpp"
#include <iostream>

auto
string_write (std::string & into) -> void
{
	Json::Value root;
	Json::Value data;
	std::string json_file;

	root["action"] = "run";
	data["number"] = 42;
	root["data"] = data;


	{	// Old Way!
		std::cout << "\nOld Way:\n";
		Json::FastWriter writer;
		json_file = writer.write(root);
		std::cout << json_file << '\n'; 
	}

	json_file.clear();

	{	// New Way!
		std::cout << "\nNew Way:\n";
		Json::StreamWriterBuilder builder;
		json_file = Json::writeString(builder, root);
		std::cout << json_file << '\n';
	}
	
	into = json_file;
};
