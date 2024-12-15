#include <iostream>
#include "database.hpp"

static int callback(void* data, int argc, char** argv, char** azColName) 
{ 
    int i; 
    fprintf(stderr, "data: %s: ", (const char*)data); 
  
    for (i = 0; i < argc; i++) { 
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL"); 
    } 
  
    printf("\n");
    return 0;
}

namespace sqlmate {
    SQLite Database::connect(const std::string& dbfname)
    {
        SQLite s = SQLite(dbfname);
        
        try{
            std::cout << "[INFO]: Opening database " << dbfname
                << " ..." << std::endl;
            s.open();
            if (!_connected) {
                std::cout << "[INFO]: Connection succeeded !\n";
                _connected = true;
            }
        } catch (std::runtime_error& re) {
            std::cout << re.what() << std::endl;
        }
        return s;
    }

    void Database::close(SQLite& s)
    {
        std::cout << "[INFO]: Disconnecting..." << std::endl;
        s.close();
        _connected = false;
    }

    int Database::execute(SQLite& s, const std::string& q)
    {
        std::cout << "[INFO]: Executing query: " << q << std::endl;

        return (s.exec(q, callback)); // to allow users to see the response of their query;
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