#include "ClicDb.hpp"

#include "clic_printer.hpp"
#include "clic_parser.hpp"
#include <boost/foreach.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <iterator>

ClicDb::ClicDb(const char* dbFilename) : db(NULL, 0)
{
    try {
        db.set_error_stream(&std::cerr);
        db.open(NULL, dbFilename, NULL, DB_BTREE, DB_CREATE, 0);
    } catch(DbException &e) {
        std::cerr << "Exception thrown: " << e.what() << std::endl;
        exit(1);
    }
}

ClicDb::~ClicDb() {
    db.close(0);
}

void ClicDb::clear() {
    try {
        db.truncate(0, 0, 0);
    } catch(DbException &e) {
        std::cerr << "Exception thrown: " << e.what() << std::endl;
        exit(1);
    }
}

void ClicDb::set(const std::string& usr, const std::set<std::string>& locations) {
    std::stringstream ss;
    printLocations(ss, locations);
    std::string locationsString = ss.str();
    Dbt key(const_cast<char*>(usr.c_str()), usr.size());
    Dbt value(const_cast<char*>(locationsString.c_str()), locationsString.size());
    db.put(NULL, &key, &value, 0);
}

std::set<std::string> ClicDb::get(const std::string& usr) {
    Dbt key(const_cast<char*>(usr.c_str()), usr.size());
    Dbt value;
    if (db.get(NULL, &key, &value, 0) == DB_NOTFOUND)
        return std::set<std::string>();
    std::string str((char*)value.get_data(), value.get_size());
    std::set<std::string> result;
    boost::algorithm::split(result, str, boost::algorithm::is_any_of("\t"));
    return result;
}

void ClicDb::addMultiple(const std::string& usr, const std::set<std::string>& locationsToAdd) {
    std::set<std::string> storedLocations = get(usr);
    int originalCount = storedLocations.size();
    boost::copy(locationsToAdd, std::inserter(storedLocations, storedLocations.begin()));
    if (storedLocations.size() > originalCount)
        set(usr, storedLocations);
}

void ClicDb::rmMultiple(const std::string& usr, const std::set<std::string>& locationsToRemove) {
    std::set<std::string> storedLocations = get(usr);
    int originalCount = storedLocations.size();
    BOOST_FOREACH(std::string loc, locationsToRemove)
        storedLocations.erase(loc);
    if (storedLocations.size() < originalCount)
        set(usr, storedLocations);
}
