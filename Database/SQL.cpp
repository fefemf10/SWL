#include "SQL.hpp"
namespace db
{
	SQL::SQL()
	{
		connection = mysql_init(nullptr);
		mysql_set_character_set(connection, "utf8");
	}

	SQL::~SQL()
	{
		mysql_close(connection);
	}

	unsigned int SQL::connect(const std::string& user, const std::string& passsword, const std::string& host, const unsigned short& port)
	{
		mysql_real_connect(connection, host.c_str(), user.c_str(), passsword.c_str(), nullptr, port, nullptr, 0);
		error = mysql_error(connection);
		return mysql_errno(connection);
	}

	unsigned int SQL::selectDatabase(const std::string& database)
	{
		mysql_select_db(connection, database.c_str());
		error = mysql_error(connection);
		return mysql_errno(connection);
	}

	unsigned int SQL::disconnect()
	{
		mysql_close(connection);
	}

	unsigned int SQL::query(const std::string& query)
	{
		mysql_query(connection, query.c_str());
		error = mysql_error(connection);
		return mysql_errno(connection);
	}

	unsigned int SQL::createDatabase(const std::string& name)
	{
		mysql_query(connection, std::string("CREATE DATABASE "+  name).c_str());
		error = mysql_error(connection);
		return mysql_errno(connection);
	}

	unsigned int SQL::createTable(const std::string& nameTable, const std::string& nameColumn, const std::string& type, const std::string& value, const std::vector<std::string>& attributes)
	{
		std::stringstream attribute;
		for (size_t i = 0; i < attributes.size() - 1; i++)
		{
			attribute << attributes[i] << " ";
		}
		attribute << attributes[attributes.size() - 1];
		mysql_query(connection, std::string("CREATE TABLE " + nameTable + "(" + nameColumn + " " + type + "(" + value + ")" + attribute.str() + ") DEFAULT CHARSET utf8;").c_str());
		error = mysql_error(connection);
		return mysql_errno(connection);
	}

	unsigned int SQL::createColumn(const std::string& nameTable, const std::string& nameColumn, const std::string& type, const std::string& value, const std::vector<std::string>& attributes)
	{
		std::stringstream attribute;
		if (!attributes.empty())
		{
			for (size_t i = 0; i < attributes.size() - 1; i++)
			{
				attribute << attributes[i] << " ";
			}
			attribute << attributes[attributes.size() - 1];
		}
		mysql_query(connection, std::string("ALTER TABLE " + nameTable + "\nADD " + nameColumn + " " + type + "(" + value + ")" + attribute.str() + ";").c_str());
		error = mysql_error(connection);
		return mysql_errno(connection);
	}

	unsigned int SQL::modifyColumn(const std::string& nameTable, const std::string& nameColumn, const std::string& type, const std::string& value, const std::vector<std::string>& attributes)
	{
		std::stringstream attribute;
		if (!attributes.empty())
		{
			for (size_t i = 0; i < attributes.size() - 1; i++)
			{
				attribute << attributes[i] << " ";
			}
			attribute << attributes[attributes.size() - 1];
		}
		mysql_query(connection, std::string("ALTER TABLE " + nameTable + "\nMODIFY COLUMN " + nameColumn + " " + type + "(" + value + ")" + attribute.str() + ";").c_str());
	}

	unsigned int SQL::insertValues(const std::string& nameTable, const std::vector<std::string>& nameColumns, const std::vector<std::vector<std::string>>& values)
	{
		std::stringstream nameColumn, value;
		std::string a, b;
		if (!nameColumns.empty())
		{
			for (size_t i = 0; i < nameColumns.size() - 1; i++)
			{
				nameColumn << nameColumns[i] + ", ";
			}
			nameColumn << nameColumns[nameColumns.size() - 1];
		}
		if (!values.empty())
		{
			for (size_t i = 0; i < values.size(); i++)
			{
				value << "(";
				for (size_t j = 0; j < values[i].size() - 1; j++)
				{
					value << "'" << values[i][j] + "',";
				}
				value << "'" << values[i][values[i].size() - 1] << "'),";
			}
			a = nameColumn.str();
			b = value.str().replace(value.str().size() - 1, 1, ";");
			std::cout << std::string("INSERT " + nameTable + "(" + a + ")" + "VALUES" + b).c_str() << std::endl;
			mysql_query(connection, std::string("INSERT " + nameTable + "(" + a + ")" + "VALUES" + b).c_str());
		}
		else
		{
			std::cout << std::string("INSERT " + nameTable + "() VALUES()").c_str() << std::endl;
			mysql_query(connection, std::string("INSERT " + nameTable + "() VALUES()").c_str());
		}
		error = mysql_error(connection);
		return mysql_errno(connection);
	}

	std::vector<std::vector<std::string>> SQL::selectValues(const std::string& nameTable, const std::vector<std::string>& nameColumns, const std::string& condition)
	{
		std::stringstream value;
		std::string a;
		
		for (size_t i = 0; i < nameColumns.size(); i++)
		{
			value << nameColumns[i] << ",";
		}
		a = value.str();
		a.pop_back();
		if (!condition.empty())
			mysql_query(connection, std::string("SELECT " + a + " FROM " + nameTable + "\nWHERE " + condition + ";").c_str());
		else
			mysql_query(connection, std::string("SELECT " + a + " FROM " + nameTable + ";").c_str());
		std::cout << mysql_error(connection) << std::endl;

		MYSQL_ROW row;
		MYSQL_RES* res = mysql_store_result(connection);
		if (res == nullptr) return {};
		size_t j{ 0 };
		std::vector<std::vector<std::string>> result(res->row_count, std::vector<std::string>(res->field_count));
		while (row = mysql_fetch_row(res))
		{
			for (size_t i = 0; i < mysql_num_fields(res); i++)
			{
				result[j][i] = std::string(row[i] ? row[i] : "NULL");
			}
			j++;
		}
		return result;
	}

	unsigned int SQL::updateValues(const std::string& nameTable, const std::vector<std::string>& nameColumns, const std::vector<std::string>& values, const std::string& condition)
	{
		std::stringstream value;
		std::string a;
		for (size_t i = 0; i < nameColumns.size(); i++)
		{
			value <<nameColumns[i] << "='" << values[i] << "',";
		}
		if (!condition.empty())
		{
			a = value.str();
			a.pop_back();
			mysql_query(connection, std::string("UPDATE " + nameTable + "\nSET " + a + "\nWHERE " + condition + ";").c_str());
		}
		else
		{
			a = value.str().replace(value.str().size() - 1, 1, ";");
			mysql_query(connection, std::string("UPDATE " + nameTable + "\nSET " + a).c_str());
		}
		error = mysql_error(connection);
		return mysql_errno(connection);
	}

	unsigned int SQL::deleteValues(const std::string& nameTable, const std::string& condition)
	{
		mysql_query(connection, std::string("DELETE FROM " + nameTable + "\nWHERE " + condition + ";").c_str());
		error = mysql_error(connection);
		return mysql_errno(connection);
	}

	unsigned int SQL::deleteDatabase(const std::string& name)
	{
		mysql_query(connection, std::string("DROP DATABASE " + name).c_str());
		error = mysql_error(connection);
		return mysql_errno(connection);
	}

	unsigned int SQL::deleteTable(const std::string& nameTable)
	{
		mysql_query(connection, std::string("DROP TABLE " + nameTable).c_str());
		error = mysql_error(connection);
		return mysql_errno(connection);
	}

	unsigned int SQL::deleteColumn(const std::string& nameTable, const std::string& nameColumn)
	{
		mysql_query(connection, std::string("ALTER TABLE " + nameTable + "\nDROP COLUMN " + nameColumn).c_str());
		error = mysql_error(connection);
		return mysql_errno(connection);
	}
	std::string SQL::getError()
	{
		return error;
	}
}