#include <iostream>
#include <exception>

#pragma once

namespace sqlmate
{

    class ModelError : public std::exception
    {
    public:
        ModelError(const std::string &message) : _msg(message)
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