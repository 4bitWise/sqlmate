// Macro pour définir un nom de table personnalisé
#define TABLE_NAME(name)                      \
public:                                       \
    std::string getTableName() const override \
    {                                         \
        return name;                          \
    }


// Macro pour déclarer les champs
#define FIELD_HELPER(_1, _2, NAME, ...) NAME

#define FIELD_1(variable) this->fields.insert({#variable, FieldInfo(std::ref(this->variable), std::type_index(typeid(this->variable)))})

#define FIELD_2(variable, name) this->fields.insert({name, FieldInfo(std::ref(this->variable), std::type_index(typeid(this->variable)))})

#define FIELD_ERROR(...) _Static_assert(0, "FIELD doit avoir 1 ou 2 arguments")

#define FIELD(...) FIELD_HELPER(__VA_ARGS__, FIELD_2, FIELD_1,  FIELD_ERROR)(__VA_ARGS__)

#define FIELDS(...)                                  \
    {                                                \
        __VA_ARGS__;                                 \
    }
