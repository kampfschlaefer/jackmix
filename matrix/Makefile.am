## Makefile.am for jackmix

# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out
SUBDIRS = .

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
check_PROGRAMS = matrix
noinst_LTLIBRARIES = libmixingmatrix.la

# set the include path for X, qt and KDE
INCLUDES = -I$(top_srcdir)/guilib $(QT_INCLUDES) $(X_INCLUDES)

# the library search path. 
#jackmix_LDFLAGS = $(KDE_RPATH) $(all_libraries)
matrix_LDFLAGS = $(QT_LDFLAGS) $(X_LDFLAGS)
libmixingmatrix_la_LDFLAGS = $(QT_LDFLAGS)

# the libraries to link against.
matrix_LDADD = libmixingmatrix.la $(top_builddir)/guilib/libjackmix_gui.la $(LIB_QT) $(LIB_XEXT) $(LIB_X11)
libmixingmatrix_la_LIBADD = $(LIB_QT) $(LIB_XEXT) $(LIB_X11)

# which sources should be compiled for jackmix
matrix_SOURCES = \
	matrixelements.cpp \
	mainwidget.cpp \
	main.cpp

libmixingmatrix_la_SOURCES = \
	mixingmatrix.cpp

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

