#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include "Utility.hpp"
bool scaf::FileExists(const char *file)
{
	if(access(file, R_OK | W_OK) != -1)
		return true;
	return false;
}
bool scaf::FileExists(const string &file)
{
	return scaf::FileExists(file.c_str());
}
bool scaf::DirectoryExists(const char *dir)
{
	DIR *d = opendir(dir);
	if(d != nullptr)
	{
		closedir(d);
		return true;
	}
	return false;
}
bool scaf::DirectoryExists(const string &dir)
{
	return scaf::DirectoryExists(dir.c_str());
}
