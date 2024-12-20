#include <iostream>
#include <vector>
#include <unordered_map>
#include <any>
#include <typeindex>

#pragma once

namespace sqlmate
{

    struct FieldInfo
    {
        FieldInfo(std::any val, std::type_index tId) : value(val), typeId(tId) {}

        std::any value;
        std::type_index typeId;
    };

    class IQueryBuilder
    {
    public:
        virtual ~IQueryBuilder() = default;

        // CREATE TABLE
        virtual std::string createTableQuery(const std::string &tableName, const std::unordered_map<std::string, FieldInfo> &columns) const = 0; // handle _id to auto incement, Create if not exist

        // INSERT
        virtual std::string insertQuery(const std::string &tableName, const std::unordered_map<std::string, FieldInfo> &values) const = 0; // insert if not exist

        // SELECT
        virtual std::string selectQuery(const std::string &tableName, const std::string &condition = "",
                                        int limit = -1) const = 0;

        // DELETE
        virtual std::string deleteQuery(const std::string &tableName, int id) const = 0; // delete if exists

        // Drop Table
        virtual std::string dropTableQuery(const std::string &tableName) const = 0; // drop if exists
    };

} // namespace sqlmate
