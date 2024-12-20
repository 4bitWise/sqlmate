#include <iostream>
#include <memory>
#include "../Exceptions/Database.hpp"
#include "../Exceptions/QueryBuilder.hpp"
#include "../QueryBuilder/QueryBuilder.hpp"

#pragma once

namespace sqlmate
{
    typedef int (*db_callback)(void *, int, char **, char **);

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
