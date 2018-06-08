-- This file was automatically generated for the LuaDist project.

package = "readline"
version = "1.5-0"
-- LuaDist source
source = {
  tag = "1.5-0",
  url = "git://github.com/LuaDist-testing/readline.git"
}
-- Original source
-- source = {
--    url = "http://www.pjb.com.au/comp/lua/readline-1.5.tar.gz",
--    md5 = "6cef95bd94f092b1236a66e2bb72ec62"
-- }
description = {
   summary = "Interface to the readline library",
   detailed = [[
      This Lua module offers a simple calling interface
      to the GNU Readline/History Library.
   ]],
   homepage = "http://www.pjb.com.au/comp/lua/readline.html",
   license = "MIT/X11",
}
-- http://www.luarocks.org/en/Rockspec_format
dependencies = {
   "lua >=5.1, <5.3",
   "luaposix >= 30",
}
build = {
   type = "builtin",
   modules = {
      ["readline"] = "readline.lua",
      ["C-readline"] = {
         sources   = { "C-readline.c" },
         libraries = { "readline" },
      },
   },
   copy_directories = { "doc", "test" },
}