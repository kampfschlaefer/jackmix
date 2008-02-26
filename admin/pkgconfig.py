#!/usr/bin/python
#
# Copyright (C) 2007-2008 Arnold Krille
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

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
	env['PKGCONFIG_TESTS' ] = { 'CheckForPKGConfig' : CheckForPKGConfig, 'GetPKGFlags' : GetPKGFlags }

def exists( env ):
	return 1


#!/usr/bin/python
#
# Copyright (C) 2007-2008 Arnold Krille
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

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
	env['PKGCONFIG_TESTS' ] = { 'CheckForPKGConfig' : CheckForPKGConfig, 'GetPKGFlags' : GetPKGFlags }

def exists( env ):
	return 1


#!/usr/bin/python
#
# Copyright (C) 2007-2008 Arnold Krille
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

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
	env['PKGCONFIG_TESTS' ] = { 'CheckForPKGConfig' : CheckForPKGConfig, 'GetPKGFlags' : GetPKGFlags }

def exists( env ):
	return 1


