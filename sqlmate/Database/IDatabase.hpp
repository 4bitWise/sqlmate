/**
 * @file IDatabase.hpp
 * @brief Defines the abstract interface for a database class in the sqlmate namespace.
 */

#include <iostream>
#include <memory>
#include <functional>
#include "../Exceptions/Database.hpp"
#include "../Exceptions/QueryBuilder.hpp"
#include "../QueryBuilder/QueryBuilder.hpp"

#pragma once

namespace sqlmate
{
    /**
     * @typedef db_callback
     * @brief Alias for a database callback function type.
     * 
     * This type represents a callback function used by database operations.
     * The function takes the following parameters:
     * - The number of columns in the current row (`int`).
     * - An array of column values (`char **`).
     * - An array of column names (`char **`).
     * 
     * The function should return an integer value.
     */
    typedef std::function<int(int, char **, char **)> db_callback;

    class QueryCallBackWrapper
    {
    public:
        QueryCallBackWrapper(db_callback cb) : _func(cb)
        {
        }

        db_callback get()
        {
            return _func;
        }

    private:
        db_callback _func;
    };

    /**
     * @class IDatabase
     * @brief Abstract interface for database operations.
     * 
     * Provides a standard interface for connecting to, querying, and managing databases.
     * Derived classes must implement the virtual methods for specific database types.
     */
    class IDatabase
    {
    public:
        /**
         * @brief Virtual destructor to ensure proper cleanup of derived classes.
         */
        virtual ~IDatabase() = default;

        /**
         * @brief Connects to the database using the specified URL.
         * 
         * @param url The URL or path to the database.
         * @throw DatabaseError If the connection fails.
         */
        virtual void connect(std::string url) = 0;

        /**
         * @brief Checks if the database is currently connected.
         * 
         * @return True if the database is connected, false otherwise.
         */
        virtual bool isConnected() = 0;

        /**
         * @brief Disconnects from the database.
         * 
         * @throw DatabaseError If disconnection fails or the database is not connected.
         */
        virtual void disconnect() = 0;

        /**
         * @brief Executes a database query with an optional callback.
         * 
         * @param query The SQL query to execute.
         * @param cb_wrapper A callback function to handle query results, or `nullptr` if not used.
         * @throw DatabaseError If the query execution fails.
         */
        virtual void exec(std::string query, QueryCallBackWrapper *cb_wrapper) = 0;

    public:
        /**
         * @brief Shared pointer to an `IQueryBuilder` for constructing SQL queries.
         * 
         * This allows the user to build SQL queries programmatically.
         */
        std::shared_ptr<IQueryBuilder> qbuilder;
    };
} // namespace sqlmate
