#ifndef GOOFY_CPP_JSONCPP_IO
# define GOOFY_CPP_JSONCPP_IO

#include <json/json.h>

namespace goofy
{
	void stream_write(std::ofstream &);
	void string_write(std::string &);

	bool read_from_stream (const std::string &, const Json::Value &);
	bool read_string (const std::string &, const Json::Value &); 
}

#ifdef GOOFY_CPP_JSONCPP_IO_IMPL

#include <iostream>
#include <fstream>

namespace goofy 
{
	/**
	 * @brief		reading json file from stream, allowing to also collect comments or not
	 * @return {bool}	if boolean value is false it means it caught an internal error while parsing
	 *			otherwise it will return true.
	 **/
	inline auto
	read_from_stream (const std::string & filePath, Json::Value & into) -> bool
	{
		std::ifstream ifs;
		JSONCPP_STRING errs;
		Json::CharReaderBuilder builder;

		ifs.open(filePath);
		builder["collectComments"] = true;
		if ( !Json::parseFromStream ( builder, ifs, &into, &errs ) )
		{
			std::cout << errs << '\n';
			return false;
		}

		std::cout << into << "\n";
		return true;
	};
	
	/**
	 * @brief		reading JSON data from a string (which inherit is the same as from a file, but doesn't use fd)
	 * @return {bool}	if parsing failed it can get caught in the New Way, in the old way it just sneakily failed
	 *			returns true, when parsing successfully went through
	 **/
	inline auto
	read_string (const std::string & from, Json::Value & into) -> bool
	{
		const auto raw_json_length = from.length();

		JSONCPP_STRING err;
		
		{
			std::cout << "\nOld Way\n";
			Json::Reader reader;
			reader.parse(from, into);
		}
		
		std::cout << into << '\n';

		into.clear();

		{
			std::cout << "\nNew Way\n";
			const Json::CharReaderBuilder builder;

			if (const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
				!reader->parse(from.c_str(), from.c_str() + raw_json_length, &into, &err))
			{
				std::cout << err << '\n';
				return false;
			}
		}

		std::cout << into << '\n';
		return true;
	}
	
	/**
	 * @brief this is basic reading to a std::ofstream buff
	 **/
	inline auto
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
	}

	/**
	 * @brief this is just a basic reading into a string.
	 **/
	inline auto
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
	}
}

#endif
#endif
