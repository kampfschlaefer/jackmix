## Makefile.am for jackmix

# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out
SUBDIRS = .

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
noinst_LTLIBRARIES = libjackmix_vg.la

check_PROGRAMS = dialog_test

# set the include path for X, qt and KDE
INCLUDES         = -I$(top_srcdir)/backend -I$(top_srcdir)/guilib $(QT_INCLUDES)

# the library search path. 
#jackmix_LDFLAGS = $(KDE_RPATH) $(all_libraries)
libjackmix_vg_la_LDFLAGS = $(QT_LDFLAGS) -module -avoid-version -no-undefined
dialog_test_LDFLAGS = $(QT_LDFLAGS)

# the libraries to link against.
#jackmix_guilib_LDADD   = $(LIB_QT) $(JACK_LIBS)
libjackmix_vg_la_LIBADD   = $(top_builddir)/backend/libjackmix_backend.la $(top_builddir)/guilib/libjackmix_gui.la $(LIB_QT) $(JACK_LIBS)
dialog_test_LDADD = libjackmix_vg.la $(LIB_QT)

# which sources should be compiled for jackmix
libjackmix_vg_la_SOURCES = \
	volumegroup.cpp \
	vg_aux.cpp \
	vg_stereo.cpp \
	vgselect.cpp

dialog_test_SOURCES = dialog_test.cpp
#	qfloatpoti.cpp

# these are the headers for your project
#noinst_HEADERS   = jackmix.h jackmixview.h jackmixpref.h

# client stuff
#jackmix_client_LDFLAGS = $(KDE_RPATH) $(all_libraries)
#jackmix_client_LDADD   = $(LIB_KDECORE)
#jackmix_client_SOURCES = jackmix_client.cpp

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
