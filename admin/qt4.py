# Made from scons qt.py and (heavily) modified into kde.py
# Thomas Nagy, 2004, 2005 <tnagy2^8@yahoo.fr>

"""
Run scons -h to display the associated help, or look below ..
"""

import os, re, types

def exists(env):
	return True

def detect_qt4(env):
	def getpath(varname):
		if not env.has_key('ARGS'): return None
		v=env['ARGS'].get(varname, None)
		if v : v=os.path.abspath(v)
		return v

	prefix		= getpath('prefix')
	execprefix	= getpath('execprefix')
	datadir		= getpath('datadir')
	libdir		= getpath('libdir')
	qtincludes	= getpath('qtincludes')
	qtlibs		= getpath('qtlibs')
	libsuffix	= ''
	if env.has_key('ARGS'): libsuffix=env['ARGS'].get('libsuffix', '')

	p=env.pprint

	qtdir = os.getenv("QTDIR")
	env['QTDIR'] = qtdir.strip()
	if qtdir:
		if not qtlibs:
			qtlibs = os.path.join(qtdir, 'lib' + libsuffix)
		if not qtincludes:
			qtincludes = os.path.join(qtdir, 'include')
		os.putenv('PATH', os.path.join(qtdir , 'bin') +
				  ":" + os.getenv("PATH"))

	def find_qt_bin(prog):
		## Find the necessary programs
		print "Checking for %s: " % prog,
		path = os.path.join(qtdir, 'bin', prog)
		if os.path.isfile(path):
			p('GREEN',"%s was found as %s" % (prog, path))
		else:
			path = os.popen("which %s 2>/dev/null" % prog).read().strip()
			if len(path):
				p('YELLOW',"%s was found as %s" % (prog, path))
			else:
				path = os.popen("which %s 2>/dev/null" % prog).read().strip()
				if len(path):
					p('YELLOW',"%s was found as %s" % (prog, path))
				else:
					p('RED',"%s was not found - fix $QTDIR or $PATH" % prog)
					env.Exit(1)
		return path
	
	env['QT_UIC'] = find_qt_bin('uic')
	print "Checking for UIC version                       :",
	version = os.popen(env['QT_UIC'] + " -version 2>&1").read().strip()
	if version.find(" 3.") != -1:
		version = version.replace('Qt user interface compiler','')
		version = version.replace('User Interface Compiler for Qt', '')
		p('RED', version + " (too old)")
		env.Exit(1)
	print "fine - ", version
	env['QT_MOC'] = find_qt_bin('moc')
	env['QT_RCC'] = find_qt_bin('rcc')

	if os.environ.has_key('PKG_CONFIG_PATH'):
		os.environ['PKG_CONFIG_PATH'] = os.environ['PKG_CONFIG_PATH'] \
										+ ':' + qtlibs
	else:
		os.environ['PKG_CONFIG_PATH'] = qtlibs

	## check for the qt and kde includes
	print "Checking for the qt includes		 : ",
	if qtincludes and os.path.isfile(qtincludes + "/QtGui/QFont"):
		# The user told where to look for and it looks valid
		p('GREEN',"ok "+qtincludes)
	else:
		if os.path.isfile(qtdir + "/include/QtGui/QFont"):
			# Automatic detection
			p('GREEN',"ok "+qtdir+"/include/")
			qtincludes = qtdir + "/include/"
		elif os.path.isfile("/usr/include/qt4/QtGui/QFont"):
			# Debian probably
			p('YELLOW','the qt headers were found in /usr/include/qt4/')
			qtincludes = "/usr/include/qt4"
		else:
			p('RED',"the qt headers were not found")
			env.Exit(1)

	if prefix:
		## use the user-specified prefix
		if not execprefix:
			execprefix = prefix
		if not datadir:
			datadir=prefix+"/share"
		if not libdir:
			libdir=execprefix+"/lib"+libsuffix

		subst_vars = lambda x: x.replace('${exec_prefix}', execprefix)\
				 .replace('${datadir}', datadir)\
				 .replace('${libdir}', libdir)
		debian_fix = lambda x: x.replace('/usr/share', '${datadir}')
		env['PREFIX'] = prefix
	else:
		env['PREFIX'] = "/usr/"

	#env['QTPLUGINS']=os.popen('kde-config --expandvars --install qtplugins').read().strip()

	## qt libs and includes
	env['QTINCLUDEPATH']=qtincludes
	if not qtlibs: qtlibs=env.join(qtdir, 'lib', libsuffix)
	env['QTLIBPATH']=qtlibs

def generate(env):
	"""Set up the qt environment and builders - the moc part can be difficult to understand """
	if env['HELP']:
                p=env.pprint
                p('BOLD','*** QT4 options ***')
                p('BOLD','--------------------')
                p('BOLD','* prefix     ','base install path,         ie: /usr/local')
                p('BOLD','* execprefix ','install path for binaries, ie: /usr/bin')
                p('BOLD','* datadir    ','install path for the data, ie: /usr/local/share')
                p('BOLD','* libdir     ','install path for the libs, ie: /usr/lib')
		p('BOLD','* libsuffix  ','suffix of libraries on amd64, ie: 64, 32')
		p('BOLD','* qtincludes ','qt includes path (/usr/include/qt on debian, ..)')
		p('BOLD','* qtlibs     ','qt libraries path, for linking the program')

                p('BOLD','* scons configure libdir=/usr/local/lib qtincludes=/usr/include/qt\n')
		return

	import SCons.Defaults
	import SCons.Tool
	import SCons.Util
	import SCons.Node

	CLVar = SCons.Util.CLVar
	splitext = SCons.Util.splitext
	Builder = SCons.Builder.Builder
	
	# Detect the environment - replaces ./configure implicitely and store the options into a cache
	from SCons.Options import Options
	cachefile=env['CACHEDIR']+'qt4.cache.py'
	opts = Options(cachefile)
	opts.AddOptions(
		('PREFIX', 'root of the program installation'),

		('QTDIR', ''),
		('QTLIBPATH', 'path to the qt libraries'),
		('QTINCLUDEPATH', 'path to the qt includes'),
		('QT_UIC', 'uic command'),
		('QT_MOC', 'moc command'),
		('QT_RCC', 'rcc command'),
		('QTPLUGINS', 'uic executable command'),
	)
	opts.Update(env)

	# reconfigure when things are missing
	if not env['HELP'] and (env['_CONFIGURE'] or not env.has_key('QTDIR')):
		#from qt4_config import detect_qt4
		detect_qt4(env)
		opts.Save(cachefile, env)

	## set default variables, one can override them in sconscript files
	#env.Append(CXXFLAGS = ['-I'+env['QTINCLUDEPATH'], '-I'+env['QTINCLUDEPATH']+'Qt'], LIBPATH = [env['QTLIBPATH'] ])
	env.Append(CXXFLAGS = ['-I'+env['QTINCLUDEPATH']], LIBPATH = [env['QTLIBPATH'] ])
	
	env['QT_AUTOSCAN'] = 1
	env['QT_DEBUG']    = 0

	env['MSGFMT']   = 'msgfmt'

	## ui file processing
	def uic_processing(target, source, env):
		comp_h   ='$QT_UIC -o %s %s' % (target[0].path, source[0].path)
		return env.Execute(comp_h)
	def uicEmitter(target, source, env):
		adjustixes = SCons.Util.adjustixes
		bs = SCons.Util.splitext(str(source[0].name))[0]
		bs = os.path.join(str(target[0].get_dir()),bs)
		#target.append(bs+'.cpp')
		#target.append(bs+'.moc')
		return target, source
	env['BUILDERS']['Uic']=Builder(action=uic_processing,emitter=uicEmitter,suffix='.h',src_suffix='.ui3')

	def qrc_buildit(target, source, env):
		dir=str(source[0].get_dir())
		name = SCons.Util.splitext( source[0].name )[0]
		comp='cd %s && %s -name %s %s -o %s' % (dir, env['QT_RCC'], name, source[0].name, target[0].name)
		return env.Execute(comp)
	def qrc_stringit(target, source, env):
		print "processing %s to get %s" % (source[0].name, target[0].name)

	env['BUILDERS']['Qrc']=Builder(action=env.Action(qrc_buildit, qrc_stringit), suffix='_qrc.cpp', src_suffix='.qrc')

	def kcfg_buildit(target, source, env):
		comp='kconfig_compiler -d%s %s %s' % (str(source[0].get_dir()), source[1].path, source[0].path)
		return env.Execute(comp)
	
	def kcfg_stringit(target, source, env):
		print "processing %s to get %s and %s" % (source[0].name, target[0].name, target[1].name)
		
	def kcfgEmitter(target, source, env):
		adjustixes = SCons.Util.adjustixes
		bs = SCons.Util.splitext(str(source[0].name))[0]
		bs = os.path.join(str(target[0].get_dir()),bs)
		# .h file is already there
		target.append(bs+'.cpp')

		if not os.path.isfile(str(source[0])):
			lenv.pprint('RED','kcfg file given'+str(source[0])+' does not exist !')
			return target, source
		kfcgfilename=""
		kcfgFileDeclRx = re.compile("^[fF]ile\s*=\s*(.+)\s*$")
		for line in file(str(source[0]), "r").readlines():
			match = kcfgFileDeclRx.match(line.strip())
			if match:
				kcfgfilename = match.group(1)
				break
		if not kcfgfilename:
			print 'invalid kcfgc file'
			return 0
		source.append(  env.join(str(source[0].get_dir()), kcfgfilename)  )
		return target, source

	env['BUILDERS']['Kcfg']=Builder(action=env.Action(kcfg_buildit, kcfg_stringit),
			emitter=kcfgEmitter, suffix='.h', src_suffix='.kcfgc')
	
	## MOC processing
	env['BUILDERS']['Moc']=Builder(action='$QT_MOC -o $TARGET $SOURCE',suffix='.moc',src_suffix='.h')
	env['BUILDERS']['Moccpp']=Builder(action='$QT_MOC -o $TARGET $SOURCE',suffix='_moc.cpp',src_suffix='.h')

	## DOCUMENTATION
	env['BUILDERS']['Meinproc']=Builder(action='$MEINPROC --check --cache $TARGET $SOURCE',suffix='.cache.bz2')
	## TRANSLATIONS
	env['BUILDERS']['Transfiles']=Builder(action='$MSGFMT $SOURCE -o $TARGET',suffix='.gmo',src_suffix='.po')

	## Handy helpers for building kde programs
	## You should not have to modify them ..

	## Return a list of things
	def make_list(e):
		if type(e) is types.ListType:
			return e
		else:
			return e.split()

	ui_ext = [".ui"]
	header_ext = [".h", ".hxx", ".hpp", ".hh"]
	cpp_ext = [".cpp", ".cxx", ".cc"]

	def QT4files(lenv, target, source):
		""" Returns a list of files for scons (handles qt4 tricks like .qrc) """
		q_object_search = re.compile(r'[^A-Za-z0-9]Q_OBJECT[^A-Za-z0-9]')
		def scan_moc(cppfile):
			addfile=None

			# try to find the header
			orifile=cppfile.srcnode().name
			bs=SCons.Util.splitext(orifile)[0]

			h_file=''
			dir=cppfile.dir
			for n_h_ext in header_ext:
				afile=dir.File(bs+n_h_ext)
				if afile.rexists():
					#h_ext=n_h_ext
					h_file=afile
					break
			# We have the header corresponding to the cpp file
			if h_file:
				h_contents = h_file.get_contents()
				if q_object_search.search(h_contents):
					# we know now there is Q_OBJECT macro
					reg = '\n\s*#include\s*("|<)'+str(bs)+'.moc("|>)'
					meta_object_search = re.compile(reg)
					#cpp_contents = open(file_cpp, 'rb').read()
					cpp_contents=cppfile.get_contents()
					if meta_object_search.search(cpp_contents):
						lenv.Moc(h_file)
					else:
						lenv.Moccpp(h_file)
						addfile=bs+'_moc.cpp'
						print "WARNING: moc.cpp for "+h_file.name+" consider using #include <file.moc> instead"
			return addfile


		src=[]
		ui_files=[]
		other_files=[]

		source_=make_list(source)

		# For each file, check wether it is a dcop file or not, and create the complete list of sources
		for file in source_:
			sfile=SCons.Node.FS.default_fs.File(str(file))
			bs  = SCons.Util.splitext(file)[0]
			ext = SCons.Util.splitext(file)[1]
			if ext == ".moch":
				lenv.Moccpp(bs+'.h')
				src.append(bs+'_moc.cpp')
			elif ext == ".qrc":
				src.append(bs+'_qrc.cpp')
				lenv.Qrc(file)

				from xml.sax import make_parser
				from xml.sax.handler import ContentHandler

				class SconsHandler(ContentHandler):
					def __init__ (self, envi):
						self.files=[]
						self.mstr=''
					def startElement(self, name, attrs):
						if name =='file':
							self.mstr=''
					def endElement(self, name):
						if name =='file':
							self.files.append(self.mstr)
							self.mstr=''
					def characters(self, chars):
						self.mstr+=chars

				parser = make_parser()
				curHandler = SconsHandler(lenv)
				parser.setContentHandler(curHandler)
				parser.parse(open(sfile.srcnode().abspath))
				files=curHandler.files

				lenv.Depends(bs+'_qrc.cpp', files)
#				parser = make_parser()
#				curHandler = SconsHandler(lenv)
#				parser.setContentHandler(curHandler)
#				parser.parse(open(sfile.srcnode().abspath))
#				files=curHandler.files
#				lenv.Depends(bs+'_qrc.cpp', files)
#				for i in files:
#				print "   -> "+i
			elif ext in cpp_ext:
				src.append(file)
				if not env.has_key('NOMOCFILE'):
					ret = scan_moc(sfile)
					if ret: src.append( ret )
			elif ext in ui_ext:
				lenv.Uic(file)
				#src.append(bs+'.cpp')
			else:
				src.append(file)

		# Now check against typical newbie errors
		for file in ui_files:
			for ofile in other_files:
				if ofile == file:
					lenv.pprint('RED', "WARNING: You have included "+file+".ui and another file of the same prefix")
					print "Files generated by uic (file.h, file.cpp must not be included"
		return src


	""" In the future, these functions will contain the code that will dump the
	configuration for re-use from an IDE """
	import glob

	def getInstDirForResType(lenv, restype):
		if len(restype) == 0 or not lenv.has_key(restype):
			lenv.pprint('RED',"unknown resource type "+restype)
			lenv.Exit(1)
		else: instdir = lenv[restype]

		if env['ARGS'] and env['ARGS'].has_key('prefix'):
			instdir = instdir.replace(lenv['PREFIX'], env['ARGS']['prefix'])
		return instdir
		#if restype == "QT4BIN":	return env['PREFIX']+'/bin'
		#if restype == "QT4MODULE" or restype == "QT4LIB": return env['PREFIX']+'/lib'
		#return env['PREFIX']

	def QT4install(lenv, restype, subdir, files):
		if not env['_INSTALL']:
			return
		dir = getInstDirForResType(lenv, restype)
		#install_list = lenv.bksys_install(lenv.join(dir,subdir), files, nodestdir=1)
		install_list = lenv.bksys_install(lenv.join(dir,subdir), files)
		return install_list

	def QT4installas(lenv, restype, destfile, file):
		if not env['_INSTALL']:
			return
		dir = getInstDirForResType(lenv, restype)
		install_list = lenv.InstallAs(lenv.join(dir,destfile), file)
                env.Alias('install', install_list)
		return install_list

	def QT4lang(lenv, folder, appname):
		""" Process translations (.po files) in a po/ dir """
		transfiles = glob.glob(folder+'/*.po')
		for lang in transfiles:
			result = lenv.Transfiles(lang)
			country = SCons.Util.splitext(result[0].name)[0]
			lenv.QT4installas('QT4LOCALE', lenv.join(country,'LC_MESSAGES',appname+'.mo'), result)

	def QT4icon(lenv, icname='*', path='./', restype='QT4ICONS', subdir=''):
		"""Contributed by: "Andrey Golovizin" <grooz()gorodok()net>
		modified by "Martin Ellis" <m.a.ellis()ncl()ac()uk>

		Installs icons with filenames such as cr22-action-frame.png into 
		QT4 icon hierachy with names like icons/crystalsvg/22x22/actions/frame.png.
		
		Global QT4 icons can be installed simply using env.QT4icon('name').
		The second parameter, path, is optional, and specifies the icons
		location in the source, relative to the SConscript file.

		To install icons that need to go under an applications directory (to
		avoid name conflicts, for example), use e.g.
		env.QT4icon('name', './', 'QT4DATA', 'appname/icons')"""

		type_dic = { 'action' : 'actions', 'app' : 'apps', 'device' : 
			'devices', 'filesys' : 'filesystems', 'mime' : 'mimetypes' } 
		dir_dic = {
		'los'  :'locolor/16x16',
		'lom'  :'locolor/32x32',
		'him'  :'hicolor/32x32',
		'hil'  :'hicolor/48x48',
		'lo16' :'locolor/16x16',
		'lo22' :'locolor/22x22',
		'lo32' :'locolor/32x32',
		'hi16' :'hicolor/16x16',
		'hi22' :'hicolor/22x22',
		'hi32' :'hicolor/32x32',
		'hi48' :'hicolor/48x48',
		'hi64' :'hicolor/64x64',
		'hi128':'hicolor/128x128',
		'hisc' :'hicolor/scalable',
		'cr16' :'crystalsvg/16x16',
		'cr22' :'crystalsvg/22x22',
		'cr32' :'crystalsvg/32x32',
		'cr48' :'crystalsvg/48x48',
		'cr64' :'crystalsvg/64x64',
		'cr128':'crystalsvg/128x128',
		'crsc' :'crystalsvg/scalable'
		}

		iconfiles = []
		for ext in "png xpm mng svg svgz".split():
			files = glob.glob(path+'/'+'*-*-%s.%s' % (icname, ext))
			iconfiles += files
		for iconfile in iconfiles:
			lst = iconfile.split('/')
			filename = lst[ len(lst) - 1 ]
			tmp = filename.split('-')
			if len(tmp)!=3:
				lenv.pprint('RED','WARNING: icon filename has unknown format: '+iconfile)
				continue
			[icon_dir, icon_type, icon_filename]=tmp
			try:
				basedir=getInstDirForResType(lenv, restype)
				destfile = '%s/%s/%s/%s/%s' % (basedir, subdir, dir_dic[icon_dir], type_dic[icon_type], icon_filename)
			except KeyError:
				lenv.pprint('RED','WARNING: unknown icon type: '+iconfile)
				continue
			## Do not use QT4installas here, as parsing from an ide will be necessary
			if env['_INSTALL']: env.Alias('install', env.InstallAs( destfile, iconfile ) )

        import generic
        class qt4obj(generic.genobj):
                def __init__(self, val, senv=None):
                        if senv: generic.genobj.__init__(self, val, senv)
                        else: generic.genobj.__init__(self, val, env)
                        self.iskdelib=0
                def execute(self):
                        if self.orenv.has_key('DUMPCONFIG'):
                                print self.xml()
                                return
			self.env=self.orenv.Copy()
			#self.env.AppendUnique(LIBPATH=env['LIBPATH_KDE4'])

			# then add the includes for qt
#			inctypes="QtXml QtGui QtCore QtOpenGL Qt3Support".split()
			qtincludes=[]
#			for dir in inctypes: qtincludes.append( env.join(env['QTINCLUDEPATH'], dir) )
			self.env.AppendUnique(CPPPATH=qtincludes)

                        self.setsource( QT4files(env, self.target, self.source) )
                        generic.genobj.execute(self)

                def xml(self):
                        ret= '<compile type="%s" dirprefix="%s" target="%s" cxxflags="%s" cflags="%s" includes="%s" linkflags="%s" libpaths="%s" libs="%s" vnum="%s" iskdelib="%s" libprefix="%s">\n' % (self.type, self.dirprefix, self.target, self.cxxflags, self.cflags, self.includes, self.linkflags, self.libpaths, self.libs, self.vnum, self.iskdelib, self.libprefix)
                        if self.source:
                                for i in self.orenv.make_list(self.source):
                                        ret += '  <source file="%s"/>\n' % i
                        ret += "</compile>"
                        return ret

	# Attach the functions to the environment so that SConscripts can use them
	from SCons.Script.SConscript import SConsEnvironment
	SConsEnvironment.QT4install = QT4install
	SConsEnvironment.QT4installas = QT4installas
	SConsEnvironment.QT4lang = QT4lang
	SConsEnvironment.QT4icon = QT4icon
	SConsEnvironment.qt4obj=qt4obj

