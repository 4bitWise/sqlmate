# Project Documentation: sqlmate

Welcome to the documentation of **sqlmate**, a C++ library designed to simplify interactions with SQLite databases. This library encapsulates SQLite functionality and provides a set of tools to map objects to SQLite tables efficiently.

## Overview

**sqlmate** is a C++ library that abstracts SQLite database operations into high-level classes and functions. The library offers a simple and intuitive interface to perform standard database operations such as creating tables, inserting and retrieving data, and managing the schema. The library also provides support for building SQL queries dynamically through the `QueryBuilder` class.

The core components of the library are:

- **IDatabase**: An abstract class for defining database interaction (e.g., connecting, executing queries, etc.).
- **SQLite**: An implementation of the `IDatabase` interface using SQLite3 to manage a database connection and perform SQL operations.
- **QueryBuilder**: A helper class that generates SQL queries (e.g., `CREATE TABLE`, `INSERT`, `SELECT`, etc.) dynamically based on model data.
- **AModel**: A base class for creating model classes that represent tables in the SQLite database.
- **DatabaseManager**: A singleton class that manages multiple database connections.

## Features

- **Database Management**: Easily connect to SQLite databases, check connection status, and perform operations.
- **Dynamic Query Building**: The `QueryBuilder` class dynamically generates SQL queries based on provided data.
- **Model-Based**: The `AModel` class allows you to define fields and manage table creation and data insertion through an object-oriented approach.
- **Singleton Design**: The `DatabaseManager` class manages multiple database connections using the singleton pattern.

## Installation

To integrate **sqlmate** into your project, follow these steps:

1. Clone or download the repository.
2. Include the necessary header files in your project.
3. Link against the SQLite3 library if necessary.
4. Include the `sqlmate.hpp` header to access the main API.

## Usage

### Connecting to a Database

```
#include "sqlmate/Database/DatabaseManager.hpp"
std::shared_ptr<sqlmate::IDatabase> db = sqlmate::DatabaseManager::getInstance().connect("my_database.db", sqlmate::DatabaseType::SQLITE);
```