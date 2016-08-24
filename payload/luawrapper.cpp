#include "stdafx.h"
#include "luawrapper.h"

luawrapper::luawrapper()
{
	this->state = luaL_newstate();
}

void luawrapper::print_error()
{
	const char *message = lua_tostring(this->state, -1);
	puts(message);
	lua_pop(this->state, 1);
}

void luawrapper::execute(const char *fileName)
{
	this->state = luaL_newstate();
	luaL_openlibs(this->state);
	int result;
	result = luaL_loadfile(this->state, fileName);
	if (result != LUA_OK) {
		print_error();
		return;
	}

	result = lua_pcall(this->state, 0, LUA_MULTRET, 0);
	if (result != LUA_OK) {
		print_error();
		return;
	}
}
