## Makefile.am for jackmix

# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out
SUBDIRS = .

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = jackmix

# set the include path for X, qt and KDE
INCLUDES = \
	-I$(top_srcdir)/libmatrix \
	-I$(top_srcdir)/backend \
	-I$(top_srcdir)/libgui \
	-I$(top_srcdir)/libelements \
	$(QT_INCLUDES)

# the library search path. 
#jackmix_LDFLAGS = $(KDE_RPATH) $(all_libraries)
jackmix_LDFLAGS = $(QT_LDFLAGS) $(JACK_CFLAGS)

# the libraries to link against.
jackmix_LDADD = \
	$(top_builddir)/libmatrix/libjackmix_mixingmatrix.la \
	$(top_builddir)/libelements/libjackmix_mixingelements.la \
	../libgui/libjackmix_gui.la \
	$(top_builddir)/backend/libjackmix_backend.la \
	$(LIB_QT) $(JACK_LIBS)

# which sources should be compiled for jackmix
jackmix_SOURCES = \
	mainwindow.cpp \
	main.cpp

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
