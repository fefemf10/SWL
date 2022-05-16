#include <iostream>
#include <vector>
#include <string>
#include <locale.h>
#include "SQL.hpp"

void PrintDoubleVector(const std::vector<std::vector<std::string>>& mas)
{
	for (int i = 0; i < mas.size(); i++)
	{
		for (int j = 0; j < mas[i].size(); j++)
		{
			std::cout << mas[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

int main()
{
	db::SQL database;
	database.connect("gb_x_lolola32", "55b2zzada", "mysql101.1gb.ru", 3306);
	std::cout << database.deleteDatabase("gb_x_lolola32") << " " << database.getError() << std::endl;
	std::cout << database.selectDatabase("gb_x_lolola32") << " " << database.getError() << std::endl;
	//database.createDatabase("lol");
	//database.createTable("test", "id", "INT", "0", { "PRIMARY KEY", "AUTO_INCREMENT" });
	//database.createColumn("test", "text", "VARCHAR", "30", {});
	//database.insertValues("test", { "id", "text" }, { { "10", "as" }, { "5", "jsdlf" }, { "8", "asdasd" } });
	//database.updateValues("test", { "text" }, { "dsfjl" }, "");
	//database.deleteValues("test", "id = 5");
	std::vector<std::vector<std::string>> table(std::move(database.selectValues("test", { "text", "id" }, "id = 8")));
	PrintDoubleVector(std::move(database.selectValues("test", { "*" }, "")));
	PrintDoubleVector(table);
}