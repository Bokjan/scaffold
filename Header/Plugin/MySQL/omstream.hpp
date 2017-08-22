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
		omysqlstream(void):
			tied(false), copied(false),
			index(0), pstmt(nullptr) { }
		omysqlstream(sql::PreparedStatement* ps)
		{
			tie(ps);
		}
		omysqlstream(omysqlstream &src):
			tied(src.tied), copied(false),
			index(src.index), pstmt(src.pstmt)
		{
			src.copied = true;
		}
		omysqlstream(omysqlstream &&src):
			tied(src.tied), copied(false),
			index(src.index), pstmt(src.pstmt)
		{
			src.copied = true;
		}
		~omysqlstream(void)
		{
			untie();
		}
		sql::PreparedStatement& ref(void)
		{
			return *pstmt;
		}
		sql::PreparedStatement* ptr(void)
		{
			return pstmt;
		}
		void tie(sql::PreparedStatement* ps)
		{
			if(tied)
				untie();
			tied = true;
			pstmt = ps;
			index = 1;
		}
		void untie(void)
		{
			if(pstmt != nullptr && !copied)
				delete pstmt;
			tied = false;
			pstmt = nullptr;
			index = 0;
		}
		unsigned int getIndex(void)
		{
			return index;
		}
		unsigned int increaseIndex(void)
		{
			return index++;
		}
		void clear(void)
		{
			index = 1;
		}
		void set(const sql::SQLString &s, MysqlType t = MysqlType::String)
		{
			switch(t)
			{
				case MysqlType::String:
					pstmt->setString(index++, s);
					break;
				case MysqlType::DateTime:
					pstmt->setDateTime(index++, s);
					break;
				case MysqlType::BigInt:
					pstmt->setBigInt(index++, s);
					break;
				default:;
			}
		}
		void set(const char *s, MysqlType t = MysqlType::String)
		{
			set(sql::SQLString(s), t);
		}
		void set(std::istream &blob)
		{
			set(&blob);
		}
		void set(std::istream *is)
		{
			if(is == nullptr)
			{
				setNull();
				return;
			}
			pstmt->setBlob(index++, is);
		}
		void setNull(void)
		{
			pstmt->setNull(index++, sql::DataType::SQLNULL);
		}
		void set(bool boolean)
		{
			pstmt->setBoolean(index++, boolean);
		}
		void set(double dbl)
		{
			pstmt->setDouble(index++, dbl);
		}
		void set(int32_t i32)
		{
			pstmt->setInt(index++, i32);
		}
		void set(uint32_t ui32)
		{
			pstmt->setUInt(index++, ui32);
		}
		void set(int64_t i64)
		{
			pstmt->setInt64(index++, i64);
		}
		void set(uint64_t ui64)
		{
			pstmt->setUInt64(index++, ui64);
		}
	};
	template <typename T>
	omysqlstream& operator << (omysqlstream &os, T value)
	{
		os.set(value);
		return os;
	}
	omysqlstream& operator << (omysqlstream &os, const char *str)
	{
		os.set(sql::SQLString(str));
		return os;
	}
	omysqlstream& operator << (omysqlstream &os, std::nullptr_t np)
	{
		os.setNull();
		return os;
	}
}
#endif //SCAFFOLD_OMSTREAM_HPP
