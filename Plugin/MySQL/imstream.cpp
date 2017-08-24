#include "Plugin/MySQL/imstream.hpp"
using namespace scaf;
imysqlstream::imysqlstream(void):
	tied(false), copied(false),
	rs(nullptr) { }
imysqlstream::imysqlstream(sql::ResultSet* rs):
	tied(false), copied(false)
{
	tie(rs);
}
imysqlstream::imysqlstream(imysqlstream &src):
	tied(src.tied), copied(false),
	index(src.index), rs(src.rs)
{
	src.copied = true;
}
imysqlstream::imysqlstream(imysqlstream &&src):
	tied(src.tied), copied(false),
	index(src.index), rs(src.rs)
{
	src.copied = true;
}
imysqlstream::~imysqlstream(void)
{
	untie();
}
sql::ResultSet& imysqlstream::ref(void)
{
	return *rs;
}
sql::ResultSet* imysqlstream::ptr(void)
{
	return rs;
}
void imysqlstream::tie(sql::ResultSet* _rs)
{
	if(tied)
		untie();
	rs = _rs;
	tied = true;
}
void imysqlstream::untie(void)
{
	if(rs != nullptr && !copied)
		delete rs;
	tied = false;
	rs = nullptr;
}
bool imysqlstream::startRow(void)
{
	if(!rs->next())
		return false;
	index = 1;
	return true;
}
void imysqlstream::skip(void)
{
	++index;
}
size_t imysqlstream::rowsCount(void)
{
	return rs->rowsCount();
}
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
