#ifndef SQLMATEDB_H
#define SQLMATEDB_H

#include <iostream>
#include <unordered_map>

namespace sqlmate {
	class Database {
	public:
        Database(Database const&)        = delete;
        void operator=(Database const&)  = delete;

		static Database& getInstance()
        { static Database instance; return instance; };

		void connect(const std::string& s)
		{
			parseConnectionString(s);

			for (auto v: _connectionInfos) {
				std::cout << v.first << ' ' << v.second << std::endl;
			}
			
			return;
		};

		void close(void);

		std::string& execute(const std::string& query);
	private:
        Database() = default;

		void parseConnectionString(const std::string& s);

		std::unordered_map<std::string, std::string> _connectionInfos;
        bool _connected = false;
  };
}

#endif