
SUBDIRS=.

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
#check_PROGRAMS = jackmix_osc_client jackmix_osc_server
lib_LTLIBRARIES = libjackmix_core.la

# set the include path for X, qt and KDE
INCLUDES = \
	$(LIBLO_INCLUDES) $(QT_INCLUDES)

# the library search path. 
#jackmix_LDFLAGS = $(KDE_RPATH) $(all_libraries)
#jackmix_osc_client_LDFLAGS = $(QT_LDFLAGS) $(JACK_CFLAGS)
#jackmix_osc_server_LDFLAGS = $(QT_LDFLAGS) $(JACK_CFLAGS)
libjackmix_core_la_LDFLAGS = $(QT_LDFLAGS) $(JACK_CFLAGS) $(LIBLO_CFLAGS)

# the libraries to link against.
#jackmix_osc_client_LDADD = \
#	$(LIB_QT) $(JACK_LIBS)
#jackmix_osc_server_LDADD = \
#	$(LIB_QT) $(JACK_LIBS)
libjackmix_core_la_LIBADD = \
	$(LIBLO_LIBS) \
	$(LIB_QT)

# which sources should be compiled for jackmix
#jackmix_osc_client_SOURCES = \
#	lineinput.cpp \
#	jackmix_osc_client_main.cpp
#jackmix_osc_server_SOURCES = \
#	textedit.cpp \
#	jackmix_osc_server_main.cpp
libjackmix_core_la_SOURCES = \
	updatefilter.cpp

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
