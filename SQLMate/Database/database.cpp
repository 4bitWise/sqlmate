#include <iostream>
#include "database.hpp"

namespace sqlmate {
    void Database::connect(const std::string& s)
    {
        if (!_connected)
        {
            std::cout << "[INFO]: Connecting to database..." << std::endl;
            _connected = true;
        }
    }

    void Database::close(void)
    {
        std::cout << "[INFO]: Disconnecting..." << std::endl;
    }

    std::string& Database::execute(const std::string& q)
    {
        std::cout << "Executing query: " << q << std::endl;
    }
}