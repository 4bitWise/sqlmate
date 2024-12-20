/**
 * @file databaseManager.hpp
 * @brief Provides a singleton class for managing database connections in the sqlmate namespace.
 */

#include <unordered_map>
#include <memory>
#include "./IDatabase.hpp"
#include "./SQLite/SQLite.hpp"

#pragma once

namespace sqlmate
{

    /**
     * @enum DatabaseType
     * @brief Enum representing supported database types.
     */
    enum DatabaseType
    {
        SQLITE ///< Represents an SQLite database.
    };

    /**
     * @class DatabaseManager
     * @brief A singleton class to manage database connections.
     * 
     * This class is responsible for registering, connecting, and disconnecting databases.
     */
    class DatabaseManager
    {
    public:
        /**
         * @brief Deleted copy constructor to enforce the singleton pattern.
         */
        DatabaseManager(DatabaseManager const &) = delete;

        /**
         * @brief Deleted assignment operator to enforce the singleton pattern.
         */
        void operator=(DatabaseManager const &) = delete;

        /**
         * @brief Retrieves the singleton instance of the DatabaseManager.
         * 
         * @return Reference to the singleton DatabaseManager instance.
         */
        static DatabaseManager &getInstance()
        {
            static DatabaseManager instance;
            return instance;
        }

        /**
         * @brief Connects to a database using the specified URL and type.
         * 
         * If the database is not registered, it will be registered first.
         * 
         * @param url The URL or path of the database.
         * @param type The type of the database (e.g., SQLITE).
         * @return A shared pointer to the connected database.
         * @throw DatabaseError If the database connection fails.
         */
        std::shared_ptr<IDatabase> connect(const std::string &url, DatabaseType type)
        {
            if (!isRegistered(url))
                registerDatabase(url, type);
            databases[url]->connect(url);
            return (databases[url]);
        }

        /**
         * @brief Checks if a database is connected.
         * 
         * @param url The URL or path of the database.
         * @return True if the database is connected, false otherwise.
         * @throw DatabaseError If the database is not registered.
         */
        bool isConnected(const std::string &url)
        {
            if (!isRegistered(url))
                throw DatabaseError("Database at " + url + " not registered");
            return (databases[url]->isConnected());
        }

        /**
         * @brief Disconnects a database using the specified URL.
         * 
         * @param url The URL or path of the database.
         * @throw DatabaseError If the database is not registered.
         */
        void disconnect(const std::string &url)
        {
            if (!isRegistered(url))
                throw DatabaseError("Database at " + url + " not registered");
            databases[url]->disconnect();
        }

    private:
        /**
         * @brief Private default constructor to enforce the singleton pattern.
         */
        DatabaseManager() = default;

        std::unordered_map<std::string, std::shared_ptr<IDatabase>> databases; ///< A map of database URLs to database instances.

        /**
         * @brief Checks if a database is registered.
         * 
         * @param url The URL or path of the database.
         * @return True if the database is registered, false otherwise.
         */
        bool isRegistered(const std::string &url)
        {
            return (databases.find(url) != databases.end());
        }

        /**
         * @brief Registers a database using the specified URL and type.
         * 
         * @param url The URL or path of the database.
         * @param type The type of the database (e.g., SQLITE).
         * @throw DatabaseError If the database type is unsupported.
         */
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
} // namespace sqlmate
