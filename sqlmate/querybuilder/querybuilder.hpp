#ifndef QUERYBUILDER_H
#define QUERYBUILDER_H

#include <iostream>
#include <unordered_map>

class IModel;

namespace sqlmate {
    class QueryBuilder {
        public:
            static std::string insertQuery(
                const IModel& table,
                const std::unordered_map<std::string, std::string>& values
            );
            static std::string selectQuery(
                const IModel& table,
                const std::string& condition, const std::string& limit
            );
            static std::string updateQuery(
                const IModel& table,
                const std::unordered_map<std::string, std::string>& oldFields,
                const std::unordered_map<std::string, std::string>& newFields
            );
            static std::string deleteQuery(
                const IModel& table,
                const std::string id
            );
    };
}

#endif