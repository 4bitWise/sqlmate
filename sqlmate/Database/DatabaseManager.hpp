#include <unordered_map>
#include <memory>
#include "./IDatabase.hpp"
#include "./SQLite/SQLite.hpp"

#pragma once

namespace sqlmate
{

    enum DatabaseType
    {
        SQLITE
    };

    class DatabaseManager
    {
    public:
        DatabaseManager(DatabaseManager const &) = delete;
        void operator=(DatabaseManager const &) = delete;

        static DatabaseManager &getInstance()
        {
            static DatabaseManager instance;
            return instance;
        }

        std::shared_ptr<IDatabase> connect(const std::string &url, DatabaseType type)
        {
            if (!isRegistered(url))
                registerDatabase(url, type);
            databases[url]->connect(url);
            return (databases[url]);
        }

        bool isConnected(const std::string &url)
        {
            if (!isRegistered(url))
                throw DatabaseError("Database at " + url + " not registered");
            return (databases[url]->isConnected());
        }

        void disconnect(const std::string &url)
        {
            if (!isRegistered(url))
                throw DatabaseError("Database at " + url + " not registered");
            databases[url]->disconnect();
        }

    private:
        DatabaseManager() = default;

        std::unordered_map<std::string, std::shared_ptr<IDatabase>> databases;

        bool isRegistered(const std::string &url)
        {
            return (databases.find(url) != databases.end());
        }

        void registerDatabase(const std::string &url, DatabaseType type)
        {

            switch (type)
            {
            case DatabaseType::SQLITE:
                databases.insert({url, std::make_shared<SQLite>()});
                break;
            default:
                throw DatabaseError("Unable to register database: " + url);
                break;
            }
        }
    };

}
