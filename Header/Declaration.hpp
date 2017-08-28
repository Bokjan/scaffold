#ifndef SCAFFOLD_DECLARATION_HPP
#define SCAFFOLD_DECLARATION_HPP
#include <ctime>
#include <string>
#include <functional>
using std::string;
struct mg_mgr;
struct mg_connection;
struct http_message;
class Request;
class Response;
using callback_t = std::function<void(Request&, Response&)>;
enum class HttpMethod : int
{
	GET = 0,
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
