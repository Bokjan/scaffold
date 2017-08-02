#ifndef SCAFFOLD_REQUEST_HPP
#define SCAFFOLD_REQUEST_HPP
#include <map>
#include <string>
using std::string;
class Request
{
public:
	string body;
	std::map<string, string> cookies;

};
#endif //SCAFFOLD_REQUEST_HPP
