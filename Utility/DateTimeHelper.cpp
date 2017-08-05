#include <cstdio>
#include <cstring>
#include "Utility.hpp"
static int StringMonthToInt(const char *m)
{
	int ret = 1;
	if(strcmp(m, "Feb") == 0)
		ret = 2;
	else if(strcmp(m, "Mar") == 0)
		ret = 3;
	else if(strcmp(m, "Apr") == 0)
		ret = 4;
	else if(strcmp(m, "May") == 0)
		ret = 5;
	else if(strcmp(m, "Jun") == 0)
		ret = 6;
	else if(strcmp(m, "Jul") == 0)
		ret = 7;
	else if(strcmp(m, "Aug") == 0)
		ret = 8;
	else if(strcmp(m, "Sep") == 0)
		ret = 9;
	else if(strcmp(m, "Oct") == 0)
		ret = 10;
	else if(strcmp(m, "Nov") == 0)
		ret = 11;
	else if(strcmp(m, "Dec") == 0)
		ret = 12;
	return ret;
}
time_t HttpDateStringToTimestamp(string str)
{
	return HttpDateStringToTimestamp(str.c_str());
}
time_t HttpDateStringToTimestamp(const char *str)
{
	// RFC 7231, Page 64
	// Example: Sun, 06 Nov 1994 08:49:37 GMT
	int y, m, d, h, i, s;
	char month[32];
	sscanf(str + 5, "%d %s %d %d:%d:%d", &d, month, &y, &h, &i, &s);
	m = StringMonthToInt(month) - 1;
	y -= 1900;
	tm stm =
	{
		.tm_year = y,
		.tm_mon  = m,
		.tm_mday = d,
		.tm_hour = h,
		.tm_min  = i,
		.tm_sec  = s
	};
	return mktime(&stm);
};
string TimestampToHttpDateString(time_t time)
{
	char buff[256];
	tm *stm = gmtime(&time);
	strftime(buff, 256, "%a, %d %b %Y %T", stm);
	return string(buff);
}
