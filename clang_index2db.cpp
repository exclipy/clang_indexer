#include "clang_index_parser.hpp"

#include <db_cxx.h>
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

    Db db(NULL, 0);
    try {
        db.set_error_stream(&cerr);
        db.set_flags(DB_DUPSORT);
        db.open(NULL, "index.db", NULL, DB_BTREE, DB_CREATE | DB_TRUNCATE, 0);

        for (int i = 1; i < argc; i++) {
            ifstream file(argv[i]);
            if (file.fail()) {
                cerr << argv[i] << " cannot be opened" << endl;
            } else {
                boost::iostreams::filtering_stream<boost::iostreams::input> zfile;
                zfile.push(boost::iostreams::gzip_decompressor());
                zfile.push(file);

                BOOST_FOREACH(const Index::value_type& it, parseIndex(zfile)) {
                    const std::string& usr = it.first;
                    BOOST_FOREACH(const std::string& location, it.second) {
                        Dbt key(const_cast<char*>(usr.c_str()), usr.size());
                        Dbt value(const_cast<char*>(location.c_str()), location.size());
                        db.put(NULL, &key, &value, DB_NODUPDATA);
                    }
                }
            }
        }

        db.close(0);
    } catch(DbException &e) {
        cerr << "Exception thrown: " << e.what() << endl;
        return 1;
    } 

}
