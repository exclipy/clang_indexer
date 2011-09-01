#include "clang_index_parser.hpp"
#include "clang_index_printer.hpp"

#include <iostream>
#include <boost/foreach.hpp>

int main() {
    Index parsedResult = parseIndex(std::cin);

    printIndex(std::cout, parsedResult);
}
