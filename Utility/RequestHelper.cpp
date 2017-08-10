#include <cstring>
#include "RequestHelper.hpp"
#include "../Utility/Utility.hpp"
using scaf::UrlEncode;
using scaf::UrlDecode;
bool RequestHelper::determineXhr(std::map<string, string> &headers)
{
	auto xhr = headers.find("X-Requested-With");
	if(xhr == headers.end())
		return false;
	return xhr->second == "XMLHttpRequest";
}
HttpMethod RequestHelper::determineMethod(mg_str mg_method)
{
	auto ret = HttpMethod::NOTIMPLEMENTED;
	string method(mg_method.p, mg_method.len);
	if(method == "GET")
		ret = HttpMethod::GET;
	else if(method == "POST")
		ret = HttpMethod::POST;
	else if(method == "HEAD")
		ret = HttpMethod::HEAD;
	else if(method == "PUT")
		ret = HttpMethod::PUT;
	else if(method == "DELETE")
		ret = HttpMethod::DELETE;
	else if(method == "TRACE")
		ret = HttpMethod::TRACE;
	else if(method == "OPTIONS")
		ret = HttpMethod::OPTIONS;
	else if(method == "CONNECT")
		ret = HttpMethod::CONNECT;
	return ret;
}
void RequestHelper::buildHeaders(std::map<string, string> &headers,
                                      mg_str *k, mg_str *v)
{
	for(int i = 0; k[i].p != nullptr && i < MG_MAX_HTTP_HEADERS; ++i)
	{
		auto key = string(k[i].p, k[i].len);
		auto f = headers.find(key);
		if(f != headers.end())
			f->second += "," + string(v[i].p, v[i].len);
		else
			headers.insert(std::make_pair(key, string(v[i].p, v[i].len)));
	}
}
void RequestHelper::parseCookies(std::map<string, string> &cookies,
                                      const std::map<string, string> &headers)
{
	auto f = headers.find("Cookie");
	if(f == headers.end())
		return;
	auto &s = f->second;
	// Start
	string k, v;
	for(auto sit = s.cbegin(); sit != s.cend(); ++sit)
	{
		// Clean
		k.clear(), v.clear();
		// Skip some characters
		if(*sit == ' ' || *sit == ';')
			continue;
		// Read key
		while(*sit != '=' && sit < s.cend())
			k.push_back(*sit), ++sit;
		if(sit >= s.cend())
			break;
		++sit;
		// Read value
		while(*sit != ';' && sit < s.cend())
			v.push_back(*sit), ++sit;
		// Escape
		k = UrlDecode(k);
		v = UrlDecode(v);
		cookies[k] = v;
	}
}
void RequestHelper::parseQuery(std::map<string, string> &query, mg_str q)
{
	int i = 0, len = (int)q.len;
	const char *s = q.p;
	string k, v;
	for( ; i < len; ++i)
	{
		k.clear(), v.clear();
		// Skip some characters
		if(s[i] == ' ' || s[i] == '&')
			continue;
		// Read key
		while(s[i] != '=' && i < len)
			k.push_back(s[i++]);
		if(i >= len)
			break;
		++i;
		// Read value
		while(s[i] != '&' && i < len)
			v.push_back(s[i++]);
		// Process escape sequence
		k = UrlDecode(k);
		v = UrlDecode(v);
		query[k] = v;
	}
}
string RequestHelper::getHostname(std::map<string, string> &headers)
{
	string ret;
	auto f = headers.find("Host");
	if(f == headers.end())
		return "";
	auto &s = f->second;
	int i = (int)s.length() - 1;
	while(s[i] != ':' && i >= 0)
		--i;
	return i == 0 ? s : string(s, 0, (unsigned long)(i + 1));
}
string RequestHelper::getIp(mg_connection *conn)
{
	char buff[64];
	mg_conn_addr_to_str(conn, buff, 63, MG_SOCK_STRINGIFY_IP | MG_SOCK_STRINGIFY_REMOTE);
	return string(buff);
}
void RequestHelper::parseParams(std::map<string, string> &params, string path, string pattern)
{
	// Todo
}
