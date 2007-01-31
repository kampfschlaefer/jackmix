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

#env['CXXFLAGS']+="-Wall -Werror -g -fpic" #-mcmodel=medium"
env['CXXFLAGS']+="-Wall -fpic" #-mcmodel=medium"
#env['BKS_DEBUG'] = "full"

#env['DEBUG']="_debug"
env['DEBUG']=""

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

conf = Configure( env, custom_tests={'CheckPKGConfig' : CheckPKGConfig }, conf_dir='cache', log_file='cache/config.log' )
conf.CheckPKGConfig( 'jack' )
conf.CheckPKGConfig( 'liblo' )

env = conf.Finish()

###################################################################
# SCRIPTS FOR BUILDING THE TARGETS
###################################################################

## target processing is done in the subdirectory
env.subdirs(['libcore','libgui','backend','libmatrix','libelements','jackmix'])

