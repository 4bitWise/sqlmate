
/**
 * @file AModel.hpp
 * @brief Defines the abstract base class for models in the sqlmate namespace.
 */

#include "../Database/IDatabase.hpp"
#include "./IModel.hpp"
#include "./decorators.hpp"

#include <cxxabi.h>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <functional>

#pragma once

namespace sqlmate
{
    /**
     * @class AModel
     * @brief Abstract base class for database models.
     * 
     * Provides common functionality for database models, such as table management
     * and saving/removing records. Derived classes represent specific database entities.
     */
    class AModel : public IModel
    {
    public:
        /**
         * @brief Constructs an `AModel` with a shared database instance.
         * 
         * @param db A shared pointer to an `IDatabase` instance.
         */
        AModel(std::shared_ptr<IDatabase> db) : _db(db), _tableCreated(false), _id(nextID++)
        {
            FIELDS(FIELD(_id))
        }

        /**
         * @brief Virtual destructor for proper cleanup of derived classes.
         */
        virtual ~AModel() {}

        /**
         * @brief Gets the default table name.
         * 
         * The default table name is derived from the class name.
         * 
         * @return The table name as a string.
         */
        virtual std::string getTableName() const override
        {
            return abi::__cxa_demangle(typeid(*this).name(), 0, 0, 0);
        }

        /**
         * @brief Saves the current model instance to the database.
         * 
         * If the table does not exist, it is created. The model's fields are then inserted
         * into the table.
         * 
         * @throw DatabaseError If the save operation fails.
         */
        void save() override
        {
            _createTableIfNotExists();

            std::string query = _db->qbuilder->insertQuery(getTableName(), fields);
            _db->exec(query, nullptr);
        }

        /**
         * @brief Removes the current model instance from the database.
         * 
         * If the table does not exist, it is created. The model instance is then removed
         * using its `_id` field as the primary key.
         * 
         * @throw DatabaseError If the remove operation fails.
         */
        void remove() override
        {
            _createTableIfNotExists();

            std::string query = _db->qbuilder->deleteQuery(getTableName(), _id);
            _db->exec(query, nullptr);
        }

        template <typename T>
        std::shared_ptr<T> findOne(int id)
        {

            static_assert(std::is_base_of<AModel, T>::value, "type parameter of this class must derive from AModel");
            std::shared_ptr<T> model = nullptr;
            std::string query = _db->qbuilder->selectQuery(getTableName(), "_id == " + std::to_string(id), 1);

            QueryCallBackWrapper cb([&](int argc, char **argv, char **azColName) -> int
                                    {
                        model = std::make_shared<T>(_db);
                        for (int i = 0; i < argc; i++) {
                            // std::cout << i << std::endl;
                            // std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
                            if (argv[i])
                                model->updateField(azColName[i], argv[i]);
                        }
                        return 0; });
            _db->exec(query, &cb);
            return (model);
        }

        template <typename T>
        std::vector<std::shared_ptr<T>> findAll()
        {
            static_assert(std::is_base_of<AModel, T>::value, "type parameter of this class must derive from AModel");
            std::vector<std::shared_ptr<T>> models;
            std::string query = _db->qbuilder->selectQuery(getTableName());

            QueryCallBackWrapper cb([&](int argc, char **argv, char **azColName) -> int
                                    {
                        models.push_back(std::make_shared<T>(_db));
                        for (int i = 0; i < argc; i++) {
                            // std::cout << i << std::endl;
                            // std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
                            if (argv[i])
                                models.back()->updateField(azColName[i], argv[i]);
                        }
                        return 0; });
            _db->exec(query, &cb);
            return (models);
        }

        int getId()
        {
            return (_id);
        }

    protected:
        std::unordered_map<std::string, FieldInfo> fields;
        std::shared_ptr<IDatabase> _db;
        bool _tableCreated;
        int _id;
        static int nextID;

        /**
         * @brief Ensures the table exists by creating it if it does not already exist.
         * 
         * This method is called before performing operations like saving or removing records.
         * 
         * @throw DatabaseError If the table creation query fails.
         */
        void _createTableIfNotExists()
        {
            if (!_tableCreated)
            {
                std::string query = _db->qbuilder->createTableQuery(getTableName(), fields);

                _db->exec(query, nullptr);
                _tableCreated = true;
            }
        }

        template <typename T>
        void _parseStr(std::string str, T &value)
        {
            std::stringstream stream(str);

            if constexpr (std::is_same<T, bool>::value)
                stream >> std::boolalpha >> value;
            else
                stream >> value;
            if (stream.fail())
                throw ModelError("Error parsing value :" + str);
        }

        void updateField(const std::string &key, const std::string &input)
        {

            if (fields.find(key) == fields.end())
                throw ModelError("Error parsing key :" + key);

            if (fields.at(key).typeId == typeid(int))
            {
                int &value = std::any_cast<std::reference_wrapper<int>>(fields.at(key).value).get();
                _parseStr<int>(input, value);
            }
            else if (fields.at(key).typeId == typeid(double))
            {
                double &value = std::any_cast<std::reference_wrapper<double>>(fields.at(key).value).get();
                _parseStr<double>(input, value);
            }
            else if (fields.at(key).typeId == typeid(std::string))
            {
                std::string &value = std::any_cast<std::reference_wrapper<std::string>>(fields.at(key).value).get();
                _parseStr<std::string>(input, value);
            }
            else if (fields.at(key).typeId == typeid(bool))
            {
                bool &value = std::any_cast<std::reference_wrapper<bool>>(fields.at(key).value).get();
                _parseStr<bool>(input, value);
            }
            else
                throw ModelError("Unsupported type for value formatting");
        }
    };

    int AModel::nextID = 1;
} // namespace sqlmate
