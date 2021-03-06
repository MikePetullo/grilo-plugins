#
# Makefile.am
#
# Author: Victor Toso <me@victortoso.com>
#
# Copyright (C) 2013 Victor Toso. All rights reserved.

include $(top_srcdir)/gtester.mk

SUBDIRS = sources

ext_LTLIBRARIES = libgrlluafactory.la

lua-library/htmlentity.c: lua-library/htmlentity.gperf
	$(AM_V_GEN) gperf --struct-type -l -N html_entity_hash -L ANSI-C $< > $@

libgrlluafactory_la_CFLAGS =						\
	$(DEPS_LUA_FACTORY_CFLAGS)					\
	-DLUA_FACTORY_SOURCE_LOCATION=\"@LUA_FACTORY_SOURCE_LOCATION@\"

libgrlluafactory_la_LIBADD = $(DEPS_LUA_FACTORY_LIBS)

libgrlluafactory_la_LDFLAGS =	\
	-no-undefined		\
	-module			\
	-avoid-version

libgrlluafactory_la_SOURCES =					\
	grl-lua-factory.c					\
	grl-lua-factory.h					\
	grl-lua-library.c					\
	grl-lua-library.h					\
	grl-lua-library-operations.c				\
	grl-lua-library-operations.h				\
	grl-lua-common.h					\
	lua-library/htmlentity.h				\
	lua-library/lua-json.c					\
	lua-library/lua-xml.c					\
	lua-library/lua-libraries.h

nodist_libgrlluafactory_la_SOURCES =				\
	luafactoryresources.c					\
	luafactoryresources.h					\
	lua-library/htmlentity.c

extdir			= $(GRL_PLUGINS_DIR)

lua_factory_resources_files =	\
  lua-library/inspect.lua

luafactoryresources.c: grl-lua-factory.gresource.xml luafactoryresources.h $(lua_factory_resources_files)
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) $(srcdir)/grl-lua-factory.gresource.xml \
		--target=$@ --sourcedir=$(srcdir) --c-name _grl_lua_factory --generate-source

luafactoryresources.h: grl-lua-factory.gresource.xml
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) $(srcdir)/grl-lua-factory.gresource.xml \
		--target=$@ --sourcedir=$(srcdir) --c-name _grl_lua_factory --generate-header

CLEANFILES =				\
	luafactoryresources.h		\
	luafactoryresources.c		\
	lua-library/htmlentity.c

EXTRA_DIST +=					\
	$(lua_factory_resources_files)		\
	grl-lua-factory.gresource.xml 		\
	lua-library/htmlentity.gperf

DIST_SUBDIRS = sources

-include $(top_srcdir)/git.mk
