#include <scaffold/Request.hpp>
#include <scaffold/Response.hpp>
#include <scaffold/Scaffold.hpp>
int main(void)
{
	auto &app = Scaffold();
	app.get("/", [](Request &req, Response &res)
	{
		res.send("It works!");
	});
	app.listen(3000);
}
