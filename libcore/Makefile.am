
SUBDIRS=.

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
check_PROGRAMS = config_test
lib_LTLIBRARIES = libjackmix_core.la

# set the include path for X, qt and KDE
INCLUDES = \
	$(QT_INCLUDES)

# the library search path. 
#jackmix_LDFLAGS = $(KDE_RPATH) $(all_libraries)
#jackmix_osc_client_LDFLAGS = $(QT_LDFLAGS) $(JACK_CFLAGS)
config_test_LDFLAGS = $(QT_LDFLAGS)
libjackmix_core_la_LDFLAGS = $(QT_LDFLAGS)

# the libraries to link against.
#jackmix_osc_client_LDADD = \
#	$(LIB_QT) $(JACK_LIBS)
config_test_LDADD = libjackmix_core.la $(LIB_QT)
libjackmix_core_la_LIBADD = \
	$(LIBLO_LIBS) \
	$(LIB_QT)

libjackmix_core_la_CXXFLAGS = -DQT_FATAL_ASSERT
# which sources should be compiled for jackmix
#jackmix_osc_client_SOURCES = \
#	lineinput.cpp \
#	jackmix_osc_client_main.cpp

config_test_SOURCES = \
	configoption_test.cpp

libjackmix_core_la_SOURCES = \
	configoption.cpp \
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
