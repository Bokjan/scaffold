#ifndef SCAFFOLD_ROUTER_HPP
#define SCAFFOLD_ROUTER_HPP
#include <vector>
#include <utility>
#include <functional>
#include "Declaration.hpp"
class Request;
class Response;
class scaffold;
using callback_t = std::function<void(Request&, Response&)>;
namespace scaf
{
	class Router
	{
	private:
		string documentRoot;
		std::vector<std::pair<string, callback_t>> callbacks[8];
	public:
		void registerCallback(int method, string path, callback_t callback);
		std::pair<string, callback_t> fetchCallbacks(HttpMethod method, string path);
		void serveStaticFile(Request &req, Response &res);
		void setRoot(const string &root);
	};
	extern Router router;
}
#endif //SCAFFOLD_ROUTER_HPP
