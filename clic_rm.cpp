#include "clic_parser.hpp"
#include "clic_printer.hpp"

#include "ClicDb.hpp"
#include <boost/foreach.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    assert(argc == 3);

    ClicDb db(argv[1]);

    std::ifstream file(argv[2]);
    if (file.fail()) {
        std::cerr << argv[2] << " cannot be opened" << std::endl;
    } else {
        boost::iostreams::filtering_stream<boost::iostreams::input> zfile;
        zfile.push(boost::iostreams::gzip_decompressor());
        zfile.push(file);

        BOOST_FOREACH(const ClicIndex::value_type& it, parseIndex(zfile)) {
            const std::string& usr = it.first;
            BOOST_FOREACH(const std::string& location, it.second) {
                db.rm(usr, location);
            }
        }
    }
}
