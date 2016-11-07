#include "Database.h"
#include <iostream>

void DataType::setName(std::string name)
{
	for (int i(0); i < name.size(); i++)
	{
		if (name[i] == ' ')
		{
			std::cout << "\nError: data type names cannot contain spaces! Data type name: " + name;
			std::getchar();
			return;
		}
	}
	_name = name;
}
void DataType::setDataTypeSQL(std::string dataTypeSQL)
{
	_dataTypeSQL = dataTypeSQL;
}
static int staticCallback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i < argc; i++)
		std::cout << "\n" << azColName[i] << " = " << argv[i] ? argv[i] : "NULL";
	return 0;
}
int SQLCallbackHandler(void *NotUsed, int argc, char **argv, char **azColName)
{
	if (NotUsed)
		return ((Database*)NotUsed)->callback(argc, argv, azColName);
	return 0;
}










Database::Database() : _open(true)
{

}
Database::Database(std::string path) : Database()
{
	if (sqlite3_open(path.data(), &database))
	{
		_open = false;
		std::cout << "\nCan't open database: " << sqlite3_errmsg(database);
	}
	else
	{
		std::cout << "\nOpened database successfully";
	}
}
Database::~Database()
{
	if (_open)
		sqlite3_close(database);
}
bool Database::open(std::string path)
{
	if (sqlite3_open(path.data(), &database))
	{
		_open = false;
		std::cout << "\nCan't open database: " << sqlite3_errmsg(database);
		return false;
	}
	else
	{
		std::cout << "\nOpened database successfully";
		return true;
	}
}
bool Database::executeSQL(std::string msg, const bool _printVallback)
{
	if (!_open)
	{
		std::cout << "\nCannot execute SQL command! Database is not open!";
		return false;
	}
	if (sqlite3_exec(database, msg.data(), /*staticCallback*/SQLCallbackHandler, (void*)this, &error))
	{
		std::cout << "\nError: " << error;
		return false;
	}
	if (_printVallback)
		printCallback();
	return true;
}
int Database::callback(int _argc, char** _argv, char** _azColName)
{
	for (int i = 0; i < _argc; i++)
	{
		callbackColumns.push_back(std::pair<std::string, std::string>(_azColName[i], _argv[i] ? _argv[i] : "NULL"));
	}
	return 0;
}
void Database::printCallback()
{
	for (int i = 0; i < callbackColumns.size(); i++)
		std::cout << "\n" + callbackColumns[i].first + " = " + callbackColumns[i].second;
}