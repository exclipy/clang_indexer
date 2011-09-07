#include "clang_index_parser.hpp"

#include <boost/config/warning_disable.hpp>
#include <boost/fusion/adapted/std_pair.hpp>
#include <boost/fusion/include/std_pair.hpp>
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

template <typename Iterator>
struct Grammar : qi::grammar<Iterator, IndexItem ()>
{
    Grammar() : Grammar::base_type(line)
    {
        using qi::_val;
        using qi::_1;
        using qi::char_;
        using qi::lit;
        using qi::eps;
        using ascii::space;

        word    = eps[_val = ""]            >> (+ (char_ - (lit('\t') | lit('\n')))[_val += _1]);
        wordSet = eps[phoenix::clear(_val)] >> (+word[phoenix::insert(_val, _1)] % '\t');
        line    %= word >> '\t' >> wordSet >> '\n';
    }

    qi::rule<Iterator, std::string ()> word;
    qi::rule<Iterator, std::set<std::string> ()> wordSet;
    qi::rule<Iterator, IndexItem ()> line;
};

Grammar<spirit::istream_iterator> grammar;

class IndexItemParseRange {
public:
    typedef IndexItemIterator iterator;
    typedef IndexItemIterator const_iterator;
    typedef IndexItem value_type;

    IndexItemParseRange(const boost::shared_ptr<std::istream>& in) :
        in(in),
        inputBegin(*in) {
    }

    IndexItemIterator begin() {
        return IndexItemIterator(this);
    }

    IndexItemIterator end() const {
        return IndexItemIterator();
    }

private:
    friend class IndexItemIterator;

    IndexItem value;
    boost::shared_ptr<std::istream> in;
    boost::spirit::istream_iterator inputBegin;
    boost::spirit::istream_iterator inputEnd;
};

IndexItemIterator::IndexItemIterator() : count(-1) {}

IndexItemIterator::IndexItemIterator(IndexItemParseRange* range) :
    count(0),
    range(range)
{
    range->in->unsetf(std::ios::skipws);
}

const IndexItem& IndexItemIterator::dereference() const {
    return range->value;
}

bool IndexItemIterator::equal(const IndexItemIterator& other) const {
    return count == other.count;
}

void IndexItemIterator::increment() {
    assert(count != -1);
    bool ok = qi::parse(range->inputBegin, range->inputEnd, grammar, range->value);
    if (ok) {
        count++;
    } else {
        count = -1; // reached the end

        if (range->inputBegin != range->inputEnd) {
            std::cerr << "Trailing characters:" << std::endl;
            while (!range->in->eof()) {
                char c;
                *range->in >> c;
                std::cerr << c;
            }
        }
    }
}

RangeOfIndexItem parseIndex(const boost::shared_ptr<std::istream>& in) {
    return IndexItemParseRange(in);
}
