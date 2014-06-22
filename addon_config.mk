meta:
	ADDON_NAME = ofxPro
	ADDON_DESCRIPTION = Prototyping Enviroment
	ADDON_AUTHOR = Patricio Gonzalez Vivo
	ADDON_TAGS = UI Animation Record timeline
	ADDON_URL = http://github.com/patriciogonzalezvivo/ofxPro

common:
	# dependencies with other addons, a list of them separated by spaces 
	# or use += in several lines
	ADDON_DEPENDENCIES = ofxXmlSettings
	ADDON_DEPENDENCIES += ofxUI
	
	# include search paths, this will be usually parsed from the file system
	# but if the addon or addon libraries need special search paths they can be
	# specified here separated by spaces or one per line using +=
	ADDON_INCLUDES =
	
	# any special flag that should be passed to the compiler when using this
	# addon
	ADDON_CFLAGS =
	
	# any special flag that should be passed to the linker when using this
	# addon, also used for system libraries with -lname
	ADDON_LDFLAGS =
	
	# linux only, any library that should be included in the project using
	# pkg-config
	ADDON_PKG_CONFIG_LIBRARIES =
	
	# osx/iOS only, any framework that should be included in the project
	ADDON_FRAMEWORKS =
	
	# source files, these will be usually parsed from the file system looking
	# in the src folders in libs and the root of the addon. if your addon needs
	# to include files in different places or a different set of files per platform
	# they can be specified here
	ADDON_SOURCES =
	
	# binary libraries, these will be usually parsed from the file system but some 
	# libraries need to passed to the linker in a specific order 
	ADDON_LIBS =
	
	# some addons need resources to be copied to the bin/data folder of the project
	# specify here any files that need to be copied, you can use wildcards like * and ?
	ADDON_DATA = copy_to_data/*
	
linux64:
linux:
linuxarmv6l:
linuxarmv7l:
osx:
win_cb:
vs2010:
iphone:
android:
