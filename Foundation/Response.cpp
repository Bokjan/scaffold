#include <cctype>
#include <cstring>
#include <algorithm>
#include "Response.hpp"
#include "mongoose/mongoose.h"
#include "../Utility/Utility.hpp"
#include "../Utility/ResponseHelper.hpp"
Response::Response(mg_connection *c):
	conn(c), statusCode(200), _headersSent(false),
	_contentEnded(false)
{
	headers.insert(std::make_pair("Content-Type", "text/html"));
}
Response::~Response(void)
{
	if(!_contentEnded)
		end();
}
bool Response::headersSent(void)
{
	return _headersSent;
}
void Response::type(const string &type)
{
	if(type.find('/') != string::npos)
		headers["Content-Type"] = type;
	else
		headers["Content-Type"] = ResponseHelper::mimeLookup(type);
}
string Response::get(const string &key)
{
	// First, case-sensitively search
	auto f = headers.find(key);
	if(f != headers.end())
		return f->second;
	string t = key;
	std::transform(t.begin(), t.end(), t.begin(), (int (*)(int))tolower);
	for(auto &i : headers)
	{
		string copy = i.first;
		std::transform(copy.begin(), copy.end(), copy.begin(), (int (*)(int))tolower);
		if(t == copy)
			return i.second;
	}
	return string();
}
void Response::set(const string &key, const string &value)
{
	headers[key] = value;
}
void Response::header(const string &key, const string &value)
{
	headers[key] = value;
}
//void Response::cookie(const string &name, const string &value)
//void Response::cookie(const string &name, const Cookie &cookie)
void Response::clearCookie(const string &name)
{
	auto f = cookies.find(name);
	if(f != cookies.end())
		cookies.erase(f);
}
//void Response::download(const string &file, string name = "")
//void Response::link(const string &rel, const string &link)
//void Response::location(const string &path)
//void Response::redirect(const string &path, int status = 302)
//void Response::render(const string &view, std::map<string, string> vars = {})
Response& Response::status(int code)
{
	statusCode = HttpStatusDesc.find(code)
	                != HttpStatusDesc.end()
	                ? code : 500;
	return *this;
}
Response* Response::status(int code, bool _x)
{
	return &status(code);
}
Response& Response::vary(const string &value)
{
	auto f = headers.find("Vary");
	if(f != headers.end())
		f->second += "," + value;
	else
		headers.insert(std::make_pair("Vary", value));
	return *this;
}
Response* Response::vary(const string &value, bool _x)
{
	return &vary(value);
}
//void Response::sendStatus(int code)
void Response::send(const string &content)
{
	send(content.c_str(), (int)content.length());
}
void Response::send(const char *content, int len)
{
	if(_contentEnded)
		return;
	if(len < 0)
		len = (int)strlen(content);
	if(!_headersSent)
		sendHeader();
	mg_send_http_chunk(conn, content, (size_t)len);
}
const static char STATIC_HEADERS[] =
	"X-Powered-By: scaffold\r\n"
	"Transfer-Encoding: chunked\r\n";
void Response::sendHeader(void) // Transfer-Encoding: chunked
{
	if(_headersSent)
		return;
	// Send response line, `Server` and `Transfer-Encoding`
	mg_printf(conn, "HTTP/1.1 %d %s\r\nServer: %s\r\n",
		statusCode, HttpStatusDesc.at(statusCode),	"Mongoose/" MG_VERSION);
	mg_send(conn, STATIC_HEADERS, sizeof(STATIC_HEADERS) - 1);
	string buff; // Extra headers' buffer
	// Expand headers
	for(const auto &i : headers)
		buff += i.first + ": " + i.second + "\r\n";
	// Todo: expand cookies

	// Add CR LF
	buff += "\r\n";
	mg_send(conn, buff.c_str(), (int)buff.length());
	_headersSent = true;
}
void Response::end(void)
{
	// Send a zero-length chunk
	mg_send(conn, "0\r\n\r\n", 5);
	_contentEnded = true;
}
