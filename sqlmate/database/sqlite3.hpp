#include <iostream>
#include <sqlite3.h>

class SQLite
{
    public:
        SQLite() : _db(nullptr), _filename("")
        {};

        SQLite(const std::string& filename) : _db(nullptr), _filename(filename)
        {};

        void open(void)
        {
            int rc = sqlite3_open(
                _filename.c_str(),
                &_db
            );
            
            if (rc != SQLITE_OK) {
                throw std::runtime_error("[ERROR]: An error occured during the connection !");
            }
        };

        int exec(std::string query, sqlite3_callback cb) noexcept
        {
            int rc;
    
            if (cb == nullptr) {
                rc = sqlite3_exec(
                    _db,
                    query.c_str(),
                    NULL,
                    0,
                    NULL
                );
            } else {
                rc = sqlite3_exec(
                    _db,
                    query.c_str(),
                    cb,
                    0,
                    NULL
                );
            }

            if (rc != SQLITE_OK) {
                std::cout << "[ERR]: " << sqlite3_errmsg(_db) << std::endl;
                return 1;
            } else {
                std::cout << "Query Successfully executed !" << std::endl;
            }

            return 0;
        };

        void close(void) noexcept
        {
            sqlite3_close(_db);
        };

    private:
        std::string _filename;
        sqlite3 *_db;
};