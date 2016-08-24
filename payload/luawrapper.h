#pragma once
#include <lua.hpp>
class luawrapper
{
public:
	luawrapper();
	void print_error();
	void execute(const char *fileName);
private:
	lua_State *state; // http://csl.sublevel3.org/post/lua-and-cpp/
};

