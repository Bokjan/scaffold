#include <cctype>
#include <string>
using std::string;
bool CompareStringsCaseInsensitive(const char *x, const char *y)
{
	for( ; ;)
	{
		if(tolower(*x) != tolower(*y))
			return false;
		if(*x == '\0')
			return true;
		++x, ++y;
	}
}
string UtilUrlEncode(const string &s)
{
	const static char dont_escape[] = "._-$,;~()/";
	const static char hex[] = "0123456789ABCDEF";
	string ret;
	for(const auto i : s)
		if(isalnum(s[i]) ||
			strchr(dont_escape, s[i]) != nullptr)
			ret.push_back(i);
		else
		{
			ret.push_back('%');
			ret.push_back(hex[s[i] >> 4]);
			ret.push_back(hex[s[i] & 0xF]);
		}
	return ret;
}
