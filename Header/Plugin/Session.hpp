#ifndef SCAFFOLD_SESSION_HPP
#define SCAFFOLD_SESSION_HPP
/* *
 * Note: This header is not for
 * common use! Don't include it.
 * */
class Request;
class Response;
namespace scaf
{
	class Session
	{
	public:
		static void onRequestReceived(Request&, Response&);
		static void onResponseFinished(Request&, Response&);
	private:
		static string generateSessionID(Request&);
		static void setNewSession(Request&, Response&);
		static void ensureDirectoryAvailable(void);
		static void parseSessionDataFromJson(const string&, Request&, Response&);
	};
}
#endif //SCAFFOLD_SESSION_HPP
