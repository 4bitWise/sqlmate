#include <iostream>
#include <vector>
#include <unordered_map>
#include <any>
#include <typeindex>

#pragma once

/**
 * @file QueryBuilder.hpp
 * @brief Interface for building DB queries.
 */
namespace sqlmate
{
    /**
     * @struct FieldInfo
     * @brief Represents a field's metadata in a database table.
     * 
     * Stores the value and type information of a field for use in database queries.
     */
    struct FieldInfo
    {
        /**
         * @brief Constructs a `FieldInfo` instance with the given value and type.
         * 
         * @param val The value of the field, stored as a `std::any`.
         * @param tId The type of the field, represented as a `std::type_index`.
         */
        FieldInfo(std::any val, std::type_index tId) : value(val), typeId(tId) {}

        std::any value; /**< The value of the field, stored as a generic type.*/
        std::type_index typeId; /**< The type of the field, represented as a `std::type_index`. */
    };

    /**
     * @class IQueryBuilder
     * @brief Interface for building SQL queries.
     * 
     * This interface defines methods for creating SQL queries for common database operations.
     * Classes implementing this interface are expected to provide query generation
     * tailored to a specific database system.
     */
    class IQueryBuilder
    {
    public:
        /**
         * @brief Virtual destructor for proper cleanup of derived classes.
         */
        virtual ~IQueryBuilder() = default;

        /**
         * @brief Generates a SQL query for creating a table.
         * 
         * The table will have an auto-incrementing `_id` field as the primary key.
         * 
         * @param tableName The name of the table to create.
         * @param columns A map of column names to their corresponding field metadata.
         * @return A SQL string for creating the table.
         */
        virtual std::string createTableQuery(const std::string &tableName, const std::unordered_map<std::string, FieldInfo> &columns) const = 0; // handle _id to auto incement, Create if not exist

        /**
         * @brief Generates a SQL query for inserting or replacing a row in a table.
         * 
         * @param tableName The name of the table to insert into.
         * @param values A map of column names to their corresponding field values.
         * @return A SQL string for inserting or replacing a row.
         */
        virtual std::string insertQuery(const std::string &tableName, const std::unordered_map<std::string, FieldInfo> &values) const = 0; // insert if not exist

        /**
         * @brief Generates a SQL query for selecting rows from a table.
         * 
         * @param tableName The name of the table to query.
         * @param condition An optional condition for filtering rows.
         * @param limit An optional limit on the number of rows to return. Defaults to no limit.
         * @return A SQL string for selecting rows.
         */
        virtual std::string selectQuery(const std::string &tableName, const std::string &condition = "",
                                        int limit = -1) const = 0;

        /**
         * @brief Generates a SQL query for deleting a row from a table.
         * 
         * @param tableName The name of the table to delete from.
         * @param id The primary key value of the row to delete.
         * @return A SQL string for deleting the row.
         */
        virtual std::string deleteQuery(const std::string &tableName, int id) const = 0; // delete if exists

        /**
         * @brief Generates a SQL query for dropping a table.
         * 
         * @param tableName The name of the table to drop.
         * @return A SQL string for dropping the table.
         */
        virtual std::string dropTableQuery(const std::string &tableName) const = 0; // drop if exists
    };

} // namespace sqlmate
