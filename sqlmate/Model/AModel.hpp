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
        AModel(std::shared_ptr<IDatabase> db) : _db(db), _tableCreated(false), _id(nextID++)
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
}
