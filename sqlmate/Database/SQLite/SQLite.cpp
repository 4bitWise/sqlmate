#include "./SQLite.hpp"

namespace sqlmate
{
    SQLite::SQLite() : _connected(false)
    {
        qbuilder = std::make_shared<QueryBuilder>();
    }

    void SQLite::connect(std::string url)
    {
        // std::cout << "Connecting to " << url << std::endl;
        int rc = sqlite3_open(
            url.c_str(),
            &_db);

        if (rc != SQLITE_OK)
            throw DatabaseError("[ERROR]: Unable to connect to database: " + url);

        _connected = true;
    }
    bool SQLite::isConnected() { return _connected; }

    void SQLite::disconnect()
    {
        // std::cout << "Disconnecting" << std::endl;
        sqlite3_close(_db);
        _connected = false;
    }

    void SQLite::exec(std::string query, QueryCallBackWrapper *cb_wrapper)
    {
        int rc;

        if (cb_wrapper == nullptr)
        {
            rc = sqlite3_exec(
                _db,
                query.c_str(),
                NULL,
                0,
                NULL);
        }
        else
        {
            rc = sqlite3_exec(
                _db,
                query.c_str(),
                [](void *data, int argc, char **argv, char **azColName) -> int
                {
                    QueryCallBackWrapper *wrapper = static_cast<QueryCallBackWrapper *>(data);
                    return (wrapper->get()(argc, argv, azColName));
                },
                cb_wrapper,
                NULL);
        }

        if (rc != SQLITE_OK)
        {
            throw DatabaseError("[ERR]: " + std::string(sqlite3_errmsg(_db)));
        }
        // else
        // {
        //     std::cout << "Query Successfully executed !" << std::endl;
        // }
    }
}