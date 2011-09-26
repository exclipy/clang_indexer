#pragma once

#include <db_cxx.h>

class ClicCursor {
    public:
        ClicCursor(Db& db) {
            db.cursor(0, &cursor, 0);
        }
        void rm(Dbt* key, Dbt* value) {
            if (cursor->get(key, value, DB_GET_BOTH) != DB_NOTFOUND)
                cursor->del(0);
        }
        ~ClicCursor() {
            cursor->close();
        }
    private:
        Dbc* cursor;
};

class ClicDb {
    public:
        ClicDb(const char* dbFilename) : db(NULL, 0)
        {
            try {
                db.set_error_stream(&std::cerr);
                db.set_flags(DB_DUPSORT);
                db.open(NULL, dbFilename, NULL, DB_BTREE, DB_CREATE, 0);
            } catch(DbException &e) {
                std::cerr << "Exception thrown: " << e.what() << std::endl;
                exit(1);
            }
        }

        void clear() {
            try {
                db.truncate(0, 0, 0);
            } catch(DbException &e) {
                std::cerr << "Exception thrown: " << e.what() << std::endl;
                exit(1);
            }
        }

        void put(const std::string& usr, const std::string& location) {
            try {
                Dbt key(const_cast<char*>(usr.c_str()), usr.size());
                Dbt value(const_cast<char*>(location.c_str()), location.size());
                db.put(NULL, &key, &value, DB_NODUPDATA);
            } catch(DbException &e) {
                std::cerr << "Exception thrown: " << e.what() << std::endl;
                exit(1);
            }
        }

        void rm(const std::string& usr, const std::string& location) {
            ClicCursor cursor(db);
            Dbt key(const_cast<char*>(usr.c_str()), usr.size());
            Dbt value(const_cast<char*>(location.c_str()), location.size());
            cursor.rm(&key, &value);
        }

        ~ClicDb() {
            db.compact(0, 0, 0, 0, DB_FREE_SPACE, 0);
            db.close(0);
        }

    private:
        Db db;
};
