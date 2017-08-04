#include <map>
#include <cctype>
#include <algorithm>
#include "Utility.hpp"
#include "ResponseHelper.hpp"
string ResponseHelper::mimeLookup(string t)
{
	std::transform(t.begin(), t.end(), t.begin(), (int (*)(int))tolower);
	for(auto &i : MimeTable)
		if(strcmp(i.first, t.c_str()) == 0)
			return i.second;
	return "text/plain";
}
