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

	bool accepts(const string &type);
	bool accepts(const std::vector<string> &types);
	bool acceptsCharset(const string &charset);
	bool acceptsCharsets(const std::vector<string> &charsets);
	bool acceptsEncoding(const string &encoding);
	bool acceptsEncodings(const std::vector<string> &encodings);
	bool acceptsLanguage(const string &language);
	bool acceptsLanguages(const std::vector<string> &languages);

	bool is(const string &type);
	string get(const string &field);
	string param(const string &name);
};
#endif //SCAFFOLD_REQUEST_HPP
