#include <cstdio>
#include <cstring>
#include <cassert>
#include <stdexcept>
#include "Router.hpp"
#include "Logger.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Scaffold.hpp"
#include "Plugin/Session.hpp"
#include "mongoose/mongoose.h"
#include "../Utility/RequestHelper.hpp"
using scaf::router;
using scaf::Session;
using scaf::accesslog;
static int sig_num = 0;
static void signal_handler(int sig)
{
	sig_num = sig;
	fprintf(stderr, "Signal %d received...\n", sig);
}
scaffold::scaffold(void):
	isListening(false)
{
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	mgr = (mg_mgr*)malloc(sizeof(mg_mgr));
	mg_mgr_init(mgr, nullptr);
}
scaffold::scaffold(scaffold &r):
	mgr(r.mgr), conn(r.conn),
	sslKey(r.sslKey), sslCert(r.sslCert)
{
	assert(!isListening);
}
scaffold::scaffold(scaffold &&r):
	mgr(r.mgr), conn(r.conn),
	sslKey(r.sslKey), sslCert(r.sslCert)
{
	assert(!isListening);
}
scaffold::~scaffold(void) { }
scaffold* scaffold::getPointer(void)
{
	return &getReference();
}
scaffold& scaffold::getReference(void)
{
	static scaffold instance;
	return instance;
}
void scaffold::setSSL(const string &cert, const string &key)
{
	sslCert = cert, sslKey = key;
}
void scaffold::setRoot(const string &root)
{
	router.setRoot(root);
}
void scaffold::setListing(bool enable)
{
	router.setListing(enable);
}
void scaffold::listen(int _port, bool ssl)
{
#if MG_ENABLE_SSL
	mg_bind_opts bindOpts;
	memset(&bindOpts, 0, sizeof bindOpts);
#endif
	char port[8];
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
	isListening = true; // Set the flag
	while(sig_num == 0)
		mg_mgr_poll(mgr, 1000);
	mg_mgr_free(mgr);
	free(mgr);
}
void scaffold::preProcessAccessLog(http_message *hm)
{
	accesslog.printTime();
	accesslog.print("\"%.*s %.*s %.*s\" ",
		hm->method.len, hm->method.p,
		hm->uri.len, hm->uri.p,
		hm->proto.len, hm->proto.p
	);
}
void scaffold::postProcessAccessLog(Request &req, Response &res)
{
	std::map<string, string>::iterator f;
	// IP and status code
	accesslog.print("%.*s %d ", req.ip.length(), req.ip.c_str(), res.statusCode);
	// Referer
	f = req.headers.find("Referer");
	if(f != req.headers.end())
		accesslog.print("\"%.*s\" ", f->second.length(), f->second.c_str());
	// User-Agent
	f = req.headers.find("User-Agent");
	if(f != req.headers.end())
		accesslog.print("\"%.*s\" ", f->second.length(), f->second.c_str());
	// X-Forwarded-For
	f = req.headers.find("X-Forwarded-For");
	if(f != req.headers.end())
		accesslog.print("\"%.*s\" ", f->second.length(), f->second.c_str());
	accesslog.putchar('\n');
}
void scaffold::eventHandler(mg_connection *nc, int ev, void *p)
{
	// Todo: finish this method
	switch(ev)
	{
		case MG_EV_HTTP_PART_BEGIN:
		case MG_EV_HTTP_PART_DATA:
		case MG_EV_HTTP_PART_END:
			break;
		case MG_EV_HTTP_REQUEST:
		{
			// Pre process access log
			preProcessAccessLog((http_message*)p);
			// Construct values
			Request req;
			req._initialize(nc, ev, p);
			Response res(&req, (http_message*)p, nc);
			// match.first => pattern, match.second => callback
			auto match = router.fetchCallbacks(req.method, req.path);
			RequestHelper::parseParams(req.params, req.path, match.first);
#if ENABLE_SESSION
			Session::onRequestReceived(req, res);
#endif
			// Call the callback
			match.second(req, res);
			// Post process access log
			postProcessAccessLog(req, res);
#if ENABLE_SESSION
			Session::onResponseFinished(req, res);
#endif
			break;
		}
		default:;
	}
}
void scaffold::all(string path, callback_t callback)
{
	for(int i = 0; i < 8; ++i)
		router.registerCallback(i, path, callback);
}
void scaffold::head(string path, callback_t callback)
{
	router.registerCallback(static_cast<int>(HttpMethod::HEAD), path, callback);
}
void scaffold::get(string path, callback_t callback)
{
	router.registerCallback(static_cast<int>(HttpMethod::GET), path, callback);
}
void scaffold::post(string path, callback_t callback)
{
	router.registerCallback(static_cast<int>(HttpMethod::POST), path, callback);
}
void scaffold::put(string path, callback_t callback)
{
	router.registerCallback(static_cast<int>(HttpMethod::PUT), path, callback);
}
void scaffold::Delete(string path, callback_t callback)
{
	router.registerCallback(static_cast<int>(HttpMethod::DELETE), path, callback);
}
void scaffold::trace(string path, callback_t callback)
{
	router.registerCallback(static_cast<int>(HttpMethod::TRACE), path, callback);
}
void scaffold::options(string path, callback_t callback)
{
	router.registerCallback(static_cast<int>(HttpMethod::OPTIONS), path, callback);
}
void scaffold::connect(string path, callback_t callback)
{
	router.registerCallback(static_cast<int>(HttpMethod::CONNECT), path, callback);
}
