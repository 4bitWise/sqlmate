#include <iostream>
#include <memory>
#include <functional>
#include "../Exceptions/Database.hpp"
#include "../Exceptions/QueryBuilder.hpp"
#include "../QueryBuilder/QueryBuilder.hpp"

#pragma once

namespace sqlmate
{
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

    class IDatabase
    {
    public:
        virtual ~IDatabase() = default;

        virtual void connect(std::string url) = 0;
        virtual bool isConnected() = 0;
        virtual void disconnect() = 0;

        virtual void exec(std::string query, QueryCallBackWrapper *cb_wrapper) = 0;

    public:
        std::shared_ptr<IQueryBuilder> qbuilder;
    };

}
