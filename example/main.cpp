#include <iostream>
#include <sqlmate.hpp>

using namespace sqlmate;

int main()
{
    std::string dbUrl = "mama";
    auto db = DatabaseManager::getInstance().connect(dbUrl, DatabaseType::SQLITE);

    std::cout << DatabaseManager::getInstance().isConnected(dbUrl) << std::endl;

    DatabaseManager::getInstance().disconnect(dbUrl);
    return EXIT_SUCCESS;
}