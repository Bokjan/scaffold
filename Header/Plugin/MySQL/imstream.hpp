#ifndef SCAFFOLD_IMSTREAM_HPP
#define SCAFFOLD_IMSTREAM_HPP
#include <istream>
#include "MySQL.hpp"
namespace scaf
{
	class imysqlstream // Extract data from ResultSet
	{
	private:
		bool tied;
		bool copied;
		unsigned int index;
		sql::ResultSet *rs;

		friend imysqlstream& operator >> (imysqlstream &is, string &str);
 		friend imysqlstream& operator >> (imysqlstream &is, double &dbl);
		friend imysqlstream& operator >> (imysqlstream &is, int32_t &i32);
		friend imysqlstream& operator >> (imysqlstream &is, int64_t &i64);
		friend imysqlstream& operator >> (imysqlstream &is, bool &boolean);
		friend imysqlstream& operator >> (imysqlstream &is, uint32_t &ui32);
		friend imysqlstream& operator >> (imysqlstream &is, uint64_t &ui64);
		friend imysqlstream& operator >> (imysqlstream &is, std::istream*& istream);
	public:
		imysqlstream(void):
			tied(false), copied(false),
			rs(nullptr) { }
		imysqlstream(sql::ResultSet* rs)
		{
			tie(rs);
		}
		imysqlstream(imysqlstream &src):
			tied(src.tied), copied(false),
			index(src.index), rs(src.rs)
		{
			src.copied = true;
		}
		imysqlstream(imysqlstream &&src):
			tied(src.tied), copied(false),
			index(src.index), rs(src.rs)
		{
			src.copied = true;
		}
		~imysqlstream(void)
		{
			untie();
		}
		sql::ResultSet& ref(void)
		{
			return *rs;
		}
		sql::ResultSet* ptr(void)
		{
			return rs;
		}
		void tie(sql::ResultSet* _rs)
		{
			if(tied)
				untie();
			rs = _rs;
			tied = true;
		}
		void untie(void)
		{
			if(rs != nullptr && !copied)
				delete rs;
			tied = false;
			rs = nullptr;
		}
		bool startRow(void)
		{
			if(!rs->next())
				return false;
			index = 1;
			return true;
		}
		void skip(void)
		{
			++index;
		}
		size_t rowsCount(void)
		{
			return rs->rowsCount();
		}
	};
	imysqlstream& operator >> (imysqlstream &is, std::istream*& istream)
	{
		istream = is.rs->getBlob(is.index++);
		return is;
	}
	imysqlstream& operator >> (imysqlstream &is, bool &boolean)
	{
		boolean = is.rs->getBoolean(is.index++);
		return is;
	}
	imysqlstream& operator >> (imysqlstream &is, double &dbl)
	{
		dbl = is.rs->getDouble(is.index++);
		return is;
	}
	imysqlstream& operator >> (imysqlstream &is, int32_t &i32)
	{
		i32 = is.rs->getInt(is.index++);
		return is;
	}
	imysqlstream& operator >> (imysqlstream &is, uint32_t &ui32)
	{
		ui32 = is.rs->getUInt(is.index++);
		return is;
	}
	imysqlstream& operator >> (imysqlstream &is, int64_t &i64)
	{
		i64 = is.rs->getInt64(is.index++);
		return is;
	}
	imysqlstream& operator >> (imysqlstream &is, uint64_t &ui64)
	{
		ui64 = is.rs->getUInt64(is.index++);
		return is;
	}
	imysqlstream& operator >> (imysqlstream &is, string &str)
	{
		str = is.rs->getString(is.index++);
		return is;
	}
}
#endif //SCAFFOLD_IMSTREAM_HPP
