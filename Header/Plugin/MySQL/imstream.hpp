#ifndef SCAFFOLD_IMSTREAM_HPP
#define SCAFFOLD_IMSTREAM_HPP
#include "MySQL.hpp"
namespace scaf
{
	class imysqlstream // Extract data from ResultSet
	{
	private:
		sql::ResultSet *rs;
	public:
		imysqlstream(void):
			rs(nullptr) { }
		imysqlstream(sql::ResultSet* rs):
			rs(rs) { }
		~imysqlstream(void)
		{
			if(rs != nullptr)
				delete rs;
		}
	};
}
#endif //SCAFFOLD_IMSTREAM_HPP
