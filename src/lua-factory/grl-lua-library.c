/*
 * Copyright (C) 2013 Victor Toso.
 *
 * Contact: Victor Toso <me@victortoso.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation; version 2.1 of
 * the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#include <net/grl-net.h>
#include <string.h>

#include "grl-lua-common.h"
#include "grl-lua-library.h"

#define GRL_LOG_DOMAIN_DEFAULT lua_library_log_domain
GRL_LOG_DOMAIN_STATIC (lua_library_log_domain);

/* ================== Lua-Library initialization =========================== */

gint
luaopen_grilo (lua_State *L)
{
  static const luaL_Reg library_fn[] = {
    {NULL, NULL}
  };

  GRL_LOG_DOMAIN_INIT (lua_library_log_domain, "lua-library");

  GRL_DEBUG ("Loading grilo lua-library");
  luaL_newlib (L, library_fn);

  return 1;
}

/* ======= Lua-Library and Lua-Factory utilities ============= */

/**
 * grl_lua_library_save_operation_data
 *
 * @L : LuaState where the data will be stored.
 * @os: The Operation Data to store.
 * @return: Nothing.
 *
 * Stores the OperationSpec from Lua-Factory in the global environment of
 * lua_State.
 **/
void
grl_lua_library_save_operation_data (lua_State *L, OperationSpec *os)
{
  lua_getglobal (L, LUA_ENV_TABLE);
  lua_pushstring (L, GRILO_LUA_OPERATION_INDEX);
  lua_pushlightuserdata (L, os);
  lua_settable (L, -3);
  lua_pop (L, 1);
}

/**
 * grl_lua_library_load_operation_data
 *
 * @L : LuaState where the data is stored.
 * @return: The Operation Data.
 **/
OperationSpec *
grl_lua_library_load_operation_data (lua_State *L)
{
  OperationSpec *os = NULL;

  lua_getglobal (L, LUA_ENV_TABLE);
  lua_pushstring (L, GRILO_LUA_OPERATION_INDEX);
  lua_gettable (L, -2);
  os = (lua_islightuserdata(L, -1)) ? lua_touserdata(L, -1) : NULL;
  lua_pop(L, 1);
  return os;
}
