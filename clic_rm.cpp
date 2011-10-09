#include "clic_parser.hpp"
#include "clic_printer.hpp"

#include "ClicDb.hpp"
#include <boost/foreach.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage:\n"
            << "    " << argv[0] << " <dbFilename> <indexFilename>\n";
        return 1;
    }

    ClicDb db(argv[1]);

    std::ifstream file(argv[2]);
    if (file.fail()) {
        std::cerr << argv[2] << " cannot be opened" << std::endl;
    } else {
        boost::iostreams::filtering_stream<boost::iostreams::input> zfile;
        zfile.push(boost::iostreams::gzip_decompressor());
        zfile.push(file);

        BOOST_FOREACH(const ClicIndex::value_type& it, parseIndex(zfile)) {
            db.rmMultiple(it.first, it.second);
        }
    }
}
