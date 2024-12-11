#ifndef SQLMATEDB_H
#define SQLMATEDB_H

#include <iostream>

namespace sqlmate {
	class Database {
	public:
        Database(Database const&)        = delete;
        void operator=(Database const&)  = delete;

		static Database& getInstance()
        { static Database instance; return instance; };

		void connect(const std::string& connectionString);

		void close(void);

		std::string& execute(const std::string& query);
	private:
        Database() = default;
        bool _connected = false;
  };
}

#endif