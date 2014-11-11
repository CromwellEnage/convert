// Copyright (c) 2009-2014 Vladimir Batov.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. See http://www.boost.org/LICENSE_1_0.txt.

#ifndef BOOST_CONVERT_SPIRIT_BASED_CONVERTER_HPP
#define BOOST_CONVERT_SPIRIT_BASED_CONVERTER_HPP

#include <boost/convert/detail/base.hpp>
#include <boost/convert/detail/forward.hpp>
#include <boost/spirit/include/qi_parse.hpp>
#include <boost/spirit/include/qi_numeric.hpp>
#include <boost/spirit/include/qi_string.hpp>
#include <boost/range/as_literal.hpp>

#include <boost/convert/detail/coerce/detail/karma.hpp>

namespace boost { namespace cnv
{
    struct spirit;

    namespace detail
    {
        template <typename Source>
        struct generator : boost::spirit::traits::create_generator<Source>::type {};

        template <> struct generator<      float> : coerce::detail::real_generator<float> {};
        template <> struct generator<     double> : coerce::detail::real_generator<double> {};
        template <> struct generator<long double> : coerce::detail::real_generator<long double> {};
    }
}}

struct boost::cnv::spirit : public boost::cnv::detail::cnvbase<boost::cnv::spirit>
{
    typedef boost::cnv::spirit                     this_type;
    typedef boost::cnv::detail::cnvbase<this_type> base_type;

    using base_type::operator();

    template<typename string_type, typename out_type>
    void
    str_to(cnv::range<string_type> range, optional<out_type>& result_out) const
    {
        typedef typename cnv::range<string_type>::iterator                  iterator;
        typedef typename boost::spirit::traits::create_parser<out_type>::type parser;

        iterator    beg = range.begin();
        iterator    end = range.end();
        out_type result;

        if (boost::spirit::qi::parse(beg, end, parser(), result))
            if (beg == end) // ensure the whole string has been parsed
                result_out = result;
    }
    template<typename in_type, typename char_type>
    cnv::range<char*>
    to_str(in_type value_in, char_type* beg) const
    {
        typedef cnv::detail::generator<in_type> generator;

        char_type* end = beg;
        bool      good = boost::spirit::karma::generate(end, generator(), value_in);
        
        return cnv::range<char*>(beg, good ? end : beg);
    }
};

#endif // BOOST_CONVERT_SPIRIT_BASED_CONVERTER_HPP
