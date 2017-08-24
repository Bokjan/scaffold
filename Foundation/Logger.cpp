#include <ctime>
#include <cstdarg>
#include <stdexcept>
#include "Logger.hpp"
namespace scaf
{
	Logger accesslog;
}
scaf::Logger::Logger(void):
	fp(stderr),
	copied(false), isEnable(true) { }
scaf::Logger::Logger(scaf::Logger &r):
	fp(r.fp), file(r.file),
	copied(false), isEnable(r.isEnable)
{
	r.copied = true;
}
scaf::Logger::Logger(scaf::Logger &&r):
	fp(r.fp), file(r.file),
	copied(false), isEnable(r.isEnable)
{
	r.copied = true;
}
scaf::Logger::~Logger(void)
{
	if(!copied)
		fclose(fp);
}
void scaf::Logger::setFP(FILE *ptr)
{
	fp = ptr;
}
void scaf::Logger::setFile(const string &path)
{
	file = path;
	fp = fopen(file.c_str(), "a");
	if(fp == nullptr)
		throw std::runtime_error("failed to open log file: " + file);
}
void scaf::Logger::printTime(void)
{
	time_t t = time(nullptr);
	tm *u = localtime(&t);
	fprintf(fp, "[%04d/%02d/%02d %02d:%02d:%02d] ",
	        1900 + u->tm_year, 1 + u->tm_mon, u->tm_mday,
	        u->tm_hour, u->tm_min, u->tm_sec);
}
void scaf::Logger::log(const char *fmt, ...)
{
	if(!isEnable)
		return;
	printTime();
	va_list ap;
	va_start(ap, fmt);
	vfprintf(fp, fmt, ap);
	va_end(ap);
	fputc('\n', fp);
	fflush(fp);
}
void scaf::Logger::print(const char *fmt, ...)
{
	if(!isEnable)
		return;
	va_list ap;
	va_start(ap, fmt);
	vfprintf(fp, fmt, ap);
	va_end(ap);
	fflush(fp);
}
void scaf::Logger::enable(void)
{
	isEnable = true;
}
void scaf::Logger::disable(void)
{
	isEnable = false;
}
void scaf::Logger::puts(const char *s)
{
	if(!isEnable)
		return;
	fputs(s, fp);
	fflush(fp);
}
void scaf::Logger::putchar(const char c)
{
	if(!isEnable)
		return;
	fputc(c, fp);
	fflush(fp);
}
