/*
    C-readline.c - readline and history bindings for Lua

   This Lua5 module is Copyright (c) 2013, Peter J Billam
                     www.pjb.com.au

 This module is free software; you can redistribute it and/or
       modify it under the same terms as Lua5 itself.
*/

#include <lua.h>
#include <lauxlib.h>
/* #include <string.h>  thats where strlen & friends are declared */

/* --------------- from man readline -------------------- */
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
/* http://cnswww.cns.cwru.edu/php/chet/readline/rltop.html
   http://cnswww.cns.cwru.edu/php/chet/readline/readline.html#SEC52
     (totally alarming :-( )
   http://cnswww.cns.cwru.edu/php/chet/readline/history.html#IDX5
     (only moderately alarming)
*/
/* see Programming in Lua p.233 */

static int c_readline(lua_State *L) {  /* Lua stack: prompt */
	size_t len;
	const char *prompt = lua_tolstring(L, 1, &len);
    const char *line   = readline(prompt);
	lua_pushstring(L, line);
	return 1;
}

static int c_tabcompletion(lua_State *L) {  /* Lua stack: prompt */
	int is_on = lua_toboolean(L, 1);
    if (is_on) {
		rl_bind_key ('\t', rl_complete);
	} else {
		rl_bind_key ('\t', rl_insert);
	}
	return 0;
}

static int c_history_length(lua_State *L) {  /* void in, length out */
    lua_Integer n = history_length;
	lua_pushinteger(L, n);
	return 1;
}

static int c_using_history(lua_State *L) {  /* void in, void out */
    using_history();
	return 0;
}

static int c_clear_history(lua_State *L) {  /* void in, void out */
    clear_history();
	return 0;
}

static int c_add_history(lua_State *L) {  /* Lua stack: str to be added */
	size_t len;
	const char *str  = lua_tolstring(L, 1, &len);
    add_history(str);
	return 0;
}

static int c_append_history(lua_State *L) {  /* num,filename in, void out */
	lua_Integer num = lua_tointeger(L, 1);
	size_t len;
	const char *filename = lua_tolstring(L, 2, &len);
    lua_Integer rc = append_history(num, filename);
	lua_pushinteger(L, rc);
	return 1;
}

static int c_read_history(lua_State *L) {  /* Lua stack: filename */
	size_t len;
	const char *filename  = lua_tolstring(L, 1, &len);
    lua_Integer rc = read_history(filename);
	lua_pushinteger(L, rc);
	return 1;
}

static int c_stifle_history(lua_State *L) {  /* Lua stack: num */
	lua_Integer num  = lua_tointeger(L, 1);
    stifle_history(num);
	return 0;
}

static int c_write_history(lua_State *L) {  /* Lua stack: filename */
	size_t len;
	const char *filename  = lua_tolstring(L, 1, &len);
    write_history(filename);
	return 0;
}

static int c_history_truncate_file(lua_State *L) {  /* stack: filename,num */
	size_t len;
	const char *filename  = lua_tolstring(L, 1, &len);
	lua_Integer num       = lua_tointeger(L, 2);
    history_truncate_file(filename, num);
	return 0;
}

/* ----------------- evolved from C-midialsa.c ---------------- */
struct constant {  /* Gems p. 334 */
    const char * name;
    int value;
};
static const struct constant constants[] = {
    /* {"Version", Version}, */
    {NULL, 0}
};

static const luaL_Reg prv[] = {  /* private functions */
    {"add_history",           c_add_history},
    {"append_history",        c_append_history},
    {"clear_history",         c_clear_history},
    {"history_length",        c_history_length},
    {"history_truncate_file", c_history_truncate_file},
    {"read_history",          c_read_history},
    {"readline",              c_readline},
    {"stifle_history",        c_stifle_history},
    {"tabcompletion",         c_tabcompletion},
    {"using_history",         c_using_history},
    {NULL, NULL}
};

static int initialise(lua_State *L) {  /* Lua Programming Gems p. 335 */
    /* Lua stack: aux table, prv table, dat table */
    int index;  /* define constants in module namespace */
    for (index = 0; constants[index].name != NULL; ++index) {
        lua_pushinteger(L, constants[index].value);
        lua_setfield(L, 3, constants[index].name);
    }
    /* lua_pushvalue(L, 1);   * set the aux table as environment */
    /* lua_replace(L, LUA_ENVIRONINDEX);
       unnecessary here, fortunately, because it fails in 5.2 */
    lua_pushvalue(L, 2); /* register the private functions */
#if LUA_VERSION_NUM >= 502
    luaL_setfuncs(L, prv, 0);    /* 5.2 */
    return 0;
#else
    luaL_register(L, NULL, prv); /* 5.1 */
    return 0;
#endif
}

int luaopen_readline(lua_State *L) {
    lua_pushcfunction(L, initialise);
    return 1;
}
