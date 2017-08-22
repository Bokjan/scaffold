#ifndef SCAFFOLD_RESPONSE_HPP
#define SCAFFOLD_RESPONSE_HPP
#include <map>
#include <vector>
#include "Declaration.hpp"
class Request;
struct http_message;
struct mg_connection;
class Response
{
private:
	Request         *req;
	http_message    *hm;
	mg_connection   *conn;

	int  statusCode;
	bool _typeSet;
	bool _headersSent;
	bool _contentEnded;
	std::map<string, Cookie> cookies;
	std::map<string, string> headers;

	void sendHeader(void);
	string expandHeader(void);
public:
	Response(void) = delete;
	Response(Request *r, http_message *h, mg_connection *c);
	~Response(void);
	bool headersSent(void);
	void type(const string &type);
	string get(const string &key);
	void set(const string &key, const string &value);
	void header(const string &key, const string &value);
	void crossOrigin(const string &dest);
	void cookie(const string &name, const string &value, bool forever = false);
	void cookie(const string &name, Cookie &_cookie);
	void clearCookie(const string &name);
	void serveFile(const string &file);
	void download(const string &file, const string &name);
	void link(const string &rel, const string &link);
	void location(string path);
	void redirect(string location, int status = 302);
	Response& status(int code);
	Response* status(int code, bool);
	Response& vary(const string &value);
	Response* vary(const string &value, bool);
	void sendStatus(int code);
	void send(const string &content);
	void send(const char *content, int len = -1);
	void end(void);
};
#endif //SCAFFOLD_RESPONSE_HPP
