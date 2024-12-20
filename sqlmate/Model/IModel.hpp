/**
 * @file IModel.hpp
 * @brief Interface for model classes representing a database entity.
 */

#include <iostream>
#include <vector>

#pragma once

namespace sqlmate
{
    /**
     * @brief Interface for model classes representing a database entity.
     * 
     * This interface defines the essential methods that any model class must implement to interact
     * with the underlying database. It includes methods for table management
     */
    class IModel
    {
    public:
        /**
         * @brief Virtual destructor for proper cleanup of derived classes.
         */
        virtual ~IModel() = default;

        /**
         * @brief Gets the name of the database table associated with the model.
         * 
         * This method returns the name of the table that the model class represents. The table name 
         * could be dynamically generated or fixed depending on the implementation.
         * 
         * @return The name of the database table.
         */
        virtual std::string getTableName() const = 0;

        /**
         * @brief Saves the current model to the database.
         * 
         * This method persists the current state of the model object in the database. If the model
         * doesn't exist, it should be inserted; otherwise, it should update the existing record.
         */
        virtual void save() = 0;

        /**
         * @brief Removes the current model from the database.
         * 
         * This method removes the model from the database. It should be implemented to handle 
         * deleting the record from the table where the model is stored.
         */
        virtual void remove() = 0;

        /**
         * @brief TODO Add createTable and deleteTable methods.
         * 
         * This is a placeholder comment suggesting the implementation of methods for creating
         * and deleting the table associated with the model. These methods could be useful for 
         * initializing or cleaning up the database schema.
         */
    };
} // namespace sqlmate
