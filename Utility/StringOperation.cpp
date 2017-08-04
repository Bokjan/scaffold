#include <cctype>
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
