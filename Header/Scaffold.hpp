#ifndef SCAFFOLD_SCAFFOLD_HPP
#define SCAFFOLD_SCAFFOLD_HPP
#define SCAFFOLD_VERSION_STRING "0.0.1"
#include <string>
#include "Declaration.hpp"
using std::string;
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
	scaffold(scaffold&) = delete;
	~scaffold(void);
	static scaffold* getPointer(void);
	static scaffold& getReference(void);

	void setSSL(const string &cert, const string &key);
	void setRoot(const string &root); // Set root of static files, absolute path
	void listen(int port, bool ssl = false);

	void all    (string path, callback_t callback);
	void head   (string path, callback_t callback);
	void get    (string path, callback_t callback);
	void post   (string path, callback_t callback);
	void put    (string path, callback_t callback);
	void Delete (string path, callback_t callback);
	void trace  (string path, callback_t callback);
	void options(string path, callback_t callback);
	void connect(string path, callback_t callback);
};
inline scaffold& Scaffold(void)
{
	return scaffold::getReference();
}
#endif //SCAFFOLD_SCAFFOLD_HPP
