#include "Request.hpp"
#include "../Utility/RequestConstructor.hpp"
void Request::_initialize(mg_connection *nc, int ev, void *p)
{
	http_message *hm = (http_message*)p;
	// method
	method = RequestConstructor::determineMethod(hm->method);
	// path
	path = string(hm->uri.p, hm->uri.len);
	// body
	body = string(hm->body.p, hm->body.len);
	// headers
	RequestConstructor::buildHeaders(headers, hm->header_names, hm->header_values);
	// cookies
	RequestConstructor::parseCookies(cookies, headers);
	// query
	RequestConstructor::parseQuery(query, hm->query_string);
	// hostname
	hostname = RequestConstructor::getHostname(headers);
	// ip
	ip = RequestConstructor::getIp(nc);
	// xhr
	xhr = RequestConstructor::determineXhr(headers);
}
bool Request::accepts(const string &type)
{
	return false;
}
bool Request::accepts(const std::vector<string> &types)
{
	return false;
}
bool Request::acceptsCharset(const string &charset)
{
	return false;
}
bool Request::acceptsCharsets(const std::vector<string> &charsets)
{
	return false;
}
bool Request::acceptsEncoding(const string &encoding)
{
	return false;
}
bool Request::acceptsEncodings(const std::vector<string> &encodings)
{
	return false;
}
bool Request::acceptsLanguage(const string &language)
{
	return false;
}
bool Request::acceptsLanguages(const std::vector<string> &languages)
{
	return false;
}
bool Request::is(const string &type)
{
	return false;
}
string Request::get(const string &field)
{
	return "";
}
string Request::param(const string &name)
{
	return "";
}
