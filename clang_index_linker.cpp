#include "clang_index_parser.hpp"
#include "clang_index_printer.hpp"

#include <fstream>
#include <iostream>
#include <boost/foreach.hpp>

int main(int argc, char* argv[]) {
    Index mergedIndex;

    for (int i = 1; i < argc; i++) {
        std::ifstream file(argv[i]);
        if (file.fail()) {
            std::cerr << argv[i] << " cannot be opened" << std::endl;
        } else {
            Index index = parseIndex(file);
            BOOST_FOREACH(const Index::value_type& it, index) {
                mergedIndex[it.first].insert(it.second.begin(), it.second.end());
            }
        }
    }

    printIndex(std::cout, mergedIndex);
}
