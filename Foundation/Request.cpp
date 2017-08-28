#include "Request.hpp"
#include "../Utility/Utility.hpp"
#include "../Utility/RequestHelper.hpp"
void Request::_initialize(mg_connection *nc, int ev, void *p)
{
	hm = (http_message*)p;
	// conn
	conn = nc;
	// method
	method = RequestHelper::determineMethod(hm->method);
	// path
	path = scaf::UrlDecode(string(hm->uri.p, hm->uri.len));
	// body
	body = string(hm->body.p, hm->body.len);
	// headers
	RequestHelper::buildHeaders(headers, hm->header_names, hm->header_values);
	// cookies
	RequestHelper::parseCookies(cookies, headers);
	// query
	RequestHelper::parseQuery(query, hm->query_string);
	// hostname
	hostname = RequestHelper::getHostname(headers);
	// ip
	ip = RequestHelper::getIp(nc);
	// xhr
	xhr = RequestHelper::determineXhr(headers);
	// post (application/x-www-form-urlencoded)
	RequestHelper::parseWwwFormEncodedPostBody(post, body);
}
bool Request::is(const string &type)
{
	auto f = headers.find("Content-Type");
	if(f == headers.end())
		return false;
	auto &s = f->second;
	return strstr(s.c_str(), type.c_str()) != nullptr;
}
string Request::get(const string &field)
{
	auto f = headers.find(field);
	if(f != headers.end())
		return f->second;
	for(auto &i : headers)
		if(scaf::CompareStringsCaseInsensitive(field.c_str(), i.first.c_str()))
			return i.second;
	return string();
}
