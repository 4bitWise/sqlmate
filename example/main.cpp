#include <iostream>
#include "../sqlmate/database/database.hpp"

int main() {
    sqlmate::Database& db = sqlmate::Database::getInstance();
    SQLite testdb = db.connect("testdb.db");
    std::string testQuery("INSERT INTO PERSON VALUES(1, 'STEVE', 'GATES', 30, 'PALO ALTO', 1000.0);"
               "INSERT INTO PERSON VALUES(2, 'BILL', 'ALLEN', 20, 'SEATTLE', 300.22);"
               "INSERT INTO PERSON VALUES(3, 'PAUL', 'JOBS', 24, 'SEATTLE', 9900.0);"); 

    db.execute(testdb, "CREATE TABLE IF NOT EXISTS PERSON("
                      "ID INT PRIMARY KEY     NOT NULL, "
                      "NAME           TEXT    NOT NULL, "
                      "SURNAME          TEXT     NOT NULL, "
                      "AGE            INT     NOT NULL, "
                      "ADDRESS        CHAR(50), "
                      "SALARY         REAL );");
    
    db.execute(testdb, testQuery);
    db.execute(testdb, "SELECT * FROM PERSON");
    db.close(testdb);

    return EXIT_SUCCESS;
}