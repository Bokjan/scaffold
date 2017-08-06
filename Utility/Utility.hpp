#ifndef SCAFFOLD_UTILITY_HPP
#define SCAFFOLD_UTILITY_HPP
#include <map>
#include <ctime>
#include <string>
using std::string;
const extern std::map<int, const char * const> HttpStatusDesc;
const extern std::map<const char * const, const char * const> MimeTable;
bool CompareStringsCaseInsensitive(const char *, const char *);
string UtilUrlEncode(const string&);
time_t HttpDateStringToTimestamp(string);
time_t HttpDateStringToTimestamp(const char *);
string TimestampToHttpDateString(time_t);
#endif //SCAFFOLD_UTILITY_HPP
