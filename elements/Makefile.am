## Makefile.am for jackmix

# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out
SUBDIRS = .

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
lib_LTLIBRARIES = libmixingelements.la

# set the include path for X, qt and KDE
INCLUDES = \
	-I$(top_srcdir)/matrix \
	-I$(top_srcdir)/guilib \
	-I$(top_srcdir)/backend \
	$(QT_INCLUDES) $(X_INCLUDES)

# the library search path. 
libmixingelements_la_LDFLAGS = $(QT_LDFLAGS)

# the libraries to link against.
libmixingelements_la_LDADD = \
	$(top_builddir)/matrix/libmixingmatrix.la \
	$(top_builddir)/guilib/libjackmix_gui.la \
	$(top_builddir)/guilib/libjackmix_backend.la \
	$(LIB_QT) $(LIB_XEXT) $(LIB_X11)

# which sources should be compiled
libmixingelements_la_SOURCES = \
	aux_elements.cpp

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

