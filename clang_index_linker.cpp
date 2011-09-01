#include "clang_index_parser.hpp"

#include <iostream>
#include <string>
#include <tr1/unordered_map>
#include <tr1/unordered_set>
#include <boost/foreach.hpp>

using std::tr1::unordered_set;
using std::tr1::unordered_map;

int main() {
    unordered_map< std::string, unordered_set<std::string> > parsedResult = parseIndex(std::cin);

    typedef unordered_map< std::string, unordered_set<std::string> >::value_type ValueType;
    BOOST_FOREACH(const ValueType& it, parsedResult) {
        std::cout << it.first << '\t';
        bool first = true;
        BOOST_FOREACH(const std::string& jt, it.second) {
            if (!first) std::cout << ' ';
            std::cout << jt;
            first = false;
        }
        std::cout << std::endl;
    }
}
