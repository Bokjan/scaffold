#ifndef SCAFFOLD_MYSQL_HPP
#define SCAFFOLD_MYSQL_HPP
#include <string>
#include <mysql_driver.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
using std::string;
namespace scaf
{
	class MySQL
	{
	private:
		sql::Connection *conn;
		sql::mysql::MySQL_Driver *driver;

		void ensureConnectionAlive(void);
	public:
		MySQL(void) = delete;
		MySQL(MySQL&) = delete;
		MySQL(const string &host, int port, const string &user, const string &pass);
		~MySQL(void);
		sql::Connection* getConnection(void);
		bool execute(const string &sql);
		MySQL* use(const string &t); // USE `db`
		sql::ResultSet* executeQuery(const string &sql);
		sql::PreparedStatement* prepare(const string &psql);
	};
}
#endif //SCAFFOLD_MYSQL_HPP
