#include <iostream>
#include <sqlite3.h>

class SQLite
{
    public:
        SQLite() : _obj(nullptr), _db("")
        {};

        SQLite(const std::string& dbName) : _obj(nullptr), _db(dbName)
        {};

        void open(void)
        {
            int rc = sqlite3_open(
                _db.c_str(),
                &_obj
            );
            
            if (rc != SQLITE_OK) {
                throw std::runtime_error("[ERROR]: An error occured during the connection !");
            }
        };

        void close(void) noexcept
        {
            sqlite3_close(_obj);
        };

    private:
        std::string _db;
        sqlite3 *_obj;
};