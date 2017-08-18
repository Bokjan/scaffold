#ifndef SCAFFOLD_OMSTREAM_HPP
#define SCAFFOLD_OMSTREAM_HPP
#include <istream>
#include "MySQL.hpp"
#include <cppconn/datatype.h>
namespace scaf
{
	class imysqlstream;
	class omysqlstream // Insert data to a PreparedStatement
	{
	private:
		bool copied;
		bool initialized;
		unsigned int index;
		sql::PreparedStatement *pstmt;
		friend class MySQL;
	public:
		omysqlstream(void):
			copied(false), initialized(false),
			index(0), pstmt(nullptr) { }
		omysqlstream(sql::PreparedStatement* ps)
		{
			tie(ps);
		}
		omysqlstream(omysqlstream &src):
				copied(false), initialized(src.initialized),
				index(src.index), pstmt(src.pstmt)
		{
			src.copied = true;
		}
		omysqlstream(omysqlstream &&src):
				copied(false), initialized(src.initialized),
				index(src.index), pstmt(src.pstmt)
		{
			src.copied = true;
		}
		~omysqlstream(void)
		{
			untie();
		}
		sql::PreparedStatement* getStatement(void)
		{
			return pstmt;
		}
		unsigned int getIndex(void)
		{
			return index;
		}
		unsigned int increaseIndex(void)
		{
			return index++;
		}
		void tie(sql::PreparedStatement* ps)
		{
			if(initialized)
				untie();
			initialized = true;
			pstmt = ps;
			index = 1;
		}
		void untie(void)
		{
			if(pstmt != nullptr && !copied)
				delete pstmt;
			initialized = false;
			pstmt = nullptr;
			index = 0;
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
	omysqlstream& operator << (omysqlstream &os, const sql::SQLString &str)
	{
		os.set(str);
		return os;
	}
	omysqlstream& operator << (omysqlstream &os, const char *str)
	{
		os.set(sql::SQLString(str));
		return os;
	}
	omysqlstream& operator << (omysqlstream &os, std::istream &blob)
	{
		os.set(blob);
		return os;
	}
	omysqlstream& operator << (omysqlstream &os, bool boolean)
	{
		os.set(boolean);
		return os;
	}
	omysqlstream& operator << (omysqlstream &os, double dbl)
	{
		os.set(dbl);
		return os;
	}
	omysqlstream& operator << (omysqlstream &os, int32_t i32)
	{
		os.set(i32);
		return os;
	}
	omysqlstream& operator << (omysqlstream &os, uint32_t ui32)
	{
		os.set(ui32);
		return os;
	}
	omysqlstream& operator << (omysqlstream &os, int64_t i64)
	{
		os.set(i64);
		return os;
	}
	omysqlstream& operator << (omysqlstream &os, uint64_t ui64)
	{
		os.set(ui64);
		return os;
	}
	omysqlstream& operator << (omysqlstream &os, std::nullptr_t np)
	{
		os.setNull();
		return os;
	}
}
#endif //SCAFFOLD_OMSTREAM_HPP
