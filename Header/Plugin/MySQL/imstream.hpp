#ifndef SCAFFOLD_IMSTREAM_HPP
#define SCAFFOLD_IMSTREAM_HPP
#include <istream>
#include "MySQL.hpp"
namespace scaf
{
	class imysqlstream // Extract data from ResultSet
	{
	public:
		bool tied;
		bool copied;
		unsigned int index;
		sql::ResultSet *rs;
		imysqlstream(void);
		imysqlstream(sql::ResultSet* rs);
		imysqlstream(imysqlstream &src);
		imysqlstream(imysqlstream &&src);
		~imysqlstream(void);
		sql::ResultSet& ref(void);
		sql::ResultSet* ptr(void);
		void tie(sql::ResultSet* _rs);
		void untie(void);
		bool startRow(void);
		void skip(void);
		size_t rowsCount(void);
		void preventDeleting(void);
	};
}
scaf::imysqlstream& operator >> (scaf::imysqlstream &is, std::istream*& istream);
scaf::imysqlstream& operator >> (scaf::imysqlstream &is, bool &boolean);
scaf::imysqlstream& operator >> (scaf::imysqlstream &is, double &dbl);
scaf::imysqlstream& operator >> (scaf::imysqlstream &is, int32_t &i32);
scaf::imysqlstream& operator >> (scaf::imysqlstream &is, uint32_t &ui32);
scaf::imysqlstream& operator >> (scaf::imysqlstream &is, int64_t &i64);
scaf::imysqlstream& operator >> (scaf::imysqlstream &is, uint64_t &ui64);
scaf::imysqlstream& operator >> (scaf::imysqlstream &is, string &str);
#endif //SCAFFOLD_IMSTREAM_HPP
