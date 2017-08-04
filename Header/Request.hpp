#ifndef SCAFFOLD_REQUEST_HPP
#define SCAFFOLD_REQUEST_HPP
#include <map>
#include <vector>
#include "Declaration.hpp"
class Request
{
public:
	bool xhr;
	string ip;
	string body;
	string path;
	string hostname;
	HttpMethod method;
	std::vector<string> params; // Todo: implement initialization
	std::map<string, string> query;
	std::map<string, string> cookies;
	std::map<string, string> headers;

	void _initialize(mg_connection *nc, int ev, void *p);

	bool is(const string &type);
	string get(const string &field);
	string param(const string &name);
};
#endif //SCAFFOLD_REQUEST_HPP
