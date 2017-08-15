#include "Router.hpp"
#include "Response.hpp"
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
	std::pair<string, callback_t> ret = std::make_pair(path, [](Request &req, Response &res)
	{
		res.sendStatus(404);
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
