#include "clang_index_parser.hpp"

#include <boost/config/warning_disable.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>
#include <iostream>

namespace spirit = boost::spirit;
namespace ascii = boost::spirit::ascii;
namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

using std::tr1::unordered_set;
using std::tr1::unordered_map;

template <typename Iterator>
struct Grammar : qi::grammar<Iterator, Index ()>
{
    Grammar() : Grammar::base_type(start)
    {
        using qi::_val;
        using qi::_1;
        using qi::char_;
        using ascii::space;

        word    = + (char_ - space)[_val += _1];
        wordSet =  (+word[phoenix::insert(_val, _1)] % ' ');
        line    %= word >> '\t' >> wordSet >> '\n';
        start   = * line[phoenix::insert(_val, _1)];
    }

    qi::rule<Iterator, std::string ()> word;
    qi::rule<Iterator, unordered_set<std::string> ()> wordSet;
    qi::rule<Iterator, std::pair< std::string, unordered_set<std::string> > ()> line;
    qi::rule<Iterator, Index ()> start;
};

Index parseIndex(std::istream& in) {
    in.unsetf(std::ios::skipws);
    spirit::istream_iterator begin(in);
    spirit::istream_iterator end;

    Grammar<spirit::istream_iterator> grammar;

    Index parsedResult;

    bool ok = qi::parse(begin, end, grammar, parsedResult);

    if (!ok) {
        std::cerr << "Parse failed" << std::endl;
    } else if (begin != end) {
        std::cerr << "Trailing characters:" << std::endl;
        while (!in.eof()) {
            char c;
            in >> c;
            std::cerr << c;
        }
    }
    return parsedResult;
}
