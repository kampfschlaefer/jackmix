## Makefile.am for jackmix

# this has all of the subdirectories that make will recurse into.  if
# there are none, comment this out
SUBDIRS = .

# this is the program that gets installed.  it's name is used for all
# of the other Makefile.am variables
bin_PROGRAMS = jackmix

# set the include path for X, qt and KDE
INCLUDES         = $(all_includes)

# the library search path. 
jackmix_LDFLAGS = $(KDE_RPATH) $(all_libraries)

# the libraries to link against.
jackmix_LDADD   = $(LIB_QT) -ljack -lrt -ldl -lpthread

# which sources should be compiled for jackmix
jackmix_SOURCES = \
	jack_backend.cpp \
	mainwindow.cpp \
	channelwidget.cpp \
	volumeslider.cpp \
	stereovolumeslider.cpp \
	volumegroup.cpp \
	vg_aux.cpp \
	vg_stereo.cpp \
	qfloatslider.cpp \
	qtickmarks.cpp \
	main.cpp

# these are the headers for your project
#noinst_HEADERS   = jackmix.h jackmixview.h jackmixpref.h

# client stuff
#jackmix_client_LDFLAGS = $(KDE_RPATH) $(all_libraries)
#jackmix_client_LDADD   = $(LIB_KDECORE)
#jackmix_client_SOURCES = jackmix_client.cpp

# let automoc handle all of the meta source files (moc)
METASOURCES = AUTO

messages: rc.cpp
	$(XGETTEXT) *.cpp -o $(podir)/jackmix.pot

KDE_ICON = jackmix

# this is where the kdelnk file will go 
#xdg_apps_data = jackmix.desktop

# this is where the XML-GUI resource file goes
#rcdir = $(kde_datadir)/jackmix
#rc_DATA = jackmixui.rc
