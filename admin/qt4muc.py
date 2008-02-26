#!/usr/bin/python

import re
import os.path

include_re = re.compile(r'\n\#include \"([a-zA-Z0-9._]*)\"', re.M)

import SCons.Builder
import SCons.Tool

def exists( env ):
	return True

def moc_emitter( target, source, env ):
	base = str(source[0]).split(".")[0:-1][0]
	contents = source[0].get_contents();
	if contents.count( "\n#include \""+base+".moc\"" ):
		#print "moc_emitter on " + str(source[0])
		header = base+".h"
		moc = base+".moc"
		env.MOCBuilder( source=header, target=moc )
	return (target,source)

def uic_scanner( node, env, path ):
	#print "uic_scanner: " + str(node).split(".")[-1]
	if str(node).split(".")[-1] == "moc":
		#print "mocbuilder"
		header = str(node).split(".")[0:-1][0] + ".h"
		env.MOCBuilder( source=header, target=str(node) )
		return [ str(node).split("/")[-1].split(".")[0:-1][0] + ".h" ]

	contents = node.get_contents()
	includes = include_re.findall(contents)
	if str(node).split(".")[-1] == "cpp":
		#print "uic_scanner processing a cpp-file " + str(node)
		ret = []
		for inc in includes:
			#print "Testing " + inc
			if os.path.exists( os.path.join( str(node.dir), inc ) ) and inc.endswith( ".h" ):
				#print " Found a normal include: " + inc
				ret.append( inc )
			if inc.endswith( ".moc" ):
				#print " Found a moc include: " + inc
				ret.append( inc )
		#print "uic_scanner returning" + str(ret)
		return ret

	if str(node).split(".")[-1] == "h":
		#print "uic_scanner processing a h-file " + str(node)
		ret = []
		for inc in includes:
			header_to_check = os.path.join(str(node.dir),inc)
			#print "Header to check: " + header_to_check
			ui_to_check = header_to_check.split(".")[0:-1][0] +".ui"
			#print "Ui to check: " + ui_to_check

			if inc.endswith( ".moc" ):
				#print " Found a moc include: " + inc
				ret.append( inc )

			if os.path.exists( ui_to_check ):
				#print "Header doesn't exist but .ui file does!"
				env.UIBuilder( target = header_to_check, source = ui_to_check )
				#print [ header_to_check.split("/")[-1] ]
				ret.append( header_to_check.split("/")[-1] )

		#if ret != []:
		#	print "uic_scanner returning " + str(ret)
		return ret

	print "\nWarning! was called with an unhandled suffix: " + str(node).split(".")[-1] + " !\n"
	return []

def GetAppVersion( context, app, version ):
	import os

	context.Message( "Checking if the output of '%s' contains '%s' " % (app,version) )

	out = os.popen3( app )
	#print out[1] + out[2]
	str = out[1].read() + out[2].read()

	ret = False

	if str.count( version ) > 0:
		ret = True

	context.Result( ret )
	return ret

def generate( env ):
	print "Configuring qt4muc..."

	conf = env.Configure( custom_tests = { 'GetAppVersion' : GetAppVersion } )

	def CommandFromList( commandlist, version ):
		ret = ""
		for command in commandlist:
			if len( ret ) == 0:
				if conf.GetAppVersion( "%s -v" % command, version ):
					ret = command
		return ret

	moc = CommandFromList( ( "moc", "moc4", "moc-qt4" ), "Qt 4." )
	if len( moc ) < 3:
		env.Exit( 1 )

	uic = CommandFromList( ( "uic", "uic4", "uic-qt4" ), "4." )
	if len( uic ) < 3:
		env.Exit( 1 )

	rcc = CommandFromList( ( "rcc", "rcc4", "rcc-qt4" ), "4." )
	if len( rcc ) < 3:
		env.Exit( 1 )

	env = conf.Finish()
	print "Done. Will define a more or less automatic environment to do all the qt-specific stuff."

	moc = SCons.Builder.Builder( action="%s $SOURCES > $TARGET" % moc, suffix='.moc', src_suffix='.h'   )
	uic = SCons.Builder.Builder( action="%s $SOURCES > $TARGET" % uic, suffix='.h',   src_suffix='.ui'  )
	rcc = SCons.Builder.Builder( action="%s $SOURCES > $TARGET" % rcc, suffix='.cpp', src_suffix=".qrc" )

	env['BUILDERS']['UIBuilder'] = uic
	env['BUILDERS']['MOCBuilder'] = moc
	env['BUILDERS']['RCCBUILDER'] = rcc

	env['BUILDERS']['StaticObject'].add_emitter( ".cpp", moc_emitter )
	env['BUILDERS']['SharedObject'].add_emitter( ".cpp", moc_emitter )

	SCons.Tool.SourceFileScanner.add_scanner( '.cpp', SCons.Scanner.Scanner( uic_scanner, skeys=[".h"], recursive=1 ) )

#!/usr/bin/python

import re
import os.path

include_re = re.compile(r'\n\#include \"([a-zA-Z0-9._]*)\"', re.M)

import SCons.Builder
import SCons.Tool

def exists( env ):
	return True

def moc_emitter( target, source, env ):
	base = str(source[0]).split(".")[0:-1][0]
	contents = source[0].get_contents();
	if contents.count( "\n#include \""+base+".moc\"" ):
		#print "moc_emitter on " + str(source[0])
		header = base+".h"
		moc = base+".moc"
		env.MOCBuilder( source=header, target=moc )
	return (target,source)

def uic_scanner( node, env, path ):
	#print "uic_scanner: " + str(node).split(".")[-1]
	if str(node).split(".")[-1] == "moc":
		#print "mocbuilder"
		header = str(node).split(".")[0:-1][0] + ".h"
		env.MOCBuilder( source=header, target=str(node) )
		return [ str(node).split("/")[-1].split(".")[0:-1][0] + ".h" ]

	contents = node.get_contents()
	includes = include_re.findall(contents)
	if str(node).split(".")[-1] == "cpp":
		#print "uic_scanner processing a cpp-file " + str(node)
		ret = []
		for inc in includes:
			#print "Testing " + inc
			if os.path.exists( os.path.join( str(node.dir), inc ) ) and inc.endswith( ".h" ):
				#print " Found a normal include: " + inc
				ret.append( inc )
			if inc.endswith( ".moc" ):
				#print " Found a moc include: " + inc
				ret.append( inc )
		#print "uic_scanner returning" + str(ret)
		return ret

	if str(node).split(".")[-1] == "h":
		#print "uic_scanner processing a h-file " + str(node)
		ret = []
		for inc in includes:
			header_to_check = os.path.join(str(node.dir),inc)
			#print "Header to check: " + header_to_check
			ui_to_check = header_to_check.split(".")[0:-1][0] +".ui"
			#print "Ui to check: " + ui_to_check

			if inc.endswith( ".moc" ):
				#print " Found a moc include: " + inc
				ret.append( inc )

			if os.path.exists( ui_to_check ):
				#print "Header doesn't exist but .ui file does!"
				env.UIBuilder( target = header_to_check, source = ui_to_check )
				#print [ header_to_check.split("/")[-1] ]
				ret.append( header_to_check.split("/")[-1] )

		#if ret != []:
		#	print "uic_scanner returning " + str(ret)
		return ret

	print "\nWarning! was called with an unhandled suffix: " + str(node).split(".")[-1] + " !\n"
	return []

def GetAppVersion( context, app, version ):
	import os

	context.Message( "Checking if the output of '%s' contains '%s' " % (app,version) )

	out = os.popen3( app )
	#print out[1] + out[2]
	str = out[1].read() + out[2].read()

	ret = False

	if str.count( version ) > 0:
		ret = True

	context.Result( ret )
	return ret

def generate( env ):
	print "Configuring qt4muc..."

	conf = env.Configure( custom_tests = { 'GetAppVersion' : GetAppVersion } )

	def CommandFromList( commandlist, version ):
		ret = ""
		for command in commandlist:
			if len( ret ) == 0:
				if conf.GetAppVersion( "%s -v" % command, version ):
					ret = command
		return ret

	moc = CommandFromList( ( "moc", "moc4", "moc-qt4" ), "Qt 4." )
	if len( moc ) < 3:
		env.Exit( 1 )

	uic = CommandFromList( ( "uic", "uic4", "uic-qt4" ), "4." )
	if len( uic ) < 3:
		env.Exit( 1 )

	rcc = CommandFromList( ( "rcc", "rcc4", "rcc-qt4" ), "4." )
	if len( rcc ) < 3:
		env.Exit( 1 )

	env = conf.Finish()
	print "Done. Will define a more or less automatic environment to do all the qt-specific stuff."

	moc = SCons.Builder.Builder( action="%s $SOURCES > $TARGET" % moc, suffix='.moc', src_suffix='.h'   )
	uic = SCons.Builder.Builder( action="%s $SOURCES > $TARGET" % uic, suffix='.h',   src_suffix='.ui'  )
	rcc = SCons.Builder.Builder( action="%s $SOURCES > $TARGET" % rcc, suffix='.cpp', src_suffix=".qrc" )

	env['BUILDERS']['UIBuilder'] = uic
	env['BUILDERS']['MOCBuilder'] = moc
	env['BUILDERS']['RCCBUILDER'] = rcc

	env['BUILDERS']['StaticObject'].add_emitter( ".cpp", moc_emitter )
	env['BUILDERS']['SharedObject'].add_emitter( ".cpp", moc_emitter )

	SCons.Tool.SourceFileScanner.add_scanner( '.cpp', SCons.Scanner.Scanner( uic_scanner, skeys=[".h"], recursive=1 ) )

#!/usr/bin/python

import re
import os.path

include_re = re.compile(r'\n\#include \"([a-zA-Z0-9._]*)\"', re.M)

import SCons.Builder
import SCons.Tool

def exists( env ):
	return True

def moc_emitter( target, source, env ):
	base = str(source[0]).split(".")[0:-1][0]
	contents = source[0].get_contents();
	if contents.count( "\n#include \""+base+".moc\"" ):
		#print "moc_emitter on " + str(source[0])
		header = base+".h"
		moc = base+".moc"
		env.MOCBuilder( source=header, target=moc )
	return (target,source)

def uic_scanner( node, env, path ):
	#print "uic_scanner: " + str(node).split(".")[-1]
	if str(node).split(".")[-1] == "moc":
		#print "mocbuilder"
		header = str(node).split(".")[0:-1][0] + ".h"
		env.MOCBuilder( source=header, target=str(node) )
		return [ str(node).split("/")[-1].split(".")[0:-1][0] + ".h" ]

	contents = node.get_contents()
	includes = include_re.findall(contents)
	if str(node).split(".")[-1] == "cpp":
		#print "uic_scanner processing a cpp-file " + str(node)
		ret = []
		for inc in includes:
			#print "Testing " + inc
			if os.path.exists( os.path.join( str(node.dir), inc ) ) and inc.endswith( ".h" ):
				#print " Found a normal include: " + inc
				ret.append( inc )
			if inc.endswith( ".moc" ):
				#print " Found a moc include: " + inc
				ret.append( inc )
		#print "uic_scanner returning" + str(ret)
		return ret

	if str(node).split(".")[-1] == "h":
		#print "uic_scanner processing a h-file " + str(node)
		ret = []
		for inc in includes:
			header_to_check = os.path.join(str(node.dir),inc)
			#print "Header to check: " + header_to_check
			ui_to_check = header_to_check.split(".")[0:-1][0] +".ui"
			#print "Ui to check: " + ui_to_check

			if inc.endswith( ".moc" ):
				#print " Found a moc include: " + inc
				ret.append( inc )

			if os.path.exists( ui_to_check ):
				#print "Header doesn't exist but .ui file does!"
				env.UIBuilder( target = header_to_check, source = ui_to_check )
				#print [ header_to_check.split("/")[-1] ]
				ret.append( header_to_check.split("/")[-1] )

		#if ret != []:
		#	print "uic_scanner returning " + str(ret)
		return ret

	print "\nWarning! was called with an unhandled suffix: " + str(node).split(".")[-1] + " !\n"
	return []

def GetAppVersion( context, app, version ):
	import os

	context.Message( "Checking if the output of '%s' contains '%s' " % (app,version) )

	out = os.popen3( app )
	#print out[1] + out[2]
	str = out[1].read() + out[2].read()

	ret = False

	if str.count( version ) > 0:
		ret = True

	context.Result( ret )
	return ret

def generate( env ):
	print "Configuring qt4muc..."

	conf = env.Configure( custom_tests = { 'GetAppVersion' : GetAppVersion } )

	def CommandFromList( commandlist, version ):
		ret = ""
		for command in commandlist:
			if len( ret ) == 0:
				if conf.GetAppVersion( "%s -v" % command, version ):
					ret = command
		return ret

	moc = CommandFromList( ( "moc", "moc4", "moc-qt4" ), "Qt 4." )
	if len( moc ) < 3:
		env.Exit( 1 )

	uic = CommandFromList( ( "uic", "uic4", "uic-qt4" ), "4." )
	if len( uic ) < 3:
		env.Exit( 1 )

	rcc = CommandFromList( ( "rcc", "rcc4", "rcc-qt4" ), "4." )
	if len( rcc ) < 3:
		env.Exit( 1 )

	env = conf.Finish()
	print "Done. Will define a more or less automatic environment to do all the qt-specific stuff."

	moc = SCons.Builder.Builder( action="%s $SOURCES > $TARGET" % moc, suffix='.moc', src_suffix='.h'   )
	uic = SCons.Builder.Builder( action="%s $SOURCES > $TARGET" % uic, suffix='.h',   src_suffix='.ui'  )
	rcc = SCons.Builder.Builder( action="%s $SOURCES > $TARGET" % rcc, suffix='.cpp', src_suffix=".qrc" )

	env['BUILDERS']['UIBuilder'] = uic
	env['BUILDERS']['MOCBuilder'] = moc
	env['BUILDERS']['RCCBUILDER'] = rcc

	env['BUILDERS']['StaticObject'].add_emitter( ".cpp", moc_emitter )
	env['BUILDERS']['SharedObject'].add_emitter( ".cpp", moc_emitter )

	SCons.Tool.SourceFileScanner.add_scanner( '.cpp', SCons.Scanner.Scanner( uic_scanner, skeys=[".h"], recursive=1 ) )

