#ifndef SCAFFOLD_REQUEST_HPP
#define SCAFFOLD_REQUEST_HPP
#include <map>
#include <vector>
#include "RapidJSON.hpp"
#include "Declaration.hpp"
class scaffold;
struct mg_connection;
class Request
{
private:
	mg_connection *conn;
	friend class scaffold;
public:
	bool xhr;
	string ip;
	string body;
	string path;
	string hostname;
	HttpMethod method;
	rapidjson::Document session;
	std::map<string, string> post;
	std::map<string, string> query;
	std::map<string, string> params;
	std::map<string, string> cookies;
	std::map<string, string> headers;

	Request(void): conn(nullptr) { }
	void _initialize(mg_connection *nc, int ev, void *p);

	bool is(const string &type);
	string get(const string &field);
};
#endif //SCAFFOLD_REQUEST_HPP
