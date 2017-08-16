#include <memory>
#include <vector>
#include <sstream>
#include <utility>
#include <stdexcept>
#include "Plugin/MySQL.hpp"
using scaf::MySQL;
MySQL::MySQL(const string &host, int port, const string &user, const string &pass)
{
	std::ostringstream oss;
	oss << "tcp://" << host << ':' << port;
	driver = sql::mysql::get_mysql_driver_instance();
	conn = driver->connect(oss.str().c_str(), user.c_str(), pass.c_str());
}
MySQL::~MySQL(void)
{
	delete conn;
}
void MySQL::ensureConnectionAlive(void)
{
	if(!conn->isValid())
	{
		conn->reconnect();
		if(!conn->isValid())
			throw std::runtime_error("failed to establish MySQL connection");
	}
}
sql::Connection* MySQL::getConnection(void)
{
	return conn;
}
bool MySQL::execute(const string &sql)
{
	ensureConnectionAlive();
	auto stmt = conn->createStatement();
	bool ret = stmt->execute(sql);
	delete stmt;
	return ret;
}
MySQL* MySQL::use(const string &t)
{
	std::ostringstream oss;
	oss << "USE " << t << ';';
	execute(oss.str());
	return this;
}
sql::ResultSet* MySQL::executeQuery(const string &sql)
{
	ensureConnectionAlive();
	sql::Statement* stmt = conn->createStatement();
	auto ret = stmt->executeQuery(sql);
	delete stmt;
	return ret;
}
sql::PreparedStatement* MySQL::prepare(const string &psql)
{
	ensureConnectionAlive();
	return conn->prepareStatement(psql);
}
