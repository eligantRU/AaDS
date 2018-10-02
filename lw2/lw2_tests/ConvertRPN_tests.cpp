#include "stdafx.h"

#include "../lw2/main.hpp"

BOOST_AUTO_TEST_SUITE(ConvertRPN_)

BOOST_AUTO_TEST_CASE(TokenizeString_)
{
	{
		const auto expectedResult = vector<string>{ "a", "b", "+", "c", "*" };
		BOOST_CHECK(TokenizeString("a b + c *") == expectedResult);
	}
}

BOOST_AUTO_TEST_CASE(ConvertRPN_fn_)
{
	{
		BOOST_REQUIRE_THROW(ConvertRPN(TokenizeString("a b + +")), runtime_error);
		BOOST_REQUIRE_THROW(ConvertRPN(TokenizeString("a b c")), runtime_error);
	}
	{
		BOOST_CHECK(ConvertRPN(TokenizeString("a b + c *")) == "(a+b)*c");
		BOOST_CHECK(ConvertRPN(TokenizeString("a b x + * d - 2 /")) == "(a*(b+x)-d)/2");
	}
	{
		BOOST_CHECK(ConvertRPN(TokenizeString("3 c 2 d + ^ e f - ^ +")) == "3+c^(2+d)^(e-f)");
		BOOST_CHECK(ConvertRPN(TokenizeString("2 3 ^ 4 ^")) == "2^3^4");
	}
	{
		BOOST_CHECK(ConvertRPN(TokenizeString("c a b + -")) == "c-(a+b)");
		BOOST_CHECK(ConvertRPN(TokenizeString("a b + c *")) == "(a+b)*c");
	}
	{
		BOOST_CHECK(ConvertRPN(TokenizeString("a b + exp")) == "exp(a+b)");
		BOOST_CHECK(ConvertRPN(TokenizeString("0 sin")) == "sin0");
		BOOST_CHECK(ConvertRPN(TokenizeString("a b * d - cos sin")) == "sincos(a*b-d)");
	}
}

BOOST_AUTO_TEST_SUITE_END()
