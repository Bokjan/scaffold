#ifndef SCAFFOLD_SCAFFOLD_HPP
#define SCAFFOLD_SCAFFOLD_HPP
#define SCAFFOLD_VERSION_STRING "0.0.0"
#include <string>
#include "Declaration.hpp"
using std::string;
class scaffold
{
private:
	mg_mgr *mgr;
	mg_connection *conn;
	char port[8];
#if MG_ENABLE_SSL
	string sslKey;
	string sslCert;
#endif

	void eventHandler(mg_connection *nc, int ev, void *p);
public:
	scaffold(void);
	~scaffold(void);
	static scaffold* getPointer(void);
	static scaffold& getReference(void);

#if MG_ENABLE_SSL
	void setSSL(string cert, string key);
#endif

	void listen(int port, bool ssl = false);
};
#endif //SCAFFOLD_SCAFFOLD_HPP
