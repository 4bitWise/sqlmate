#include <iostream>
#include <memory>
#include <functional>
#include "../Exceptions/Database.hpp"
#include "../Exceptions/QueryBuilder.hpp"
#include "../QueryBuilder/QueryBuilder.hpp"

#pragma once

namespace sqlmate
{
    typedef std::function<int(void *, int, char **, char **)> db_callback;

    class IDatabase
    {
    public:
        virtual ~IDatabase() = default;

        virtual void connect(std::string url) = 0;
        virtual bool isConnected() = 0;
        virtual void disconnect() = 0;

        virtual void exec(std::string query, db_callback callback) = 0;

    public:
        std::shared_ptr<IQueryBuilder> qbuilder;
    };

}
