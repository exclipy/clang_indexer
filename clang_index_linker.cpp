#include "clang_index_parser.hpp"
#include "clang_index_printer.hpp"

#include <fstream>
#include <iostream>
#include <boost/foreach.hpp>

int main(int argc, char* argv[]) {
    using std::cerr;
    using std::cout;
    using std::endl;
    using std::ifstream;

    Index mergedIndex;

    for (int i = 1; i < argc; i++) {
        ifstream file(argv[i]);
        if (file.fail()) {
            cerr << argv[i] << " cannot be opened" << endl;
        } else {
            BOOST_FOREACH(const Index::value_type& it, parseIndex(file)) {
                mergedIndex[it.first].insert(it.second.begin(), it.second.end());
            }
        }
    }
    printIndex(cout, mergedIndex);
}
