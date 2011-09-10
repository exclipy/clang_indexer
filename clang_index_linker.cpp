#include "clang_index_parser.hpp"
#include "clang_index_printer.hpp"

#include <fstream>
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>

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
            boost::iostreams::filtering_stream<boost::iostreams::input> zfile;
            zfile.push(boost::iostreams::gzip_decompressor());
            zfile.push(file);

            BOOST_FOREACH(const Index::value_type& it, parseIndex(zfile)) {
                mergedIndex[it.first].insert(it.second.begin(), it.second.end());
            }
        }
    }

    boost::iostreams::filtering_stream<boost::iostreams::output> zout;
    zout.push(boost::iostreams::gzip_compressor());
    zout.push(cout);
    printIndex(zout, mergedIndex);
}
