## Makefile.am for jackmix

# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out
SUBDIRS = .

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables

AM_CXXFLAGS = -DQT_PLUGIN
widgetdir = $(libdir)/designer
widget_LTLIBRARIES = libjackmix_gui_designer.la


# set the include path for X, qt and KDE
INCLUDES         = \
	-I$(top_srcdir)/guilib \
	$(QT_INCLUDES)

# the library search path. 
libjackmix_gui_designer_la_LDFLAGS = $(QT_LDFLAGS) -module -avoid-version -no-undefined

# the libraries to link against.
libjackmix_gui_designer_la_LIBADD   = ../libjackmix_gui.la $(LIB_QT)

# which sources should be compiled for jackmix
libjackmix_gui_designer_la_SOURCES = \
	designer_plugin.cpp

# these are the headers for your project
#noinst_HEADERS   = jackmix.h jackmixview.h jackmixpref.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

