#pragma once

#include <istream>
#include <string>
#include <tr1/unordered_map>
#include <tr1/unordered_set>

std::tr1::unordered_map< std::string, std::tr1::unordered_set<std::string> > parseIndex(std::istream& in);
