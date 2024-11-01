#pragma once

#include <lua/lua.hpp>

class LuaState
{
private:
    static lua_State *L;

public:
    LuaState();
    static void run_script(const char *filename);
    static void assign_lua_func(const char *name, lua_CFunction func);
    static void do_string(const char *cmd);
    static lua_State *get_state();
    static void close();
    static void open_libs();
    static float get_number(const char *name);
    static void set_number(const char *name, float value);
    static const char *get_string(const char *name);
    static void set_string(const char *name, const char *value);
    static long get_integer(const char *name);
    static void set_integer(const char *name, long value);
    static bool get_boolean(const char *name);
    static void set_boolean(const char *name, bool value);
};