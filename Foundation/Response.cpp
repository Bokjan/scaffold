#include <cctype>
#include <cstring>
#include <algorithm>
#include "Request.hpp"
#include "Response.hpp"
#include "Scaffold.hpp"
#include "mongoose/mongoose.h"
#include "../Utility/Utility.hpp"
#include "../Utility/ResponseHelper.hpp"
using namespace scaf;
Response::Response(Request *r, http_message *h, mg_connection *c):
	req(r), hm(h), conn(c), statusCode(200),
	_typeSet(false), _headersSent(false), _contentEnded(false)
{
	headers.insert(std::make_pair("Content-Type", "text/html; charset=utf-8"));
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
	_typeSet = true;
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
void Response::crossOrigin(const string &dest)
{
	headers["Access-Control-Allow-Origin"] = dest;
}
void Response::cookie(const string &name, const string &value, bool forever)
{
	Cookie c;
	c.value = UrlEncode(value);
	if(forever)
		c.maxAge = 315360000;
	cookies[name] = c;
}
void Response::cookie(const string &name, Cookie &c)
{
	c.value     = UrlEncode(c.value);
	c.path      = UrlEncode(c.path);
	c.domain    = UrlEncode(c.domain);
	cookies[name] = c;
}
void Response::clearCookie(const string &name)
{
	auto f = cookies.find(name);
	if(f != cookies.end())
		cookies.erase(f);
}
void Response::download(const string &file, const string &name)
{
	// Specify the `Content-Disposition` header
	set("Content-Disposition", "filename=" + name);
	serveFile(file);
}
void Response::serveFile(const string &file)
{
	string mime;
	auto ct = headers.find("Content-Type");
	mime = ct != headers.end() ?
	       ct->second : "application/octet-stream";
	// Erase the `Content-Type`
	headers.erase(headers.find("Content-Type"));
	if(!_typeSet)
	{
		// Determine the suffix of the file
		auto i = file.length() - 1;
		for( ; i != 0; --i)
			if(file[i] == '.')
				break;
		// Get the MIME according to the suffix
		mime = ResponseHelper::mimeLookup(file.c_str() + i + 1);
	}
	auto extraHeaders = expandHeader();
	_contentEnded = true;
	mg_http_serve_file(conn, hm, file.c_str(), {mime.c_str(), mime.length()},
	                   {extraHeaders.c_str(), extraHeaders.length() - 2}); // Remove the CRLF
}
void Response::link(const string &rel, const string &link)
{
	string line = "<";
	line += UrlEncode(link) + ">; rel=\"" + UrlEncode(rel) + "\"";
	auto f = headers.find("Link");
	if(f == headers.end())
		headers.insert(std::make_pair("Link", line));
	else
		f->second += ", " + line;
}
void Response::location(string path)
{
	if(path == "back")
	{
		auto f = req->headers.find("Referer");
		path = f == req->headers.end() ? "/" : f->second;
	}
	set("Location", UrlEncode(path));
}
void Response::redirect(string location, int status)
{
	if(HttpStatusDesc.find(status) == HttpStatusDesc.end())
		status = 302;
	// A back redirection redirects the request
	// back to the referer, defaulting to / when the referer is missing.
	if(location == "back")
	{
		auto f = req->headers.find("Referer");
		location = f == req->headers.end() ? "/" : f->second;
	}
	location = UrlEncode(location);
	auto extraHeaders = expandHeader();
	_contentEnded = true;
	mg_http_send_redirect(conn, status, {location.c_str(), location.length()},
	                      {extraHeaders.c_str(), extraHeaders.length()});
}
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
void Response::sendStatus(int code)
{
	const auto &f = HttpStatusDesc.find(code);
	if(f != HttpStatusDesc.end())
		status(code).send(f->second);
}
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
	"X-Powered-By: scaffold " SCAFFOLD_VERSION_STRING "\r\n"
	"Transfer-Encoding: chunked\r\n";
void Response::sendHeader(void) // Transfer-Encoding: chunked
{
	if(_headersSent)
		return;
	_headersSent = true;
	// Send response line, `Server` and `Transfer-Encoding`
	mg_printf(conn, "HTTP/1.1 %d %s\r\nServer: %s\r\n",
		statusCode, HttpStatusDesc.at(statusCode),	"Mongoose/" MG_VERSION);
	mg_send(conn, STATIC_HEADERS, sizeof(STATIC_HEADERS) - 1);
	string buff = expandHeader() += "\r\n"; // Add CRLF
	mg_send(conn, buff.c_str(), (int)buff.length());
}
string Response::expandHeader(void)
{
	string buff; // Extra headers' buffer
	char cbuff[64];
	// Expand headers
	for(const auto &i : headers)
		buff += i.first + ": " + i.second + "\r\n";
	// Expand cookies
	for(const auto &i : cookies)
	{
		buff += "Set-Cookie: " + i.first + "=" + i.second.value;
		if(i.second.expires != 0 && i.second.maxAge == 0)
			buff += "; expires=",
			buff += scaf::TimestampToHttpDateString(i.second.expires);
		if(i.second.maxAge != 0)
			sprintf(cbuff, "%d", i.second.maxAge),
			buff += "; Max-Age=", buff += cbuff;
		if(!i.second.path.empty())
			buff += "; path=" + i.second.path;
		if(!i.second.domain.empty())
			buff += "; domain=" + i.second.path;
		if(i.second.secure)
			buff += "; secure";
		if(i.second.httpOnly)
			buff += "; HttpOnly";
		buff += "\r\n";
	}
	return buff;
}
void Response::end(void)
{
	_contentEnded = true;
	// Send a zero-length chunk
	mg_send(conn, "0\r\n\r\n", 5);
}
