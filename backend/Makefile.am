## Makefile.am for jackmix

# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out
SUBDIRS = .

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
noinst_LTLIBRARIES = libjackmix_backend.la

# set the include path for X, qt and KDE
INCLUDES         = $(QT_INCLUDES)

# the library search path. 
#jackmix_LDFLAGS = $(KDE_RPATH) $(all_libraries)
libjackmix_backend_la_LDFLAGS = $(JACK_CFLAGS) -module -avoid-version -no-undefined

# the libraries to link against.
#jackmix_guilib_LDADD   = $(LIB_QT) $(JACK_LIBS)
libjackmix_backend_la_LIBADD   = $(LIB_QT) $(AK_JACK_LIBS)

# which sources should be compiled for jackmix
libjackmix_backend_la_SOURCES = jack_backend.cpp

# these are the headers for your project
#noinst_HEADERS   = jackmix.h jackmixview.h jackmixpref.h

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

#messages: rc.cpp
#	$(XGETTEXT) *.cpp -o $(podir)/jackmix.pot

#KDE_ICON = jackmix

# this is where the kdelnk file will go 
#xdg_apps_data = jackmix.desktop

# this is where the XML-GUI resource file goes
#rcdir = $(kde_datadir)/jackmix
#rc_DATA = jackmixui.rc
