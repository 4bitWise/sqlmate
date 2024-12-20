#include "../IDatabase.hpp"
#include <any>
#include <typeindex>
#include <sstream>
#include <sqlite3.h>

#pragma once

namespace sqlmate
{
    class SQLite : public IDatabase
    {

    public:
        SQLite();

        void connect(std::string url) override;
        bool isConnected() override;
        void disconnect() override;

        void exec(std::string query, QueryCallBackWrapper *cb_wrapper) override;

    private:
        bool _connected;
        sqlite3 *_db;

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
                        query << " INTEGER PRIMARY KEY";
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
