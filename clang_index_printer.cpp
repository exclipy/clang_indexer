#include "clang_index_printer.hpp"

#include <boost/foreach.hpp>

void printIndex(
    std::ostream& out,
    const Index& index)
{
    BOOST_FOREACH(const Index::value_type& it, index) {
        out << it.first << '\t';
        bool first = true;
        BOOST_FOREACH(const std::string& jt, it.second) {
            if (!first) out << ' ';
            out << jt;
            first = false;
        }
        out << std::endl;
    }
}
