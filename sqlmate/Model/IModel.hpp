#include <iostream>
#include <vector>

#pragma once
namespace sqlmate
{

    class IModel
    {
    public:
        virtual ~IModel() = default;

        virtual std::string getTableName() const = 0;
        virtual void save() const = 0;
        virtual void remove() const = 0;

        // TODO add create table and delete table
    };

}
