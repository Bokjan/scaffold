#include "Plugin/MySQL/omstream.hpp"
using namespace scaf;
omysqlstream::omysqlstream(void):
	tied(false), copied(false),
	index(0), pstmt(nullptr) { }
omysqlstream::omysqlstream(sql::PreparedStatement* ps):
	tied(false), copied(false)
{
	tie(ps);
}
omysqlstream::omysqlstream(omysqlstream &src):
	tied(src.tied), copied(false),
	index(src.index), pstmt(src.pstmt)
{
	src.copied = true;
}
omysqlstream::omysqlstream(omysqlstream &&src):
	tied(src.tied), copied(false),
	index(src.index), pstmt(src.pstmt)
{
	src.copied = true;
}
omysqlstream::~omysqlstream(void)
{
	untie();
}
sql::PreparedStatement& omysqlstream::ref(void)
{
	return *pstmt;
}
sql::PreparedStatement* omysqlstream::ptr(void)
{
	return pstmt;
}
void omysqlstream::tie(sql::PreparedStatement* ps)
{
	if(tied)
		untie();
	tied = true;
	pstmt = ps;
	index = 1;
}
void omysqlstream::untie(void)
{
	if(pstmt != nullptr && !copied)
		delete pstmt;
	tied = false;
	pstmt = nullptr;
	index = 0;
}
unsigned int omysqlstream::getIndex(void)
{
	return index;
}
unsigned int omysqlstream::increaseIndex(void)
{
	return index++;
}
void omysqlstream::clear(void)
{
	index = 1;
}
void omysqlstream::set(const sql::SQLString &s, MysqlType t)
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
void omysqlstream::set(const char *s, MysqlType t)
{
	set(sql::SQLString(s), t);
}
void omysqlstream::set(std::istream &blob)
{
	set(&blob);
}
void omysqlstream::set(std::istream *is)
{
	if(is == nullptr)
	{
		setNull();
		return;
	}
	pstmt->setBlob(index++, is);
}
void omysqlstream::setNull(void)
{
	pstmt->setNull(index++, sql::DataType::SQLNULL);
}
void omysqlstream::set(bool boolean)
{
	pstmt->setBoolean(index++, boolean);
}
void omysqlstream::set(double dbl)
{
	pstmt->setDouble(index++, dbl);
}
void omysqlstream::set(int32_t i32)
{
	pstmt->setInt(index++, i32);
}
void omysqlstream::set(uint32_t ui32)
{
	pstmt->setUInt(index++, ui32);
}
void omysqlstream::set(int64_t i64)
{
	pstmt->setInt64(index++, i64);
}
void omysqlstream::set(uint64_t ui64)
{
	pstmt->setUInt64(index++, ui64);
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
