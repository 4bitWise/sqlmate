
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
        AModel(std::shared_ptr<IDatabase> db) : _db(db), _tableCreated(false)
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

    protected:
        std::unordered_map<std::string, FieldInfo> fields;
        std::shared_ptr<IDatabase> _db;
        bool _tableCreated;
        int _id;

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
                _db->exec(query, [&](void *data, int argc, char **argv, char **azColName)
                          { std::cout << "table created!!!!!" << std::endl;
                            return _createTableCallBack(data, argc, argv, azColName); });
                _tableCreated = true;
            }
        }

        /**
         * @brief Callback function for table creation queries.
         * 
         * Prints query results and logs information about the table creation process.
         * 
         * @param data User-provided data pointer.
         * @param argc Number of columns in the result set.
         * @param argv Array of column values.
         * @param azColName Array of column names.
         * @return Always returns 0.
         */
        int _createTableCallBack(void *data, int argc, char **argv, char **azColName)
        {
            int i;
            std::cerr << (const char *)data << std::endl;

            for (i = 0; i < argc; i++)
            {
                std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << std::endl;
            }
            std::cout << std::endl;
            return (0);
        }
    };
} // namespace sqlmate
