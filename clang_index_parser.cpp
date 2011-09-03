#include "clang_index_parser.hpp"

#include <boost/config/warning_disable.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>
#include <boost/spirit/include/phoenix_fusion.hpp>
#include <boost/spirit/include/phoenix_stl.hpp>
#include <boost/spirit/include/qi.hpp>
#include <cassert>
#include <iostream>

namespace spirit = boost::spirit;
namespace ascii = boost::spirit::ascii;
namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

using std::tr1::unordered_set;
using std::tr1::unordered_map;

template <typename Iterator>
struct Grammar : qi::grammar<Iterator, IndexItem ()>
{
    Grammar() : Grammar::base_type(line)
    {
        using qi::_val;
        using qi::_1;
        using qi::char_;
        using qi::eps;
        using ascii::space;

        word    = eps[_val = ""]            >> (+ (char_ - space)[_val += _1]);
        wordSet = eps[phoenix::clear(_val)] >> (+word[phoenix::insert(_val, _1)] % ' ');
        line    %= word >> '\t' >> wordSet >> '\n';
    }

    qi::rule<Iterator, std::string ()> word;
    qi::rule<Iterator, unordered_set<std::string> ()> wordSet;
    qi::rule<Iterator, IndexItem ()> line;
};

Grammar<spirit::istream_iterator> grammar;

IndexItemIterator::IndexItemIterator() : i(-1) {}

IndexItemIterator::IndexItemIterator(std::istream& in) :
    value(boost::make_shared<IndexItem>()),
    i(0),
    in(&in),
    inputBegin(in)
{
    in.unsetf(std::ios::skipws);
}

const IndexItem& IndexItemIterator::dereference() const {
    return *value;
}

bool IndexItemIterator::equal(const IndexItemIterator& other) const {
    return i == other.i;
}

void IndexItemIterator::increment() {
    assert(i != -1);
    bool ok = qi::parse(inputBegin, inputEnd, grammar, *value);
    if (ok) {
        i++;
    } else {
        i = -1; // reached the end

        if (inputBegin != inputEnd) {
            std::cerr << "Trailing characters:" << std::endl;
            while (!in->eof()) {
                char c;
                *in >> c;
                std::cerr << c;
            }
        }
    }
}

boost::iterator_range<IndexItemIterator> parseIndex(std::istream& in) {
    return boost::make_iterator_range(IndexItemIterator(in), IndexItemIterator());
}
