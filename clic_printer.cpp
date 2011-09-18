#include "clic_printer.hpp"

#include <boost/foreach.hpp>

void printIndex(
    std::ostream& out,
    const ClicIndex& index)
{
    BOOST_FOREACH(const ClicIndex::value_type& it, index) {
        if (!it.first.empty()) {
            out << it.first << '\t';
            bool first = true;
            BOOST_FOREACH(const std::string& jt, it.second) {
                if (!first) out << '\t';
                out << jt;
                first = false;
            }
            out << std::endl;
        }
    }
}
