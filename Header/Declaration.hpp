#ifndef SCAFFOLD_DECLARATION_HPP
#define SCAFFOLD_DECLARATION_HPP
#include <ctime>
#include <string>
using std::string;
enum class HttpMethod
{

};
struct Cookie
{
	string  value;
	string  domain;
	time_t  expires;
	int     maxAge;
	bool    secure;
	string  path;
	// TODO: Default values
};
#endif //SCAFFOLD_DECLARATION_HPP
