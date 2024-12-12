#include <iostream>
#include "../sqlmate/database/database.hpp"

int main() {
    sqlmate::Database& db = sqlmate::Database::getInstance();
    SQLite testdb = db.connect("testdb.db");
    
    db.close(testdb );
    // try {
    // db.connect("mysqlx://mike:s3cr3t!@localhost:13009");
    // } catch (const mysqlx::Error &e) {
    //     std::cout << "ERROR: " << e << std::endl;
    //     return 1;
    // } catch (std::exception &e) {
    //     std::cout << "STD EXCEPTION: " << e.what() << std::endl;
    //     return 1;
    // }

    return EXIT_SUCCESS;
}