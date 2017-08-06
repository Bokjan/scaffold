#ifndef SCAFFOLD_DECLARATION_HPP
#define SCAFFOLD_DECLARATION_HPP
#include <ctime>
#include <string>
using std::string;
struct mg_mgr;
struct mg_connection;
enum class HttpMethod
{
	GET,
	HEAD,
	POST,
	PUT,
	DELETE,
	TRACE,
	OPTIONS,
	CONNECT,
	NOTIMPLEMENTED
};
struct Cookie
{
	string  value;
	time_t  expires;
	int     maxAge;
	string  path;
	string  domain;
	bool    secure;
	bool    httpOnly;
	Cookie(void):
		expires(0), maxAge(0),
		secure(false), httpOnly(false) { }
};
#endif //SCAFFOLD_DECLARATION_HPP
