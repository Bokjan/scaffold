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
	string sslKey;
	string sslCert;
	bool isListening;

	void eventHandler(mg_connection*, int, void*);
	void preProcessAccessLog(http_message*);
	void postProcessAccessLog(Request&, Response&);
public:
	scaffold(void);
	scaffold(scaffold&);
	scaffold(scaffold&&);
	~scaffold(void);
	static scaffold* getPointer(void);
	static scaffold& getReference(void);

	void setSSL(const string &cert, const string &key);
	void setRoot(const string &root); // Set root of static files, absolute path
	void setListing(bool enable); // Enable directory listing
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
auto Scaffold = []() -> scaffold&
{
	return scaffold::getReference();
};
#endif //SCAFFOLD_SCAFFOLD_HPP
