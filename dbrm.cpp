#include "clang_index_parser.hpp"
#include "clang_index_printer.hpp"

#include <db_cxx.h>
#include <boost/foreach.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <fstream>
#include <iostream>

int main(int argc, char* argv[]) {
    assert(argc == 3);

    Db db(NULL, 0);
    try {
        db.set_error_stream(&std::cerr);
        db.set_flags(DB_DUPSORT);
        db.open(NULL, argv[1], NULL, DB_BTREE, 0, 0);

        std::ifstream file(argv[2]);
        if (file.fail()) {
            std::cerr << argv[2] << " cannot be opened" << std::endl;
        } else {
            boost::iostreams::filtering_stream<boost::iostreams::input> zfile;
            zfile.push(boost::iostreams::gzip_decompressor());
            zfile.push(file);

            Dbc* cursor;
            try {
                db.cursor(0, &cursor, 0);
                BOOST_FOREACH(const Index::value_type& it, parseIndex(zfile)) {
                    const std::string& usr = it.first;
                    BOOST_FOREACH(const std::string& location, it.second) {
                        Dbt key(const_cast<char*>(usr.c_str()), usr.size());
                        Dbt value(const_cast<char*>(location.c_str()), location.size());
                        if (cursor->get(&key, &value, DB_GET_BOTH) != DB_NOTFOUND) {
                            cursor->del(0);
                        }
                    }
                }
                cursor->close();
            } catch(DbException &e) {
                cursor->close();
                throw e;
            }
        }

        db.compact(0, 0, 0, 0, DB_FREE_SPACE, 0);

        db.close(0);
    } catch(DbException &e) {
        std::cerr << "Exception thrown: " << e.what() << std::endl;
        return 1;
    } 
}
