/**
 * @file SQLite.hpp
 * @brief C++ encapsulation of the SQLite3 C library.
 */

#include "../IDatabase.hpp"
#include <any>
#include <typeindex>
#include <sstream>
#include <sqlite3.h>

#pragma once

namespace sqlmate
{
    /**
     * @class SQLite
     * @brief Encapsulates SQLite3 functionality in a C++ interface.
     * 
     * Provides methods to connect, execute queries, and manage database operations using SQLite3.
     */

    class SQLite : public IDatabase
    {

    public:
        /**
         * @brief Constructs an SQLite instance.
         * Initializes the connection state to false and shared pointer to query builder.
         */
        SQLite();


        /**
         * @brief Connects to the specified SQLite database.
         * 
         * @param url The file path or URL of the SQLite database.
         * @throw DatabaseError If the connection fails.
         */
        void connect(std::string url) override;
        
        /**
         * @brief Checks whether the database is currently connected.
         * 
         * @return True if connected, false otherwise.
         */
        bool isConnected() override;

        /**
         * @brief Disconnects from the SQLite database.
         */
        void disconnect() override;

        /**
         * @brief Executes a SQL query on the connected database.
         * 
         * @param query The SQL query string to execute.
         * @param callback An optional callback function for processing query results.
         *        Should match the signature `int(void*, int, char**, char**)`.
         * @throw DatabaseError If the query execution fails.
         */
        void exec(std::string query, db_callback callback) override;

    private:
        bool _connected; ///< Indicates the connection status to the database.
        sqlite3 *_db; ///< Pointer to the SQLite database instance.

    public:
        class QueryBuilder : public IQueryBuilder
        {
        public:
            std::string createTableQuery(const std::string &tableName, const std::unordered_map<std::string, FieldInfo> &columns) const override
            {
                std::ostringstream query;
                query << "CREATE TABLE IF NOT EXISTS " << tableName << " (";

                bool first = true;
                for (const auto &[columnName, field] : columns)
                {
                    if (!first)
                        query << ", ";
                    first = false;

                    query << columnName << " " << typeToSQLiteType(field.typeId);
                    if (columnName == "_id")
                        query << " INTEGER PRIMARY KEY ";
                    // query << " INTEGER PRIMARY KEY AUTOINCREMENT";
                }
                query << ");";

                return query.str();
            }

            std::string insertQuery(const std::string &tableName, const std::unordered_map<std::string, FieldInfo> &values) const override
            {
                std::ostringstream query;
                query << "INSERT OR REPLACE INTO " << tableName << " (";

                bool first = true;
                for (const auto &[columnName, field] : values)
                {
                    if (!first)
                        query << ", ";
                    first = false;

                    query << columnName;
                }

                query << ") VALUES (";

                first = true;
                for (const auto &[columnName, field] : values)
                {
                    if (!first)
                        query << ", ";
                    first = false;

                    query << formatValue(field);
                }

                query << ");";

                return query.str();
            }

            std::string selectQuery(const std::string &tableName, const std::string &condition = "",
                                    int limit = -1) const override
            {
                std::ostringstream query;
                query << "SELECT * FROM " << tableName;
                if (!condition.empty())
                    query << " WHERE " << condition;
                if (limit > 0)
                    query << " LIMIT " << limit;
                query << ";";

                return query.str();
            }

            std::string deleteQuery(const std::string &tableName, int id) const override
            {
                std::ostringstream query;
                query << "DELETE FROM " << tableName << " WHERE _id = " << id << ";";
                return query.str();
            }

            std::string dropTableQuery(const std::string &tableName) const override
            {
                return "DROP TABLE IF EXISTS " + tableName + ";";
            }

        private:
            std::string typeToSQLiteType(const std::type_index &typeId) const
            {
                if (typeId == typeid(int))
                    return "INTEGER";
                else if (typeId == typeid(double))
                    return "REAL";
                else if (typeId == typeid(std::string))
                    return "TEXT";
                else if (typeId == typeid(bool))
                    return "BOOLEAN"; // SQLite uses INTEGER for boolean values
                else
                    throw QueryBuilderError("Unsupported type for SQLite");
            }

            std::string formatValue(const FieldInfo &field) const
            {
                if (field.typeId == typeid(int))
                    return std::to_string(std::any_cast<std::reference_wrapper<int>>(field.value).get());
                else if (field.typeId == typeid(double))
                    return std::to_string(std::any_cast<std::reference_wrapper<double>>(field.value).get());
                else if (field.typeId == typeid(std::string))
                    return "'" + std::any_cast<std::reference_wrapper<std::string>>(field.value).get() + "'";
                else if (field.typeId == typeid(bool))
                    return std::to_string(std::any_cast<std::reference_wrapper<bool>>(field.value).get() ? 1 : 0);
                else
                    throw QueryBuilderError("Unsupported type for value formatting");
            }
        };
    };

}
