#include <ctime>
#include <random>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <sys/stat.h>
#include "Request.hpp"
#include "Response.hpp"
#include "Plugin/Session.hpp"
#include "../Utility/Utility.hpp"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
const static char SESSID_KEY[] = "SCSESSID";
const static char SESS_DIR[] = "/tmp/scsess/";
static inline string getSessionFileName(const string &sid)
{
	return SESS_DIR + sid + ".json";
}
string scaf::Session::generateSessionID(Request &req)
{
	std::ostringstream oss;
	static std::random_device rd;
	oss << req.ip << time(nullptr) << rd();
	auto ostr = oss.str();
	// Returns 26 characters, as PHPSESSID
	return scaf::SHA256(ostr.c_str(), ostr.length()).substr(0, 26);
}
void scaf::Session::onRequestReceived(Request &req, Response &res)
{
	auto f = req.cookies.find(SESSID_KEY);
	if(f == req.cookies.end()) // Not found
		setNewSession(req, res);
	else
		parseSessionDataFromJson(f->second, req, res);
}
void scaf::Session::setNewSession(Request &req, Response &res)
{
	string sid = generateSessionID(req);
	ensureDirectoryAvailable();
	// Create the session file and write a empty JSON
	string filename = getSessionFileName(sid);
	FILE *sf = fopen(filename.c_str(), "w");
	if(sf == nullptr)
		throw std::runtime_error("failed to create session file(s)");
	fputs("{}", sf);
	// Update Request's JSON document
	req.session.Parse("{}");
	// Cookie operations
	req.cookies[SESSID_KEY] = sid;
	res.cookie(SESSID_KEY, sid);
}
void scaf::Session::ensureDirectoryAvailable(void)
{
	if(!scaf::DirectoryExists(SESS_DIR))
		if(mkdir(SESS_DIR, S_IRWXU) == -1)
			throw std::runtime_error("failed to create session directory");
}
void scaf::Session::parseSessionDataFromJson(const string &sid, Request &req, Response &res)
{
	// Test if local session file exists
	string filename = getSessionFileName(sid);
	if(!scaf::FileExists(filename.c_str()))
	{
		setNewSession(req, res);
		return;
	}
	FILE *fp = fopen(filename.c_str(), "r");
	char read_buffer[4096];
	rapidjson::FileReadStream is(fp, read_buffer, sizeof(read_buffer));
	req.session.ParseStream(is);
	fclose(fp);
}
void scaf::Session::onResponseFinished(Request &req, Response &res)
{
	auto f = req.cookies.find(SESSID_KEY);
	if(f == req.cookies.end()) // Not found
		return;
	string filename = getSessionFileName(f->second);
	FILE *fp = fopen(filename.c_str(), "w");
	char write_buffer[4096];
	if(fp == nullptr)
		throw std::runtime_error("failed to write session data to disk");
	rapidjson::FileWriteStream os(fp, write_buffer, sizeof(write_buffer));
	rapidjson::Writer<rapidjson::FileWriteStream> writer(os);
	req.session.Accept(writer);
	fclose(fp);
}
