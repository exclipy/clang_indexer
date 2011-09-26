#include "clic_db.hpp"
#include <cassert>

int main(int argc, char* argv[]) {
    assert(argc == 2);
    ClicDb db(argv[1]);
    db.clear();
}
