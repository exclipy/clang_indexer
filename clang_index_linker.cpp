#include "clang_index_parser.hpp"
#include "clang_index_printer.hpp"

#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>
#include <boost/foreach.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/noncopyable.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/any_range.hpp>
#include <iterator>

typedef boost::any_range<
    IndexItem,
    boost::single_pass_traversal_tag,
    const IndexItem&,
    boost::use_default
    > RangeOfIndexItem;

class RangeOfIndexItemComparator {
public:
    bool operator()(const RangeOfIndexItem& a, const RangeOfIndexItem& b) {
        return a.begin()->first < b.begin()->first;
    }
};

class OctopusMergeRange;

class OctopusMergeIterator : public boost::iterator_facade<
        OctopusMergeIterator,
        const IndexItem,
        boost::single_pass_traversal_tag,
        const IndexItem&,
        std::ptrdiff_t> {
public:
    typedef IndexItem type;

    OctopusMergeIterator();

    OctopusMergeIterator(OctopusMergeRange* range);    

private:
    friend class boost::iterator_core_access;

    const IndexItem& dereference() const;
    bool equal(const OctopusMergeIterator& other) const;
    void increment();

    std::ptrdiff_t count;
    OctopusMergeRange* range;
};

class OctopusMergeRange {
public:
    typedef OctopusMergeIterator iterator;
    typedef OctopusMergeIterator const_iterator;
    typedef IndexItem value_type;

    OctopusMergeRange() : pq(RangeOfIndexItemComparator()) {
    }

    void push(const RangeOfIndexItem& range) {
        pq.push(range);
    }

    OctopusMergeIterator begin() {
        return OctopusMergeIterator(this);
    }

    OctopusMergeIterator end() {
        return OctopusMergeIterator();
    }

private:
    friend class OctopusMergeIterator;

    IndexItem value;
    typedef std::priority_queue<
            RangeOfIndexItem, std::vector<RangeOfIndexItem>, RangeOfIndexItemComparator
            > PriorityQueueType;
    PriorityQueueType pq;
};

OctopusMergeIterator::OctopusMergeIterator() : count(-1) {}

OctopusMergeIterator::OctopusMergeIterator(OctopusMergeRange* range) :
        count(0),
        range(range) {
    increment();
}
    

const IndexItem& OctopusMergeIterator::dereference() const {
    assert(count != -1);
    return range->value;
}

bool OctopusMergeIterator::equal(const OctopusMergeIterator& other) const {
    return count == other.count;
}

void OctopusMergeIterator::increment() {
    std::cout << "increment\n";
    if (range->pq.empty()) {
        count = -1;
        return;
    }
    range->value = *range->pq.top().begin();
    std::cout << range->pq.size() << "'" << range->value.first << "'" << range->value.second.size() << std::endl;
    boost::copy(range->value.second, std::ostream_iterator<std::string>(std::cout));
    range->pq.pop();
    // Merge together the frontmost items that have the same USR
    while (!range->pq.empty() && range->pq.top().begin()->first == range->value.first) {
        const std::set<std::string>& locations = range->pq.top().begin()->second;
        std::cout << range->pq.size() << "'" << range->value.first << "'" << range->value.second.size() << std::endl;
        boost::copy(locations, std::ostream_iterator<std::string>(std::cout));
        range->value.second.insert(locations.begin(), locations.end());
        range->pq.pop();
    }
    std::cout << "value has els: " << range->value.second.size() << std::endl;
    count++;
}

int main(int argc, char* argv[]) {
    using std::cerr;
    using std::cout;
    using std::endl;

    OctopusMergeRange merger;
    for (int i = 1; i < argc; i++) {
        boost::shared_ptr<std::ifstream> file(new std::ifstream(argv[i]));
        if (file->fail()) {
            cerr << argv[i] << " cannot be opened" << endl;
        } else {
            RangeOfIndexItem indexItems = parseIndex(file);
            merger.push(indexItems);
        }
    }
    BOOST_FOREACH(const IndexItem& it, merger) {
        if (!it.first.empty()) {
            cout << it.first << '\t';
            bool first = true;
            BOOST_FOREACH(const std::string& jt, it.second) {
                if (!first) cout << '\t';
                cout << jt;
                first = false;
            }
            cout << std::endl;
        }
    }
}
