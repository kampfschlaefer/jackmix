
SUBDIRS=.

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = jackmix_osc_client jackmix_osc_server

# set the include path for X, qt and KDE
INCLUDES = \
	-I$(top_srcdir)/backend \
	$(LIBLO_INCLUDES) $(QT_INCLUDES)

# the library search path. 
#jackmix_LDFLAGS = $(KDE_RPATH) $(all_libraries)
jackmix_osc_client_LDFLAGS = $(QT_LDFLAGS) $(JACK_CFLAGS) $(LIBLO_CFLAGS)
jackmix_osc_server_LDFLAGS = $(QT_LDFLAGS) $(JACK_CFLAGS) $(LIBLO_CFLAGS)

# the libraries to link against.
jackmix_osc_client_LDADD = \
	$(LIBLO_LIBS) \
	$(LIB_QT) $(JACK_LIBS)
jackmix_osc_server_LDADD = \
	$(top_builddir)/backend/libjackmix_backend.la \
	$(LIBLO_LIBS) \
	$(LIB_QT) $(JACK_LIBS)

# which sources should be compiled for jackmix
jackmix_osc_client_SOURCES = \
	osc_client.cpp \
	lineinput.cpp \
	jackmix_osc_client_main.cpp
jackmix_osc_server_SOURCES = \
	oscserver.cpp \
	jackmix_osc_server_main.cpp

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
