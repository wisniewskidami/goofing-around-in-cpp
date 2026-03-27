#include <criterion/criterion.h>
#define GOOFY_CPP_JSONCPP_IO_IMPL
#include <filesystem>

#include "io.hpp"

Test(read_from_stream, assertion)
{
	Json::Value root;
	goofy::read_from_stream("test.json", root);

	cr_assert_neq(Json::Value{}, root);
}

Test(read_string, assert_dominance)
{
	Json::Value root;
	const std::string json_in_string =
	 "{\n"
			"\"try\" : \"parse me\", \n"
			"\"array\" : [ 42, 21, 10, 5, 84, 168, 126, ], \n"
			"\"does-it-allow-trailing-comma\": \n"
			"{\n"
						"\"maybe\": \"it does.\",\n"
						"\"but-it-may\": \"don't\"\n"
			"}\n"
	 "}\n";

	goofy::read_string(json_in_string, root);

	cr_assert_neq(Json::Value{}, root);
}

Test(stream_write, dominamce_almost_asserted)
{
	std::filesystem::remove("omg.json");

	std::ofstream stream;

	stream.open("omg.json", std::ios::out);

	goofy::stream_write(stream);

	cr_assert(stream.good());
}

Test(string_write, dominance_asserted)
{
	std::string value;

	goofy::string_write(value);

	cr_assert_not(value.empty());
}