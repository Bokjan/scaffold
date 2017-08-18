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
	enum class MysqlType
	{
		BigInt,     // string
		Blob,       // std::istream*
		Boolean,    // bool
		DateTime,   // string
		Double,     // double
		Int,        // int32_t
		UInt,       // uint32_t
		Int64,      // int64_t
		UInt64,     // uint64_t
		Null,       // sql::DataType::SQLNULL
		String      // string
	};
	// Forward declarations
	class imysqlstream;
	class omysqlstream;
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
		MySQL* use(const string &t); // USE `db`
		bool execute(const string &sql);
		imysqlstream execute(omysqlstream &os);
		imysqlstream executeQuery(const string &sql);
		omysqlstream prepare(const string &psql);
	};
}
#endif //SCAFFOLD_MYSQL_HPP
