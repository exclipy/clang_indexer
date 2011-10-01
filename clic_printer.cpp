#include "clic_printer.hpp"

#include <boost/foreach.hpp>

void printIndex(
    std::ostream& out,
    const ClicIndex& index)
{
    BOOST_FOREACH(const ClicIndex::value_type& it, index) {
        if (!it.first.empty()) {
            out << it.first << '\t';
            printLocations(out, it.second);
            out << std::endl;
        }
    }
}

void printLocations(std::ostream& out, const std::set<std::string>& locations) {
    bool first = true;
    BOOST_FOREACH(const std::string& loc, locations) {
        if (!first) out << '\t';
        out << loc;
        first = false;
    }
}
