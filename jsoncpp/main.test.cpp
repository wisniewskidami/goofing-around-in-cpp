#include <criterion/criterion.h>
#define GOOFY_CPP_JSONCPP_IO_IMPL
#include "io.hpp"

Test(read_from_stream, assertion)
{
	Json::Value root;
	goofy::read_from_stream("test.json", root);
	cr_assert_neq(nullptr, &root);
}
