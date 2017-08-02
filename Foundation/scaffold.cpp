#include <cstdio>
#include <cstring>
#include <stdexcept>
#include "scaffold.hpp"
#include "mongoose/mongoose.h"
scaffold::scaffold(void)
{
	mgr = (mg_mgr*)malloc(sizeof(mg_mgr));
	mg_mgr_init(mgr, nullptr);
}
scaffold::~scaffold(void)
{
	mg_mgr_free(mgr);
	free(mgr);
}
scaffold* scaffold::getPointer(void)
{
	return &getReference();
}
scaffold& scaffold::getReference(void)
{
	static scaffold instance;
	return instance;
}
void scaffold::setSSL(string cert, string key)
{
	sslCert = cert, sslKey = key;
}
void scaffold::listen(int _port, bool ssl)
{
#if MG_ENABLE_SSL
	mg_bind_opts bindOpts;
	memset(&bindOpts, 0, sizeof bindOpts);
#endif
	if(_port > 0xFFFF || _port < 0)
		throw std::out_of_range("port out of range");
	sprintf(port, "%d", _port);
	auto lambda = [](mg_connection *nc, int ev, void *p) -> void
	{
		scaffold::getReference().eventHandler(nc, ev, p);
	};
#if MG_ENABLE_SSL
	if(ssl)
	{
		bindOpts.ssl_cert = sslCert.c_str();
		bindOpts.ssl_key  = sslKey.c_str();
		conn = mg_bind_opt(mgr, port, lambda, bindOpts);
	}
	else
#endif
		conn = mg_bind(mgr, port, lambda);
	if(conn == nullptr)
		throw std::runtime_error("listen on port failed");
	mg_set_protocol_http_websocket(conn);
	for( ; ;)
		mg_mgr_poll(mgr, 1000);
}
void scaffold::eventHandler(mg_connection *nc, int ev, void *p)
{
	// TODO
}
