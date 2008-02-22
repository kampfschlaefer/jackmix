# Copyright LiuCougar 2005
# BSD license (see COPYING)

RED    ="\033[91m"
GREEN  ="\033[92m"
NORMAL ="\033[0m"

"""
This tool is used to verify that pkg-config is installed
and provides a handy function to check a library which will
neceessary compilation and link flags automatically
"""
def exists(env):
	return true

def generate(env):
	if env['HELP']:
		p=env.pprint
		p("BOLD", "*** Pkgconfig Options ***")
		p("BOLD", "--------------------")
		p("BOLD", "* pcfile_path ", ":    where pc files should be installed into, $libdir/pkgconfig by default")
		print "            ie.",
		p("BOLD", "scons install pcfile_path=/usr/lib64/pkgconfig")
		return
	
	import SCons.Util, os
	
	REQUIRED_LIBSCIM_VERSION = env['REQUIRED_LIBSCIM_VERSION']

	# the following two functions are used to detect the packages using pkg-config - scons feature
	def Check_pkg_config(context, version):
		context.Message('Checking for pkg-config           : ')
		PKGCONFIG = 'pkg-config'
		if os.environ.has_key("PKG_CONFIG"):
			PKGCONFIG = os.environ["PKG_CONFIG"]
		default_include = "/usr/local/lib/pkgconfig"
		if os.environ.has_key("PKG_CONFIG_PATH"):
			os.environ["PKG_CONFIG_PATH"]=os.environ["PKG_CONFIG_PATH"]+":"+default_include
		else:
			os.environ["PKG_CONFIG_PATH"]=default_include
		PKGCONFIG = "PKG_CONFIG_PATH="+os.environ["PKG_CONFIG_PATH"]+ " " +PKGCONFIG
		ret = context.TryAction(PKGCONFIG+' --atleast-pkgconfig-version=%s' % version)[0]
		if(ret):
			env['PKGCONFIG']=PKGCONFIG
			context.Result(GREEN + " Found" + NORMAL)
		else:
			context.Result(RED + " Not Found" + NORMAL)
		return ret

	def PKGcheckmodules(env, context, varname, pattern):
		context.Message('Checking for %s ... ' % (pattern))
		pkg_config_command = env['PKGCONFIG']

		ret = context.TryAction(pkg_config_command+' --exists "%s"' % (pattern))[0]
		if ret:
			#env.ParseConfig(pkg_config_command+' %s --cflags --libs' % module);
			env[varname + '_CFLAGS'] = \
				SCons.Util.CLVar( 
					os.popen(pkg_config_command+' "%s" --cflags 2>/dev/null' % pattern).read().strip() );
			env[varname + '_LIBS'] = \
				SCons.Util.CLVar( 
					os.popen(pkg_config_command+' "%s" --libs 2>/dev/null' % pattern).read().strip() );
			context.Result(GREEN + " Found" + NORMAL)
		else:
			context.Result(RED + " Not Found" + NORMAL)
		return ret

	if env['HELP']:
		return
	# these are our options
	from SCons.Options import Options, PathOption
	cachefile = env['CACHEDIR']+'/pkg.cache.py'
	opts = Options(cachefile)
	opts.AddOptions(
		( 'PKGCONFIG_ISCONFIGURED', 'whether it is necessary to run configure or not' ),
		( 'PKGCONFIG', 'The command of pkg-config' ),
		( 'PCFILE_PATH', 'where to install pc files' ),
		)
	opts.Update(env)

	# detect the scim packages when needed
	if not env.has_key('PKGCONFIG_ISCONFIGURED'):
		conf = env.Configure(custom_tests = { 'Check_pkg_config' : Check_pkg_config})

		# delete the cached variables
		for var in "PKGCONFIG_ISCONFIGURED PKGCONFIG PCFILE_PATH".split():
			if env.has_key(var): env.__delitem__(var)
		
		REQUIRE_PKGCONFIG_VERSION='0.15'
		if not conf.Check_pkg_config(REQUIRE_PKGCONFIG_VERSION):
			print 'pkg-config >= ' + PKGCONFIG_VERSION + ' not found.' 
			print 'Make sure it is in your path, or set the '
			print 'PKG_CONFIG environment variable to the '
			print 'full path to pkg-config.'
			env.Exit(1) 

		env = conf.Finish()

		env['PKGCONFIG_ISCONFIGURED'] = 1
		env['PCFILE_PATH'] = ''

		if env.has_key('ARGS'):
			env['PCFILE_PATH'] = env['ARGS'].get('pcfile_path', '')
		
		if len(env['PCFILE_PATH']) == 0:
			def getpath(varname):
				if not env.has_key('ARGS'): return None
				v=env['ARGS'].get(varname, None)
				if v: v=os.path.abspath(v)
				return v
			libdir      = getpath('libdir')
			libsuffix   = ''
			if env.has_key('ARGS'): libsuffix=env['ARGS'].get('libsuffix', '')

			if libdir: libdir = libdir+libsuffix
			else :
				libdir=env['ARGS'].get('execprefix', env['ARGS'].get('prefix', '/usr'))+"/lib"+libsuffix

			env['PCFILE_PATH'] = os.path.join(libdir, 'pkgconfig')
		
		# store the config
		opts.Save(cachefile, env)

	# Attach the functions to the environment so that sconscripts or other modules can use them
	from SCons.Script.SConscript import SConsEnvironment
	SConsEnvironment.PKGcheckmodules = PKGcheckmodules

