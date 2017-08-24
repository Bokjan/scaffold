#ifndef SCAFFOLD_LOGGER_HPP
#define SCAFFOLD_LOGGER_HPP
#include <cstdio>
#include <string>
using std::string;
namespace scaf
{
	class Logger
	{
	private:
		FILE *fp;
		string file;
		bool copied;
		bool isEnable;

	public:
		Logger(void);
		Logger(Logger&);
		Logger(Logger&&);
		~Logger(void);
		void setFP(FILE *ptr);
		void setFile(const string &path);
		void enable(void);
		void disable(void);
		void printTime(void);
		void log(const char *fmt, ...);
		void print(const char *fmt, ...);
		void puts(const char *s);
		void putchar(const char c);
	};
	extern Logger accesslog;
}
#endif //SCAFFOLD_LOGGER_HPP
