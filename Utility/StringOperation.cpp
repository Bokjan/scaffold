#include <cctype>
#include <string>
#include <cstring>
#include "Utility.hpp"
using std::string;
bool scaf::CompareStringsCaseInsensitive(const char *x, const char *y)
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
string scaf::UrlEncode(const string &s)
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
const static char _hexMap[] =
		"000000000000000000000000000000000000000000000000" // 0~48
				"\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9"
				"0000000"
				"\xA\xB\xC\xD\xE\xF"
				"00000000000000000000000000"
				"\xA\xB\xC\xD\xE\xF";
inline bool ISXDIGIT(char x)
{
	return _hexMap[(int)x] != '0';
}
string scaf::UrlDecode(const string &s, bool formUrlEncoded)
{
	string ret;
	auto len = s.length();
	for(decltype(len) i = 0; i < len; ++i)
	{
		if(s[i] == '+' && formUrlEncoded)
			ret.push_back(' ');
		else if(s[i] == '%')
		{
			if(i >= len - 2 || !ISXDIGIT(s[i + 1]) || !ISXDIGIT(s[i + 2]))
				break;
			ret.push_back((_hexMap[(int)s[i + 1]] << 4) + _hexMap[(int)s[i + 2]]);
			i += 2;
		}
		else
			ret.push_back(s[i]);
	}
	return ret;
}
string scaf::GetFileSuffix(const string &s)
{
	if(s[0] == '.') // Dot file
		goto RETURN;
	for(auto i = s.length() - 1; i != 0; --i)
		if(s[i] == '.')
			return s.substr(i + 1);
RETURN:
	return string();
}
