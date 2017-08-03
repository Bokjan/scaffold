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
	ERROR
};
struct Cookie
{
	string  value;
	string  domain;
	time_t  expires;
	int     maxAge;
	bool    secure;
	bool    http;
	string  path;
	// TODO: Default values
};
#endif //SCAFFOLD_DECLARATION_HPP
