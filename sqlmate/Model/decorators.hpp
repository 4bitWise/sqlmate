/**
 * @file decorators.hpp
 * @brief project decorators/macros definitions.
 */

namespace sqlmate
{

/**
 * @brief Macro to define a custom table name.
 * 
 * This macro defines the `getTableName()` method to return a specific table name.
 * It allows customizing the table name for each model without additional code.
 *
 * @param name The name of the table to return.
 */
#define TABLE_NAME(name)                      \
public:                                       \
    /**                                       \
     * @brief Returns the table name.         \
     *                                        \
     * @return The name of the table.         \
     */                                       \
    std::string getTableName() const override \
    {                                         \
        return name;                          \
    }

/**
 * @brief Internal macro to determine which version of the `FIELD` macro to use.
 * 
 * This macro helps determine whether a field is defined with one or two arguments.
 * It is used to select the correct version of `FIELD_1` or `FIELD_2` based on the number of arguments.
 */
#define FIELD_HELPER(_1, _2, NAME, ...) NAME

/**
 * @brief Macro to declare a field with a single argument (variable).
 * 
 * This macro inserts a field into the table using the given variable and its type.
 * The field name is the name of the variable.
 *
 * @param variable The variable to add as a field.
 */
#define FIELD_1(variable) this->fields.insert({#variable, FieldInfo(std::ref(this->variable), std::type_index(typeid(this->variable)))})

/**
 * @brief Macro to declare a field with a custom name.
 * 
 * This macro inserts a field into the table using the given variable and its type,
 * while allowing you to specify a custom name for the field.
 *
 * @param variable The variable to add as a field.
 * @param name The custom name for the field.
 */
#define FIELD_2(variable, name) this->fields.insert({name, FieldInfo(std::ref(this->variable), std::type_index(typeid(this->variable)))})

/**
 * @brief Macro to handle errors when `FIELD` is given an incorrect number of arguments.
 * 
 * This macro triggers a compile-time error if `FIELD` is called with an invalid number of arguments.
 * It ensures that `FIELD` is called with either one or two arguments.
 */
#define FIELD_ERROR(...) _Static_assert(0, "FIELD doit avoir 1 ou 2 arguments")

 /**
 * @brief Macro to declare a field in a table.
 * 
 * This macro allows you to declare a field in a table. It accepts either one argument (the variable)
 * or two arguments (the variable and a name). If an incorrect number of arguments is given, a compile-time error is triggered.
 *
 * @param ... The arguments to pass to the macro, either a variable alone or a variable and a name.
 */
#define FIELD(...) FIELD_HELPER(__VA_ARGS__, FIELD_2, FIELD_1, FIELD_ERROR)(__VA_ARGS__)

/**
 * @brief Macro to declare multiple fields in a table.
 * 
 * This macro allows you to define multiple fields at once by listing them as arguments.
 *
 * @param ... The fields to declare.
 */
#define FIELDS(...)  \
    {                \
        __VA_ARGS__; \
    }
}