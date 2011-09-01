#pragma once

#include <string>
#include <tr1/unordered_map>
#include <tr1/unordered_set>

typedef std::tr1::unordered_map< std::string, std::tr1::unordered_set<std::string> > Index;
