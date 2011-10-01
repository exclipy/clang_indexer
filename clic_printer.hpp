#pragma once

#include "types.hpp"

#include <ostream>

void printIndex(
    std::ostream& out,
    const ClicIndex& index);

void printLocations(
    std::ostream& out,
    const std::set<std::string>& locations);
