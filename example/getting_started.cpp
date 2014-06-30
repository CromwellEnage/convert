// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifdef _MSC_VER
#  pragma warning(disable : 4127)  // conditional expression is constant.
#  pragma warning(disable : 4189)  // local variable is initialized but not referenced.
#  pragma warning(disable : 4100)  // unreferenced formal parameter.
#  pragma warning(disable : 4714)  // marked as __forceinline not inlined.
#endif

#include <boost/detail/lightweight_test.hpp>

static
void
log(char const*)
{
  // Dummy function to demonstrate how problems might be logged.
}

//[getting_started_headers1
#include <boost/convert.hpp>
#include <boost/convert/lexical_cast.hpp>
//]

//[using_header
using std::string;
using boost::lexical_cast;
using boost::convert;
//]
static void getting_started_example1()
{
    //[getting_started_example1
    try
    {
        boost::cnv::lexical_cast cnv;

        int    i1 = lexical_cast<int>("123");
        int    i2 = convert<int>("123", cnv).value();
        string s1 = lexical_cast<string>(123);
        string s2 = convert<string>(123, cnv).value();

        BOOST_TEST(i1 == 123);
        BOOST_TEST(i2 == 123);
        BOOST_TEST(s1 == "123");
        BOOST_TEST(s2 == "123");
    }
    catch (std::exception const& ex)
    {
        // Be aware that the conversion requests above can fail,
        // so always use try'n'catch blocks to handle any exceptions thrown.
        // Ignore this at your peril!
      std::cout << "Exception " << ex.what() << std::endl;

    }
    //] [/getting_started_example1]
}

static void getting_started_example2()
{
    //[getting_started_example2
    // Non-throwing behavior. Returns fallback value (-1) when failed.
    int i = convert<int>("uhm", boost::cnv::lexical_cast()).value_or(-1);

    BOOST_TEST(i == -1);
    //]
}

//[getting_started_headers3
#include <boost/convert/spirit.hpp>
//]
static void getting_started_example3()
{
    //[getting_started_example3
    boost::cnv::spirit cnv;

    int i1 = lexical_cast<int>("123");
    int i2 = convert<int>("123", cnv).value(); // Four times faster than lexical_cast.
    //]
}

//[getting_started_headers4
#include <boost/convert/stream.hpp>
//]
static void getting_started_example4()
{
    //[getting_started_example4
    boost::cnv::cstream cnv;

    try
    {
        int i1 = lexical_cast<int>("   123"); // Does not work.
        BOOST_TEST(!"Never reached");
    }
    catch (...) {}

    int    i2 = convert<int>("   123", cnv(std::skipws)).value(); // Success
    string s1 = lexical_cast<string>(12.34567);
    string s2 = convert<string>(12.34567, cnv(std::fixed)(std::setprecision(3))).value();
    string s3 = convert<string>(12.34567, cnv(std::scientific)(std::setprecision(3))).value();

    BOOST_TEST(i2 == 123);
    BOOST_TEST(s1 == "12.34567");  // No settable precision.
    BOOST_TEST(s2 == "12.346");    // Precision was set to fixed 3. Correctly rounded.
#ifdef _MSC_VER
    BOOST_TEST(s3 == "1.235e+001"); // Precision was set to scientific 3. Correctly rounded.
#else
    BOOST_TEST(s3 == "1.235e+01"); // Precision was set to scientific 3. Correctly rounded.
#endif
    //]
}

static
void
getting_started_example5()
{
    //[getting_started_example5
    boost::cnv::cstream cnv;

    int i1 = lexical_cast<int>("123"); // Throws when conversion fails.
    int i2 = convert<int>("123", cnv).value(); // Throws when conversion fails.
    int i3 = convert<int>("uhm", cnv).value_or(-1); // Returns -1 when conversion fails.

    BOOST_TEST(i1 == 123);
    BOOST_TEST(i2 == 123);
    BOOST_TEST(i3 == -1);
    //]
}

static void getting_started_example6()
{
    std::string const    str1 = "123";
    std::string const    str2 = "456";
    int const default_num_hex = 11;
    int const default_num_dec = 12;
    boost::cnv::cstream   cnv;

    //[getting_started_example6

    int num_hex = convert<int>(str1, cnv(std::hex)).value_or(-1); // Read as hex.
    int num_dec = convert<int>(str2, cnv(std::dec)).value_or(-1); // Read as decimal.

    if (num_hex == -1) log("bad num_hex"), num_hex = default_num_hex;
    if (num_dec == -1) log("bad num_dec"), num_dec = default_num_dec;

    // ... proceed
    //]
    BOOST_TEST(num_hex == 291);
    BOOST_TEST(num_dec == 456);
}

static void getting_started_example7()
{
    std::string const    str1 = "123";
    std::string const    str2 = "456";
    int const default_num_hex = 11;
    int const default_num_dec = 12;
    boost::cnv::cstream   cnv;

    //[getting_started_example7

    int num_hex = convert<int>(str1, cnv(std::hex)).value_or(default_num_hex);
    int num_dec = convert<int>(str2, cnv(std::dec)).value_or(default_num_dec);

    // ... proceed
    //]
    BOOST_TEST(num_hex == 291);
    BOOST_TEST(num_dec == 456);
}

static void getting_started_example8()
{
    std::string const     str = "123";
    int const default_num_dec = 12;
    //[getting_started_example8
    int num_dec = default_num_dec;

    try
    {
        num_dec = lexical_cast<int>(str);
    }
    catch (...)
    {
        log("bad num_dec");
    }
    //]
    BOOST_TEST(num_dec == 123);
}

int
main(int argc, char const* argv[])
{
    getting_started_example1();
    getting_started_example2();
    getting_started_example3();
    getting_started_example4();
    getting_started_example5();
    getting_started_example6();
    getting_started_example7();
    getting_started_example8();
}