#include <Request.hpp>
#include "Router.hpp"
#include "Response.hpp"
#include "../Utility/Utility.hpp"
using scaf::FileExists;
using scaf::DirectoryExists;
namespace scaf
{
	Router router;
}
void scaf::Router::registerCallback(int method, string path, callback_t callback)
{
	callbacks[method].push_back(std::make_pair(path, callback));
}
std::pair<string, callback_t> scaf::Router::fetchCallbacks(HttpMethod method, string path)
{
	std::pair<string, callback_t> ret = std::make_pair(path, [&](Request &req, Response &res)
	{
		router.serveStaticFile(req, res);
	});
	if(method == HttpMethod::NOTIMPLEMENTED)
		return ret;
	auto alen = path.length();
	const auto &vec = callbacks[static_cast<int>(method)];
	for(const auto &item : vec)
	{
		bool hasParam = false;
		const auto &pattern = item.first;
		auto blen = pattern.length();
		string::size_type i = 0, j = 0;
		for( ; i < alen && j < blen; )
		{
			while(path[i] == '/' && path[i + 1] == '/')
				++i;
			if(pattern[j] == ':') // Skip a param
			{
				hasParam = true;
				while(pattern[j] != '/' && j < blen)
					++j;
				while(path[i] != '/' && i < alen)
					++i;
				continue;
			}
			if(path[i] != pattern[j])
				goto CONTINUE;
			++i, ++j;
		}
		if(i == alen && j == blen)
		{
			if(hasParam)
				ret = item;
			else
				return item;
		}
	CONTINUE:;
	}
	return ret;
}
void scaf::Router::setRoot(const string &root)
{
	documentRoot = root;
}
static string defaultPages[] =
{
	"/index.html", "/index.htm", "/default.html", "/default.htm"
};
void scaf::Router::serveStaticFile(Request &req, Response &res)
{
	if(documentRoot.empty() || req.method != HttpMethod::GET)
	{
		res.sendStatus(404);
		return;
	}
	auto file = documentRoot + req.path;
	if(DirectoryExists(file.c_str())) // Test default pages
	{
		for(const auto &i : defaultPages)
		{
			auto page = file + i;
			if(FileExists(page.c_str()))
			{
				res.download(page, "nil");
				return;
			}
		}
	}
	else if(FileExists(file.c_str()))
	{
		res.download(file, "nil");
		return;
	}
	res.sendStatus(404);
}
