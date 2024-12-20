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

        void save() const override
        {
            // TODO create table if not exists on each command
        }

        void remove() const override
        {

            // TODO create table if not exists on each command
        }

    protected:
        std::unordered_map<std::string, FieldInfo> fields;
        std::shared_ptr<IDatabase> _db;
        bool _tableCreated;
        int _id;

    private:
        void _createTableIfNotExists()
        {
            _db->qbuilder->createTableQuery(getTableName(), fields);
            _tableCreated = true;
        }
    };

}
