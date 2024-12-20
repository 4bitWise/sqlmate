#include <iostream>
#include <sqlmate.hpp>

using namespace sqlmate;

class User : public sqlmate::AModel
{

public:
    TABLE_NAME("UserTable")
    User(std::shared_ptr<IDatabase> db) : AModel(db)
    {
        FIELDS(FIELD(name, "pseudo"),
               FIELD(age))
    }

    // setters
    void setAge(int value)
    {
        age = value;
    }

    void setName(const std::string &value)
    {
        name = value;
    }

    // getters
    int getAge()
    {
        return (age);
    }

    std::string getName()
    {
        return (name);
    }

private:
    std::string name;
    int age;
};

int main()
{
    std::string dbUrl = "userDatabase.db";
    auto db = DatabaseManager::getInstance().connect(dbUrl, DatabaseType::SQLITE);

    User user(db);

    user.setAge(22);
    user.setName("Georges");

    user.save();
    user.setAge(23);
    user.save();
    user.remove();

    DatabaseManager::getInstance().disconnect(dbUrl);
    return EXIT_SUCCESS;
}