#! /usr/bin/env python

###################################################################
# LOAD THE ENVIRONMENT AND SET UP THE TOOLS
###################################################################

## Load the builders in config
env = Environment( tools=['default', 'pkgconfig', 'qt4muc' ], toolpath=['admin'] )

env.Replace( LIBS="" )
env.Replace( LIBPATH="" )

env['CXXFLAGS']+="-Wall -Werror -g -fpic"

tests = { }
tests.update( env['PKGCONFIG_TESTS'] )

conf = Configure( env, custom_tests=tests, conf_dir='cache', log_file='cache/config.log' )

if not conf.CheckHeader( 'stdio.h', language="C" ):
	Exit( 1 )
if not conf.CheckHeader( "iostream", language="C++" ):
	Exit( 1 )

allpresent = 1

allpresent &= conf.CheckForPKGConfig()

pkgs = {
	'jack' : '0.100.0',
	'lash-1.0' : '0.5.1',
	'QtCore' : '4.2',
	'QtGui' : '4.2',
	'QtXml' : '4.2',
	}
for pkg in pkgs:
	name2 = pkg.replace("+","").replace(".","").replace("-","").upper()
	env['%s_FLAGS' % name2] = conf.GetPKGFlags( pkg, pkgs[pkg] )
	if env['%s_FLAGS'%name2] == 0:
		allpresent &= 0

if not allpresent:
	print "(At least) One of the dependencies is missing. I can't go on without it..."
	Exit( 1 )

env = conf.Finish()

env.MergeFlags( "-I#" )

env.MergeFlags( env["QTCORE_FLAGS"] )

## target processing is done in the subdirectory
env.SConscript( dirs=['libcore','libqlash','libgui','backend','libmatrix','libelements','jackmix'], exports="env" )

