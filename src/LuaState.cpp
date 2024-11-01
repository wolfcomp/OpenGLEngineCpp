#include "LuaState.h"
#include <iostream>

lua_State *LuaState::L = nullptr;

LuaState::LuaState()
{
    L = luaL_newstate();
}

void LuaState::run_script(const char *filename)
{
    int r = luaL_dofile(L, filename);
    if (r != LUA_OK)
    {
        std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }
}

void LuaState::assign_lua_func(const char *name, lua_CFunction func)
{
    lua_pushcfunction(L, func);
    lua_setglobal(L, name);
}

void LuaState::do_string(const char *cmd)
{
    int r = luaL_dostring(L, cmd);
    if (r != LUA_OK)
    {
        std::cerr << "Error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }
}

lua_State *LuaState::get_state()
{
    return L;
}

void LuaState::close()
{
    lua_close(L);
}

void LuaState::open_libs()
{
    luaL_openlibs(L);
}

float LuaState::get_number(const char *name)
{
    lua_getglobal(L, name);
    if (lua_isnumber(L, -1))
    {
        float result = lua_tonumber(L, -1);
        lua_pop(L, 1);
        return result;
    }
    lua_pop(L, 1);
    return 0;
}

void LuaState::set_number(const char *name, float value)
{
    lua_pushnumber(L, value);
    lua_setglobal(L, name);
}

const char *LuaState::get_string(const char *name)
{
    lua_getglobal(L, name);
    if (lua_isstring(L, -1))
    {
        const char *result = lua_tostring(L, -1);
        lua_pop(L, 1);
        return result;
    }
    lua_pop(L, 1);
    return "";
}

void LuaState::set_string(const char *name, const char *value)
{
    lua_pushstring(L, value);
    lua_setglobal(L, name);
}

long LuaState::get_integer(const char *name)
{
    lua_getglobal(L, name);
    if (lua_isinteger(L, -1))
    {
        long result = lua_tointeger(L, -1);
        lua_pop(L, 1);
        return result;
    }
    lua_pop(L, 1);
    return 0;
}

void LuaState::set_integer(const char *name, long value)
{
    lua_pushinteger(L, value);
    lua_setglobal(L, name);
}

bool LuaState::get_boolean(const char *name)
{
    lua_getglobal(L, name);
    if (lua_isboolean(L, -1))
    {
        bool result = lua_toboolean(L, -1);
        lua_pop(L, 1);
        return result;
    }
    lua_pop(L, 1);
    return false;
}

void LuaState::set_boolean(const char *name, bool value)
{
    lua_pushboolean(L, value);
    lua_setglobal(L, name);
}