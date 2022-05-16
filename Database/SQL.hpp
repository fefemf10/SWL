#pragma once
#pragma warning(disable: 4996)

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <array>
#include <mysql.h>

namespace db
{
	class SQL
	{
	public:
		SQL();
		~SQL();
		unsigned int connect(const std::string& user, const std::string& passsword, const std::string& host, const unsigned short& port);
		unsigned int selectDatabase(const std::string& database);
		unsigned int disconnect();
		unsigned int query(const std::string& query);
		unsigned int createDatabase(const std::string& name);
		unsigned int createTable(const std::string& nameTable, const std::string& nameColumn, const std::string& type, const std::string& value, const std::vector<std::string>& attributes);
		unsigned int createColumn(const std::string& nameTable, const std::string& nameColumn, const std::string& type, const std::string& value, const std::vector<std::string>& attributes);
		unsigned int modifyColumn(const std::string& nameTable, const std::string& nameColumn, const std::string& type, const std::string& value, const std::vector<std::string>& attributes);
		unsigned int insertValues(const std::string& nameTable, const std::vector<std::string>& nameColumns, const std::vector<std::vector<std::string>>& values);
		std::vector<std::vector<std::string>> selectValues(const std::string& nameTable, const std::vector<std::string>& nameColumns, const std::string& condition);
		unsigned int updateValues(const std::string& nameTable, const std::vector<std::string>& nameColumns, const std::vector<std::string>& values, const std::string& condition);
		unsigned int deleteValues(const std::string& nameTable, const std::string& condition);
		unsigned int deleteDatabase(const std::string& name);
		unsigned int deleteTable(const std::string& nameTable);
		unsigned int deleteColumn(const std::string& nameTable, const std::string& nameColumn);
		std::string getError();
	private:
		MYSQL* connection;
		std::string error;
	};
}