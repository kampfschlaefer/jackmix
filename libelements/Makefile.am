## Makefile.am for jackmix

# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out
SUBDIRS = .

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
lib_LTLIBRARIES = libjackmix_mixingelements.la

# set the include path for X, qt and KDE
INCLUDES = \
	-I$(top_srcdir)/libmatrix \
	-I$(top_srcdir)/libgui \
	-I$(top_srcdir)/backend \
	$(QT_INCLUDES) $(X_INCLUDES)

# the library search path. 
libjackmix_mixingelements_la_LDFLAGS = $(QT_LDFLAGS)

# the libraries to link against.
libjackmix_mixingelements_la_LIBADD = \
	$(top_builddir)/libmatrix/libjackmix_mixingmatrix.la \
	$(top_builddir)/libgui/libjackmix_gui.la \
	$(LIB_QT) $(LIB_XEXT) $(LIB_X11)

#	$(top_builddir)/backend/libjackmix_backend.la \
#
# which sources should be compiled
libjackmix_mixingelements_la_SOURCES = \
	stereo_elements.cpp \
	aux_elements.cpp

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

