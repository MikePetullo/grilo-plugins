#
# Makefile.am
#
# Author: Victor Toso <me@victortoso.com>
#
# Copyright (C) 2013 Victor Toso. All rights reserved.

include $(top_srcdir)/gtester.mk

lua_sources_DATA =					\
	grl-acoustid.lua				\
	grl-thegamesdb.lua				\
	grl-metrolyrics.lua				\
	grl-spotify-cover.lua				\
	grl-lastfm-cover.lua				\
	grl-radiofrance.lua				\
	grl-euronews.lua				\
	grl-guardianvideos.lua				\
	grl-musicbrainz.lua				\
	grl-euronews.gresource				\
	grl-guardianvideos.gresource			\
	grl-radiofrance.gresource			\
	grl-video-title-parsing.lua			\
	grl-pocket.lua					\
	grl-pocket.gresource				\
	grl-appletrailers.lua				\
	grl-appletrailers.gresource			\
	grl-itunes-podcast.lua				\
	grl-itunes-podcast.gresource

lua_sourcesdir = $(datadir)/$(LUA_FACTORY_SOURCE_LOCATION)

grl-euronews.gresource: grl-euronews.gresource.xml
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --sourcedir=$(srcdir) $(srcdir)/grl-euronews.gresource.xml

grl-guardianvideos.gresource: grl-guardianvideos.gresource.xml
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --sourcedir=$(srcdir) $(srcdir)/grl-guardianvideos.gresource.xml

grl-radiofrance.gresource: grl-radiofrance.gresource.xml
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --sourcedir=$(srcdir) $(srcdir)/grl-radiofrance.gresource.xml

grl-pocket.gresource: grl-pocket.gresource.xml
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --sourcedir=$(srcdir) $(srcdir)/grl-pocket.gresource.xml

grl-appletrailers.gresource: grl-appletrailers.gresource.xml
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --sourcedir=$(srcdir) $(srcdir)/grl-appletrailers.gresource.xml

grl-itunes-podcast.gresource: grl-itunes-podcast.gresource.xml
	$(AM_V_GEN) $(GLIB_COMPILE_RESOURCES) --sourcedir=$(srcdir) $(srcdir)/grl-itunes-podcast.gresource.xml

CLEANFILES =						\
	grl-euronews.gresource				\
	grl-guardianvideos.gresource			\
	grl-radiofrance.gresource			\
	grl-pocket.gresource				\
	grl-appletrailers.gresource			\
	grl-itunes-podcast.gresource

EXTRA_DIST +=						\
	$(lua_sources_DATA)				\
	grl-euronews.gresource.xml			\
	grl-guardianvideos.gresource.xml		\
	grl-itunes-podcast.gresource.xml		\
	grl-radiofrance.gresource.xml			\
	grl-pocket.gresource.xml			\
	grl-appletrailers.gresource.xml			\
	euronews.svg					\
	guardianvideos.svg				\
	radiofrance.png					\
	trailers.svg					\
	pocket.svg					\
	itunes-podcast.png

-include $(top_srcdir)/git.mk
