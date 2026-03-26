#include <criterion/criterion.h>
#define GOOFY_CPP_JSONCPP_IO_IMPL
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
	 "{  \"try\" : \"parse me\", \"array\" : [ 42, 21, 10, 5, 84, 168, 126, ], \"does-it-allow-trailing-comma\": { \"maybe\": \"it does.\", \"but-it-may\": \"don't\" } }";

	goofy::read_string(json_in_string, root);

	cr_assert_neq(Json::Value{}, root);
}