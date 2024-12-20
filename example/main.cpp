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
    user.setName("Bob");

    user.save();
    user.setAge(23);
    user.save();

    User user2(db);

    user2.setAge(24);
    user2.setName("Alice");

    user2.save();

    std::shared_ptr<User> result = user2.findOne<User>(1);
    if (result)
        std::cout << "Result: " << result->getAge() << std::endl;
    else
        std::cout << "Not found User with _id = 1" << std::endl;

    std::cout << std::endl;

    std::vector<std::shared_ptr<User>> users = user2.findAll<User>();

    for (auto user : users)
    {
        std::cout << "Id: " << user->getId() << std::endl;
        std::cout << "Name: " << user->getName() << std::endl;
        std::cout << "Age: " << user->getAge() << std::endl;
        std::cout << std::endl;
    }
    DatabaseManager::getInstance().disconnect(dbUrl);
    return EXIT_SUCCESS;
}