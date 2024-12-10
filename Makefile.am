AUTOMAKE_OPTIONS = foreign

INCLUDES = -I$(prefix)/include -I/usr/local/include

bin_PROGRAMS = gvs

gvs_SOURCES = gvs_gui.c gvs_gui_cb.c gvs_gui_main.c gvs_gui.h

gvs_gui.c gvs_gui.h: gvs_gui.fd
	fdesign -convert gvs_gui

CLEANFILES = gvs_gui.c gvs_gui.h  # Yes, fdesign will make these.

EXTRA_DIST = gvs_gui.fd CHANGES
