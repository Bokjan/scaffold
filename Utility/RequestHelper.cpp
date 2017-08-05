#include <cstring>
#include "RequestHelper.hpp"
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
		k = convertUrlEscapeSequence(k);
		v = convertUrlEscapeSequence(v);
		query[k] = v;
	}
}
const static char _hexMap[] =
	"000000000000000000000000000000000000000000000000" // 0~48
	"\x0\x1\x2\x3\x4\x5\x6\x7\x8\x9"
	"0000000"
	"\xA\xB\xC\xD\xE\xF"
	"00000000000000000000000000"
	"\xA\xB\xC\xD\xE\xF";
string RequestHelper::convertUrlEscapeSequence(const string &s)
{
	string ret;
	auto len = (int)s.length();
	for(int i = 0; i < len; ++i)
	{
		if(s[i] == '+')
			ret.push_back(' ');
		else if(s[i] == '%')
		{
			if(i + 2 >= len)
				break;
			ret.push_back((_hexMap[(int)s[i + 1]] << 4) + _hexMap[(int)s[i + 2]]);
			i += 2;
		}
		else
			ret.push_back(s[i]);
	}
	return ret;
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
