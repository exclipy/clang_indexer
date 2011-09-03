#pragma once

#include "types.hpp"

#include <boost/make_shared.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include <istream>

class IndexItemIterator : public boost::iterator_facade<
        IndexItemIterator,
        const IndexItem,
        boost::single_pass_traversal_tag,
        const IndexItem&,
        std::ptrdiff_t
    > {
public:

    IndexItemIterator();

    IndexItemIterator(std::istream& in);

private:
    friend class boost::iterator_core_access;

    const IndexItem& dereference() const;
    bool equal(const IndexItemIterator& other) const;
    void increment();

    boost::shared_ptr<IndexItem> value;
    std::ptrdiff_t i;
    std::istream* in;
    boost::spirit::istream_iterator inputBegin;
    boost::spirit::istream_iterator inputEnd;
};

boost::iterator_range<IndexItemIterator> parseIndex(std::istream& in);
