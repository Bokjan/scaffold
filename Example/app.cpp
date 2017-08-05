#include <cstdio>
#include <cstdlib>
#include <signal.h>
#include <scaffold/scaffold.hpp>
void signal_handler(int sig)
{
	fprintf(stderr, "Signal %d received, exiting...", sig);
	exit(0);
}
int main(void)
{
	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);
	auto app = scaffold::getReference();
	app.listen(3000);
}
