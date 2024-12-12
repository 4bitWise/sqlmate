#ifndef SQLMATEDB_H
#define SQLMATEDB_H

#include <iostream>
#include <unordered_map>
#include "sqlite3.hpp"
// #include <mysqlx/xdevapi.h>

namespace sqlmate {
	class Database {
	public:
        Database(Database const&)        = delete;
        void operator=(Database const&)  = delete;

		static Database& getInstance()
        { static Database instance; return instance; };

		SQLite connect(const std::string& dbName);

		void close(SQLite& s);

		std::string execute(const std::string& query);
	private:
        Database() = default;

		void parseConnectionString(const std::string& s);

		std::unordered_map<std::string, std::string> _connectionInfos;
        bool _connected = false;
  };
}

#endif