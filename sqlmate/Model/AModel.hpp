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

    // Classe modèle de base pour gérer le nom par défaut
    class AModel : public IModel
    {
    public:
        AModel(std::shared_ptr<IDatabase> db) : _db(db), _tableCreated(false)
        {
            FIELDS(FIELD(_id))
        }

        virtual ~AModel() {}

        // Nom par défaut de la table (nom de la classe)
        virtual std::string getTableName() const override
        {
            return abi::__cxa_demangle(typeid(*this).name(), 0, 0, 0);
        }

        void save() override
        {
            _createTableIfNotExists();
            std::string query = _db->qbuilder->insertQuery(getTableName(), fields);
            _db->exec(query, nullptr);
        }

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

}
