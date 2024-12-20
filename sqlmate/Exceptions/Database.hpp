#include <iostream>
#include <exception>

#pragma once

namespace sqlmate
{

    class DatabaseError : public std::exception
    {
    public:
        DatabaseError(const std::string &message) : _msg(message)
        {
        }

        const char *what() const throw()
        {
            return _msg.c_str();
        }

    private:
        std::string _msg;
    };

}