#ifndef SCAFFOLD_OMSTREAM_HPP
#define SCAFFOLD_OMSTREAM_HPP
#include <istream>
#include "MySQL.hpp"
#include <cppconn/datatype.h>
namespace scaf
{
	class omysqlstream // Insert data to a PreparedStatement
	{
	private:
		bool tied;
		bool copied;
		unsigned int index;
		sql::PreparedStatement *pstmt;
		friend class MySQL;
	public:
		omysqlstream(void);
		omysqlstream(sql::PreparedStatement* ps);
		omysqlstream(omysqlstream &src);
		omysqlstream(omysqlstream &&src);
		~omysqlstream(void);
		sql::PreparedStatement& ref(void);
		sql::PreparedStatement* ptr(void);
		void tie(sql::PreparedStatement* ps);
		void untie(void);
		unsigned int getIndex(void);
		unsigned int increaseIndex(void);
		void clear(void);
		void preventDeleting(void);
		void set(const sql::SQLString &s, MysqlType t = MysqlType::String);
		void set(const char *s, MysqlType t = MysqlType::String);
		void set(std::istream &blob);
		void set(std::istream *is);
		void setNull(void);
		void set(bool boolean);
		void set(double dbl);
		void set(int32_t i32);
		void set(uint32_t ui32);
		void set(int64_t i64);
		void set(uint64_t ui64);
	};
}
template <typename T>
scaf::omysqlstream& operator << (scaf::omysqlstream &os, T value)
{
	os.set(value);
	return os;
}
scaf::omysqlstream& operator << (scaf::omysqlstream &os, const char *str);
scaf::omysqlstream& operator << (scaf::omysqlstream &os, std::nullptr_t np);
#endif //SCAFFOLD_OMSTREAM_HPP
