#ifndef SCAFFOLD_UTILITY_HPP
#define SCAFFOLD_UTILITY_HPP
#include <map>
#include <ctime>
#include <string>
using std::string;
namespace scaf
{
	const extern std::map<int, const char * const> HttpStatusDesc;
	const extern std::map<const char * const, const char * const> MimeTable;
	bool CompareStringsCaseInsensitive(const char *, const char *);
	string UrlEncode(const string &);
	string UrlDecode(const string &, bool formUrlEncoded = false);
	time_t HttpDateStringToTimestamp(string);
	time_t HttpDateStringToTimestamp(const char *);
	string TimestampToHttpDateString(time_t);
	string SHA256(const void *_data, size_t len, bool upper = false);
	bool FileExists(const char *);
	bool FileExists(const string &);
	bool DirectoryExists(const char *);
	bool DirectoryExists(const string &);
}
#endif //SCAFFOLD_UTILITY_HPP
