#ifndef SCAFFOLD_UTILITY_HPP
#define SCAFFOLD_UTILITY_HPP
#include <map>
#include <string>
using std::string;
const extern std::map<int, string> HttpStatusDesc;
const extern std::map<const char * const, const char * const> MimeTable;
bool CompareStringsCaseInsensitive(const char *, const char *);
#endif //SCAFFOLD_UTILITY_HPP
