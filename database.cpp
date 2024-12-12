#include <iostream>
#include "database.hpp"

namespace sqlmate {
    // void Database::connect(const std::string& s)
    // {
    //     if (!_connected) {
    //         std::cout << "[INFO]: Connecting to database..." << std::endl;
    //         _connected = true;
    //     }
    // }

    void Database::close(void)
    {
        std::cout << "[INFO]: Disconnecting..." << std::endl;
    }

    std::string& Database::execute(const std::string& q)
    {
        std::cout << "Executing query: " << q << std::endl;
    }
    
    void Database::parseConnectionString(const std::string& cs)
    {
        size_t protocolPos = cs.find("://");
        size_t authStart;
        size_t authEnd;
        size_t colonPos;
        size_t ipPos;
        size_t dbNamePos;
        std::string authPart;
        std::string queryPart;
        
        if (protocolPos == std::string::npos) {
            throw std::invalid_argument("Invalid connection string format: Missing protocol");
        }
        _connectionInfos["db"] = cs.substr(0, protocolPos);
        
        // Username & password
        authStart = protocolPos + 3;
        authEnd = cs.find('?', authStart);
        if (authEnd == std::string::npos) {
            throw std::invalid_argument("Invalid connection string format: Missing '?' separator");
        }
        authPart = cs.substr(authStart, authEnd - authStart);
        colonPos = authPart.find(':');
        if (colonPos == std::string::npos) {
            throw std::invalid_argument("Invalid connection string format: Missing ':' in auth part");
        }
        _connectionInfos["username"] = authPart.substr(0, colonPos);
        _connectionInfos["password"] = authPart.substr(colonPos + 1);

        // Paramètres de la requête
        queryPart = cs.substr(authEnd + 1);
        ipPos = queryPart.find("ip=");
        dbNamePos = queryPart.find("&database_name=");

        if (ipPos == std::string::npos || dbNamePos == std::string::npos) {
            throw std::invalid_argument("Invalid connection string format: Missing ip or database_name");
        }
        _connectionInfos["host"] = queryPart.substr(ipPos + 3, dbNamePos - ipPos - 3);
        _connectionInfos["db_name"] = queryPart.substr(dbNamePos + 15);
    }
}