#include <iostream>
#include "database.hpp"

int main(void)
{
    sqlmate::Database& db = sqlmate::Database::getInstance();
    
    db.connect("mysql://root:password?ip=127.0.0.1:3306&database_name=testdb");

    return EXIT_SUCCESS;
}