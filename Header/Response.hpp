#ifndef SCAFFOLD_RESPONSE_HPP
#define SCAFFOLD_RESPONSE_HPP
#include <map>
#include <vector>
#include "Declaration.hpp"

class Response
{
private:
	std::map<string, Cookie> cookies;
	std::multimap<string, string> headers;
public:
	std::map<string, string> locals;

	bool headersSent(void);
	void type(const string &type);
	string get(const string &key);
	void set(const string &key, const string &value);
	void append(const string &key, const string &value);
	void end(void);
	void cookie(const string &name, const string &value);
	void cookie(const string &name, const Cookie &cookie);
	void clearCookie(const string &name);
	void download(const string &file, string name = "");
	void link(const string &rel, const string &link);
	void location(const string &path);
	void redirect(const string &path, int status = 302);
	void render(const string &view, std::map<string, string> vars = {});
	Response& status(int code);
	Response& vary(const string &value);
	void sendStatus(int code);
	void send(const string &value);
};
#endif //SCAFFOLD_RESPONSE_HPP
