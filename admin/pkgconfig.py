#!/usr/bin/python

#
# Taken from http://www.scons.org/wiki/UsingPkgConfig
# and heavily modified
#

#
# Checks for pkg-config
#
def CheckForPKGConfig( context, version='0.0.0' ):
	context.Message( "Checking for pkg-config (at least version %s)... " % version )
	ret = context.TryAction( "pkg-config --atleast-pkgconfig-version=%s" %version )[0]
	context.Result( ret )
	return ret

#
# Checks for the given package with an optional version-requirement
#
# The flags (which can be imported into the environment by env.MergeFlags(...)
# are exported as env['NAME_FLAGS'] where name is built by removing all +,-,.
# and upper-casing.
#
def CheckForPKG( context, name, version="" ):
	name2 = name.replace("+","").replace(".","").replace("-","")

	if version == "":
		context.Message( "Checking for %s... \t" % name )
		ret = context.TryAction( "pkg-config --exists '%s'" % name )[0]
	else:
		context.Message( "Checking for %s (%s or higher)... \t" % (name,version) )
		ret = context.TryAction( "pkg-config --atleast-version=%s '%s'" % (version,name) )[0]

	if ret:
		context.env['%s_FLAGS' % name2.upper()] = context.env.ParseFlags("!pkg-config --cflags --libs %s" % name)

	context.Result( ret )
	return ret

#
# Checks for the existance of the package and returns the packages flags.
#
# This should allow caching of the flags so that pkg-config is called only once.
#
def GetPKGFlags( context, name, version="" ):
	import os

	if version == "":
		context.Message( "Checking for %s... \t" % name )
		ret = context.TryAction( "pkg-config --exists '%s'" % name )[0]
	else:
		context.Message( "Checking for %s (%s or higher)... \t" % (name,version) )
		ret = context.TryAction( "pkg-config --atleast-version=%s '%s'" % (version,name) )[0]

	if not ret:
		context.Result( ret )
		return ret

	out = os.popen2( "pkg-config --cflags --libs %s" % name )[1]
	ret = out.read()

	context.Result( True )
	return ret

def generate( env, **kw ):
	env['PKGCONFIG_TESTS' ] = { 'CheckForPKGConfig' : CheckForPKGConfig, 'CheckForPKG' : CheckForPKG, 'GetPKGFlags' : GetPKGFlags }

def exists( env ):
	return 1


