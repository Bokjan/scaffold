#include <cctype>
#include <algorithm>
#include "Response.hpp"
#include "../Utility/Utility.hpp"
#include "../Utility/ResponseHelper.hpp"
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
	return "";
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
	if(HttpStatusDesc.find(code) != HttpStatusDesc.end())
		statusCode = code;
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
//void Response::send(const string &value)
//void Response::end(void)
