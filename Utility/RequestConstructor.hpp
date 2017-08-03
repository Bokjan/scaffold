#ifndef SCAFFOLD_REQUESTCONSTRUCTOR_HPP
#define SCAFFOLD_REQUESTCONSTRUCTOR_HPP
#include <map>
#include "Declaration.hpp"
#include "mongoose/mongoose.h"
class RequestConstructor
{
public:
	static HttpMethod determineMethod(mg_str method);
	static bool determineXhr(std::map<string, string> &headers);
	static void buildHeaders(std::map<string, string> &headers, mg_str *k, mg_str *v);
	static void parseCookies(std::map<string, string> &cookies, const std::map<string, string> &headers);
	static void parseQuery(std::map<string, string> &query, mg_str q);
	static string getHostname(std::map<string, string> &headers);
	static string getIp(mg_connection *conn);
private:
	static string convertUrlEscapeSequence(const string &s);
};
#endif //SCAFFOLD_REQUESTCONSTRUCTOR_HPP
