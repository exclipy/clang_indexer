#include <db_cxx.h>
#include <cassert>

int main(int argc, char* argv[]) {
    assert(argc == 2);
    Db db(NULL, 0);
    try {
        db.set_error_stream(&std::cerr);
        db.set_flags(DB_DUPSORT);
        db.open(NULL, argv[1], NULL, DB_BTREE, DB_CREATE | DB_TRUNCATE, 0);
        db.close(0);
    } catch(DbException &e) {
        std::cerr << "Exception thrown: " << e.what() << std::endl;
        return 1;
    } 
}
