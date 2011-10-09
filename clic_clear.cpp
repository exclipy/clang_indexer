#include "ClicDb.hpp"
#include <cassert>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage:\n"
            << "    " << argv[0] << " <dbFilename>\n";
        return 1;
    }
    ClicDb db(argv[1]);
    db.clear();
}
