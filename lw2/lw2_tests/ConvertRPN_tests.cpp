#include "stdafx.h"

#include "../lw2/main.hpp"

namespace
{

static const auto eps = 10e-3;

}

BOOST_AUTO_TEST_SUITE(ConvertRPN_)

BOOST_AUTO_TEST_CASE(TokenizeString_fn_)
{
	{
		const auto expectedResult = vector<string>{ "a", "b", "+", "c", "*" };
		BOOST_CHECK(TokenizeString("a b + c *") == expectedResult);
	}
}

BOOST_AUTO_TEST_CASE(IsFLoat_fn_)
{
	{
		BOOST_CHECK(IsFloat("42"));
		BOOST_CHECK(IsFloat("-42"));
		BOOST_CHECK(IsFloat("3.141592653589793238"));
		BOOST_CHECK(IsFloat("-3.141592653589793238"));
		BOOST_CHECK(!IsFloat("3,141592653589793238"));
	}
}

BOOST_AUTO_TEST_CASE(ConvertRPN_fn_)
{
	{
		BOOST_REQUIRE_THROW(ConvertRPN(TokenizeString("a b + +")), runtime_error);
		BOOST_REQUIRE_THROW(ConvertRPN(TokenizeString("a b c")), runtime_error);
	}
	{
		BOOST_CHECK_EQUAL(ConvertRPN(TokenizeString("a b + c *")), "(a+b)*c");
		BOOST_CHECK_EQUAL(ConvertRPN(TokenizeString("a b x + * d - 2 /")), "(a*(b+x)-d)/2");
	}
	{
		BOOST_CHECK_EQUAL(ConvertRPN(TokenizeString("3 c 2 d + ^ e f - ^ +")), "3+c^(2+d)^(e-f)");
		BOOST_CHECK_EQUAL(ConvertRPN(TokenizeString("2 3 ^ 4 ^")), "2^3^4");
	}
	{
		BOOST_CHECK_EQUAL(ConvertRPN(TokenizeString("c a b + -")), "c-(a+b)");
		BOOST_CHECK_EQUAL(ConvertRPN(TokenizeString("a b + c *")), "(a+b)*c");
	}
	{
		BOOST_CHECK_EQUAL(ConvertRPN(TokenizeString("a b + exp")), "exp(a+b)");
		BOOST_CHECK_EQUAL(ConvertRPN(TokenizeString("0 sin")), "sin0");
		BOOST_CHECK_EQUAL(ConvertRPN(TokenizeString("a b * d - cos sin")), "sincos(a*b-d)");
		BOOST_CHECK_EQUAL(ConvertRPN(TokenizeString("a sin b ^ c d * +")), "sina^b+c*d");
	}
	{
		BOOST_CHECK_EQUAL(ConvertRPN(TokenizeString("a b c ^ ^")), "a^b^c");
		BOOST_CHECK_EQUAL(ConvertRPN(TokenizeString("a b ^ c ^")), "(a^b)^c");
	}
}

BOOST_AUTO_TEST_CASE(CalculateRPN_fn)
{
	{
		BOOST_REQUIRE_CLOSE_FRACTION(CalculateRPN(TokenizeString("2 3 +")), 5.f, eps);
	}
	{
		BOOST_REQUIRE_CLOSE_FRACTION(CalculateRPN(TokenizeString("2 3 * 7 + 0 exp +")), 14.f, eps);
	}
	{
		BOOST_REQUIRE_CLOSE_FRACTION(CalculateRPN(TokenizeString("1 2 + 3 *")), 9.f, eps);
	}
	{
		BOOST_REQUIRE_CLOSE_FRACTION(CalculateRPN(TokenizeString("5.497625 sin 2 ^ 5.497625 cos 2 ^ +")), 1.f, eps);
	}
	{
		BOOST_REQUIRE_CLOSE_FRACTION(CalculateRPN(TokenizeString("5.497625 sin 2 ^ 5.497625 cos 2 ^ + 2 *")), 2.f, eps);
	}
	{
		BOOST_REQUIRE_CLOSE_FRACTION(CalculateRPN(TokenizeString("5.497625 sin 2 ^ 5.497625 cos 2 ^ + 2 * exp")), 7.389f, eps);
	}
}

BOOST_AUTO_TEST_SUITE_END()
