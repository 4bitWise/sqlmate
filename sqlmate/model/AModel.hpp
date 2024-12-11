#include "./IModel.hpp"
#include "./decorators.hpp"

#include <cxxabi.h>
#include <any>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <functional>

namespace sqlmate {
struct FieldInfo
{
    FieldInfo(std::any val, std::type_index tId) : value(val), typeId(tId) {}

    std::any value;
    std::type_index typeId;
};


#if !defined(AMODEL)
#define AMODEL

// Classe modèle de base pour gérer le nom par défaut
class AModel : public IModel
{
public:
    AModel();
    ~AModel();

    // Nom par défaut de la table (nom de la classe)
    virtual std::string getTableName() const override;

protected:
    std::unordered_map<std::string, FieldInfo> fields;
};

#endif // AMODEL

}
