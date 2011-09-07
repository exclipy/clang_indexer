#pragma once

#include "types.hpp"

#include <boost/iterator/iterator_facade.hpp>
#include <boost/make_shared.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/spirit/include/support_istream_iterator.hpp>

#include <istream>

class IndexItemParseRange;

class IndexItemIterator : public boost::iterator_facade<
        IndexItemIterator,
        const IndexItem,
        boost::single_pass_traversal_tag,
        const IndexItem&,
        std::ptrdiff_t
    > {
public:

    IndexItemIterator();

    IndexItemIterator(IndexItemParseRange* range);

private:
    friend class boost::iterator_core_access;

    const IndexItem& dereference() const;
    bool equal(const IndexItemIterator& other) const;
    void increment();

    std::ptrdiff_t count;
    IndexItemParseRange* range;
};

#include <boost/range/any_range.hpp>

typedef boost::any_range<
    IndexItem,
    boost::single_pass_traversal_tag,
    const IndexItem&,
    boost::use_default
    > RangeOfIndexItem;

RangeOfIndexItem parseIndex(const boost::shared_ptr<std::istream>& in);
