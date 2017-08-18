#include <vector>
#include <sstream>
#include <utility>
#include <stdexcept>
#include "Plugin/MySQL/MySQL.hpp"
#include "Plugin/MySQL/mstream.hpp"
using scaf::MySQL;
using scaf::imysqlstream;
using scaf::omysqlstream;
MySQL::MySQL(const string &host, int port, const string &user, const string &pass)
{
	driver = sql::mysql::get_mysql_driver_instance();
	sql::ConnectOptionsMap map;
	map["hostName"] = host;
	map["userName"] = user;
	map["password"] = pass;
	map["port"]     = port;
	conn = driver->connect(map);
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
imysqlstream MySQL::executeQuery(const string &sql)
{
	ensureConnectionAlive();
	sql::Statement* stmt = conn->createStatement();
	auto ims = imysqlstream(stmt->executeQuery(sql));
	delete stmt;
	return ims;
}
omysqlstream MySQL::prepare(const string &psql)
{
	ensureConnectionAlive();
	return omysqlstream(conn->prepareStatement(psql));
}
imysqlstream MySQL::execute(omysqlstream &os)
{
	ensureConnectionAlive();
	return imysqlstream(os.pstmt->executeQuery());
}
