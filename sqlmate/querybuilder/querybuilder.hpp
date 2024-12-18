#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include <iostream>
#include <unordered_map>
#include <string>
#include <sstream>
#include "IModel.hpp"
#include "decorators.hpp"
#include "AModel.hpp"

class IModel;

namespace sqlmate {
    class QueryBuilder {
        public:
            static std::string insertQuery(
                const IModel& table,
                const std::unordered_map<std::string, std::string>& values
            )
            {
                std::ostringstream query;
                query << "INSERT INTO " << table.getTableName() << " (";

                for (auto it = values.begin(); it != values.end(); ++it) {
                    if (it != values.begin()) query << ", ";
                    query << it->first;
                }

                query << ") VALUES (";

                for (auto it = values.begin(); it != values.end(); ++it) {
                    if (it != values.begin()) query << ", ";
                    query << "'" << it->second << "'";
                }

                query << ");";
                return query.str();
            };

            static std::string selectQuery(
                const IModel& table,
                const std::string& condition = "",
                const std::string& limit = ""
            )
            {
                std::ostringstream query;
                query << "SELECT * FROM " << table.getTableName();

                if (!condition.empty()) {
                    query << " WHERE " << condition;
                }

                if (!limit.empty()) {
                    query << " LIMIT " << limit;
                }

                query << ";";
                return query.str();
            };

            static std::string updateQuery(
                const IModel& table,
                const std::unordered_map<std::string, std::string>& oldFields,
                const std::unordered_map<std::string, std::string>& newFields
            )
            {
                std::ostringstream query;
                query << "UPDATE " << table.getTableName() << " SET ";

                for (auto it = newFields.begin(); it != newFields.end(); ++it) {
                    if (it != newFields.begin()) query << ", ";
                    query << it->first << " = '" << it->second << "'";
                }

                if (!oldFields.empty()) {
                    query << " WHERE ";
                    for (auto it = oldFields.begin(); it != oldFields.end(); ++it) {
                        if (it != oldFields.begin()) query << " AND ";
                        query << it->first << " = '" << it->second << "'";
                    }
                }

                query << ";";
                return query.str();
            };

            static std::string deleteQuery(
                const IModel& table,
                const std::string id
            )
            {
                std::ostringstream query;

                query << "DELETE FROM " << table.getTableName() << " WHERE ID = '" << id << "';";
                return query.str();
            };
    };
}

#endif