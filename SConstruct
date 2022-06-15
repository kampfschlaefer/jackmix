#! /usr/bin/env python

import os

###################################################################
# LOAD THE ENVIRONMENT AND SET UP THE TOOLS
###################################################################

# Load the builders in config
env = Environment(
        ENV=os.environ,
        tools=['default', 'pkgconfig', 'qt5muc'],
        toolpath=['admin']
)

env.Replace(LIBS="")
env.Replace(LIBPATH="")

env.MergeFlags(['-Wall', '-Werror', '-g', '-fpic', '-std=c++11'])

tests = {}
tests.update(env['PKGCONFIG_TESTS'])

conf = Configure(
        env,
        custom_tests=tests,
        conf_dir='cache',
        log_file='cache/config.log'
)

if not conf.CheckHeader('stdio.h', language="C"):
        Exit(1)
if not conf.CheckHeader("iostream", language="C++"):
        Exit(1)

allpresent = 1

allpresent &= conf.CheckForPKGConfig()

pkgs = {
	'jack': '0.100.0',
	#'lash-1.0': '0.5.1',
	'Qt5Core': '5.3',
	'Qt5Widgets': '5.3',
	'Qt5Gui': '5.3',
	'Qt5Xml': '5.3',
	'alsa': '1.0'
}
for pkg in pkgs:
        name2 = pkg.replace("+", "").replace(".", "").replace("-", "").upper()
        res = conf.GetPKGFlags(pkg, pkgs[pkg])
        if isinstance(res, bytes):
                res = res.decode()
        env['%s_FLAGS' % name2] = res
        if env['%s_FLAGS' % name2] == 0:
                allpresent &= 0

if not allpresent:
        print(
                "(At least) One of the dependencies is missing. I can't go on without it..."
        )
        Exit(1)

env = conf.Finish()

env.MergeFlags("-I#")

print("Merging %s" % env["QT5WIDGETS_FLAGS"])
env.MergeFlags(env["QT5WIDGETS_FLAGS"])

# target processing is done in the subdirectory
env.SConscript(
	dirs=[
		'libcore',
		#'libqlash',
		'libgui',
		'backend',
		'libmatrix',
		'libelements',
		'libcontrol',
		'jackmix'
	],
	exports="env"
)
