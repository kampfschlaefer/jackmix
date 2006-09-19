#! /usr/bin/env python

"""
help       -> scons -h
compile    -> scons
clean      -> scons -c
install    -> scons install
uninstall  -> scons -c install
configure  -> scons configure prefix=/tmp/ita debug=full extraincludes=/usr/local/include:/tmp/include prefix=/usr/local

Run from a subdirectory -> scons -u
The variables are saved automatically after the first run (look at cache/kde.cache.py, ..)
"""

###################################################################
# LOAD THE ENVIRONMENT AND SET UP THE TOOLS
###################################################################

## Load the builders in config
env = Environment( tools=['default', 'generic', 'qt4' ], toolpath=['./', './admin'])

#env.KDEuse("environ")
#env.KDEuse("environ rpath")
#env.QT4use("environ rpath lang_qt thread nohelp")

env['CXXFLAGS']+="-Wall -Werror -g -fpic" #-mcmodel=medium"

env['DEBUG']="_debug"
#env['DEBUG']=""

def CheckPKGConfig( context, pkgname ):
	import SCons.Util, os

	context.Message( "Checking for " + pkgname + "..." )
	ret = context.TryAction( 'pkg-config --exists ' + pkgname )
	if ret[0] == 1:
		tmp = os.popen( 'pkg-config --libs ' + pkgname ).read().strip()
		env[pkgname + '_LIBS'] = SCons.Util.CLVar( tmp )
		tmp = ' ' + os.popen( 'pkg-config --cflags ' + pkgname ).read().strip()
		env[pkgname + '_CFLAGS'] = SCons.Util.CLVar( tmp )
	context.Result( ret[0] )
	return ret[0]

conf = Configure( env, custom_tests={'CheckPKGConfig' : CheckPKGConfig } )
conf.CheckPKGConfig( 'jack' )
conf.CheckPKGConfig( 'liblo' )

env = conf.Finish()

###################################################################
# SCRIPTS FOR BUILDING THE TARGETS
###################################################################

## target processing is done in the subdirectory
env.subdirs(['libcore','libgui','backend','libmatrix','libelements','jackmix','osc'])

###################################################################
# CONVENIENCE FUNCTIONS TO EMULATE 'make dist' and 'make distclean'
###################################################################

### To make a tarball of your masterpiece, use 'scons dist'
#if 'dist' in COMMAND_LINE_TARGETS:
#
#	## The target scons dist requires the python module shutil which is in 2.3
#	env.EnsurePythonVersion(2, 3)
#
#	import os
#	APPNAME = 'epos'
#	VERSION = os.popen("cat VERSION").read().rstrip()
#	FOLDER  = APPNAME+'-'+VERSION
#	TMPFOLD = ".tmp"+FOLDER
#	ARCHIVE = FOLDER+'.tar.bz2'
#
#	## If your app name and version number are defined in 'version.h', use this instead:
#	## (contributed by Dennis Schridde devurandom@gmx@net)
#	#import re
#	#INFO = dict( re.findall( '(?m)^#define\s+(\w+)\s+(.*)(?<=\S)', open(r"version.h","rb").read() ) )
#	#APPNAME = INFO['APPNAME']
#	#VERSION = INFO['VERSION']
#
#	import shutil
#	import glob
#
#	## check if the temporary directory already exists
#	for dir in [FOLDER, TMPFOLD, ARCHIVE]:
#		if os.path.isdir(dir):
#			shutil.rmtree(dir)
#
#	## create a temporary directory
#	startdir = os.getcwd()
#	
#	os.popen("mkdir -p "+TMPFOLD)	
#	os.popen("cp -R * "+TMPFOLD)
#	os.popen("mv "+TMPFOLD+" "+FOLDER)
#
#	## remove scons-local if it is unpacked
#	os.popen("rm -rf "+FOLDER+"/scons "+FOLDER+"/sconsign "+FOLDER+"/scons-local-0.96.1")
#
#	## remove our object files first
#	os.popen("find "+FOLDER+" -name \"*cache*\" | xargs rm -rf")
#	os.popen("find "+FOLDER+" -name \"*.pyc\" | xargs rm -f")
#
#	## CVS cleanup
#	os.popen("find "+FOLDER+" -name \"CVS\" | xargs rm -rf")
#	os.popen("find "+FOLDER+" -name \".cvsignore\" | xargs rm -rf")
#
#	## Subversion cleanup
#	os.popen("find %s -name .svn -type d | xargs rm -rf" % FOLDER)
#
#	## GNU Arch cleanup
#	os.popen("find "+FOLDER+" -name \"{arch}\" | xargs rm -rf")
#	os.popen("find "+FOLDER+" -name \".arch-i*\" | xargs rm -rf")
#
#	## Create the tarball (coloured output)
#	print "\033[92m"+"Writing archive "+ARCHIVE+"\033[0m"
#	os.popen("tar cjf "+ARCHIVE+" "+FOLDER)
#
#	## Remove the temporary directory
#	if os.path.isdir(FOLDER):
#		shutil.rmtree(FOLDER)
#
#	env.Default(None)
#        env.Exit(0)
#
#### Emulate "make distclean"
#if 'distclean' in COMMAND_LINE_TARGETS:
#	## Remove the cache directory
#	import os, shutil
#	if os.path.isdir(env['CACHEDIR']):
#		shutil.rmtree(env['CACHEDIR'])
#	os.popen("find . -name \"*.pyc\" | xargs rm -rf")
#
#	env.Default(None)
#	env.Exit(0)
#
