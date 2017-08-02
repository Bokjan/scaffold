#ifndef SCAFFOLD_SCAFFOLD_HPP
#define SCAFFOLD_SCAFFOLD_HPP
#include <string>
using std::string;
struct mg_mgr;
struct mg_connection;
class scaffold
{
private:
	mg_mgr *mgr;
	mg_connection *conn;
	char port[8];
	string sslKey;
	string sslCert;

	void eventHandler(mg_connection *nc, int ev, void *p);
public:
	scaffold(void);
	~scaffold(void);
	static scaffold* getPointer(void);
	static scaffold& getReference(void);

	void setSSL(string cert, string key);

	void listen(int port, bool ssl = false);
};
#endif //SCAFFOLD_SCAFFOLD_HPP
