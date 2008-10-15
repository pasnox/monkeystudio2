#!/bin/sh
# Copyright 2008 AZEVEDO Filipe

#### functions ###################################################

getBinaryDependencies()
{
	for n in `otool -LX "${1}" | grep Qt` ; do
		path=`echo $n | grep Qt`
		if [ $path ] ; then
			tmp="$tmp $path"
		fi
	done
	echo $tmp
}

makeInstall()
{
	echo "Running make install"
	if [ -e Makefile.Release ] ; then
		make -f Makefile.Release install
	elif [ -e Makefile ] ; then
		make install
	else
		echo -n "ERROR: Makefile not found. This script requires the macx-g++ makespec"
	fi
	strip "${1}"
}

copyDependencies()
{
	echo "Copying dependencies..."
	mkdir -p "${1}"
	for path in $2 ; do
		name=`basename "${path}"`
		fullname="$FWPATH/$name"
		
		# check for non framework, like libQtCLuScene.4.dylib
		dylib=`expr "${path}" : '.*\(dylib\)$'`
		if [ "x${dylib}" = "x" ] ; then
			fullname="$FWPATH/$name.framework/Versions/4/$name"
		fi
		
		# is symlink ?
		if [ -h "${fullname}" ] ; then
			symlink=`readlink "${fullname}"`
			if [ $symlink[0] = '/' ] ; then
				$fullname=$symlink
			else
				fullname="$FWPATH/$symlink"
			fi
		fi
		
		# is file exists
		if [ ! -f "${fullname}" ] ; then
			echo "ERROR: cannot find $fullname"
			exit
		fi
		
		echo -n "$name framework"
		target="$1/$name"
		cp -f "${fullname}" "${target}"
		# strip libs (-x is max allowable for shared libs)
		strip -x "${target}"
	done
}

echoFrameworks()
{
	echo "Using frameworks..."
	for path in $1 ; do
		name=`basename "${path}"`
		echo -n " $name"
	done
}

setIds()
{
	echo "Setting framework IDs..."
	for path in $1 ; do
		name=`basename "${path}"`
		echo -n " $name"
		target="$2/$name"
		install_name_tool -id @executable_path/../Frameworks/$name "${target}"
	done
}

changeBinaryPaths()
{
	echo "Changing framework paths for `basename "${1}"`..."
	for path in $2 ; do
		name=`basename "${path}"`
		echo -n " $name"
		install_name_tool -change $path @executable_path/../Frameworks/$name "${1}"
	done
}

fixBundledFrameworks()
{
	echo "Fixing bundled frameworks..."
	for fwpath in $1 ; do
		fwname=`basename "${fwpath}"`
		framework="$2/$fwname"
		# get framework dependencies
		deps=`getBinaryDependencies "$framework"`
		changeBinaryPaths "$framework" "$deps" 
	done
}

copyAccessiblePlugins()
{
	echo "Copying accessible plugins..."
	mkdir -p "$1/accessible"
	cp -Rf "$QT_PLUGINS_PATH/accessible" "$1"
	
	for lib in `ls "$1/accessible"` ; do
		lib="$1/accessible/$lib"
		deps=`getBinaryDependencies "$lib"`
		changeBinaryPaths "$lib" "$deps"
		# strip libs (-x is max allowable for shared libs)
		strip -x "${lib}"
	done
}

copyCodecsPlugins()
{
	echo "Copying codecs plugins..."
	mkdir -p "$1/codecs"
	cp -Rf "$QT_PLUGINS_PATH/codecs" "$1"
	
	for lib in `ls "$1/codecs"` ; do
		lib="$1/codecs/$lib"
		deps=`getBinaryDependencies "$lib"`
		changeBinaryPaths "$lib" "$deps"
		# strip libs (-x is max allowable for shared libs)
		strip -x "${lib}"
	done
}

copyDesignerPlugins()
{
	echo "Copying designer plugins..."
	mkdir -p "$1/designer"
	cp -Rf "$QT_PLUGINS_PATH/designer" "$1"
	
	for lib in `ls "$1/designer"` ; do
		lib="$1/designer/$lib"
		deps=`getBinaryDependencies "$lib"`
		changeBinaryPaths "$lib" "$deps"
		# strip libs (-x is max allowable for shared libs)
		strip -x "${lib}"
	done
}

copyIconPlugins()
{
	echo "Copying icon engines plugins..."
	mkdir -p "$1/iconengines"
	cp -Rf "$QT_PLUGINS_PATH/iconengines" "$1"
	
	for lib in `ls "$1/iconengines"` ; do
		lib="$1/iconengines/$lib"
		deps=`getBinaryDependencies "$lib"`
		changeBinaryPaths "$lib" "$deps"
		# strip libs (-x is max allowable for shared libs)
		strip -x "${lib}"
	done
}

copyImagePlugins()
{
	echo "Copying image formats plugins..."
	mkdir -p "$1/imageformats"
	cp -Rf "$QT_PLUGINS_PATH/imageformats" "$1"
	
	for lib in `ls "$1/imageformats"` ; do
		lib="$1/imageformats/$lib"
		deps=`getBinaryDependencies "$lib"`
		changeBinaryPaths "$lib" "$deps"
		# strip libs (-x is max allowable for shared libs)
		strip -x "${lib}"
	done
}

copyInputPlugins()
{
	echo "Copying input methods plugins..."
	mkdir -p "$1/inputmethods"
	cp -Rf "$QT_PLUGINS_PATH/inputmethods" "$1"
	
	exit
	
	for lib in `ls "$1/inputmethods"` ; do
		lib="$1/inputmethods/$lib"
		deps=`getBinaryDependencies "$lib"`
		changeBinaryPaths "$lib" "$deps"
		# strip libs (-x is max allowable for shared libs)
		strip -x "${lib}"
	done
}

copyPhononPlugins()
{
	echo "Copying phonon backend plugins..."
	mkdir -p "$1/phonon_backend"
	cp -Rf "$QT_PLUGINS_PATH/phonon_backend" "$1"
	
	for lib in `ls "$1/phonon_backend"` ; do
		lib="$1/phonon_backend/$lib"
		deps=`getBinaryDependencies "$lib"`
		changeBinaryPaths "$lib" "$deps"
		# strip libs (-x is max allowable for shared libs)
		strip -x "${lib}"
	done
}

copyScriptPlugins()
{
	echo "Copying script plugins..."
	mkdir -p "$1/script"
	cp -Rf "$QT_PLUGINS_PATH/script" "$1"
	
	for lib in `ls "$1/script"` ; do
		lib="$1/script/$lib"
		deps=`getBinaryDependencies "$lib"`
		changeBinaryPaths "$lib" "$deps"
		# strip libs (-x is max allowable for shared libs)
		strip -x "${lib}"
	done
}

copySqlPlugins()
{
	echo "Copying sql drivers plugins..."
	mkdir -p "$1/sqldrivers"
	cp -Rf "$QT_PLUGINS_PATH/sqldrivers" "$1"
	
	for lib in `ls "$1/sqldrivers"` ; do
		lib="$1/sqldrivers/$lib"
		deps=`getBinaryDependencies "$lib"`
		changeBinaryPaths "$lib" "$deps"
		# strip libs (-x is max allowable for shared libs)
		strip -x "${lib}"
	done
}

copyAllPlugins()
{
	copyAccessiblePlugins "$APP_PLUGINS_PATH"
	echo
	copyCodecsPlugins "$APP_PLUGINS_PATH"
	echo
	copyDesignerPlugins "$APP_PLUGINS_PATH"
	echo
	copyIconPlugins "$APP_PLUGINS_PATH"
	echo
	copyImagePlugins "$APP_PLUGINS_PATH"
	echo
	copyInputPlugins "$APP_PLUGINS_PATH"
	echo
	copyPhononPlugins "$APP_PLUGINS_PATH"
	echo
	copyScriptPlugins "$APP_PLUGINS_PATH"
	echo
	copySqlPlugins "$APP_PLUGINS_PATH"
}

#### app name ###################################################

APPNAME=$1

### get system configuration ########################################

# as long as we can find qmake, we don't need QTDIR
FWPATH=`qmake -query QT_INSTALL_LIBS`
if [ ! -d $FWPATH/QtCore.framework ] ; then
	echo "ERROR: cannot find the Qt frameworks. Make sure Qt is installed"
	echo "and qmake is in your environment path."
	exit
fi

### get required user input #########################################
if [ -z "$APPNAME" ] ; then
	echo
	echo "This script prepares a Qt application bundle for deployment. It will"
	echo "copy over the required Qt frameworks and sets the installation"
	echo "identifications. Please see the \"Deploying an Application on Qt/Mac\""
	echo "page in the Qt documentation for more information."
	echo
	echo "This script assumes you have already built the application bundle."
	echo
	echo -n "What is the name of the application? "
	read userinput
	APPNAME=$userinput
fi

BUNDLE="$APPNAME.app"
APPBIN="$BUNDLE/Contents/MacOS/`basename \"${APPNAME}\"`"
APP_PLUGINS_PATH="$BUNDLE/Contents/plugins"
APP_QT_PLUGINS_PATH="$BUNDLE/Contents/plugins/qt"
FRAMEWORKS_PATH="$BUNDLE/Contents/Frameworks"
QT_PLUGINS_PATH=`qmake -query QT_INSTALL_PLUGINS`

if [ ! -d "${BUNDLE}" ] ; then
	echo "ERROR: cannot find application bundle \"$BUNDLE\" in current directory"
	exit
fi

if [ ! -x "${APPBIN}" ] ; then
	echo "ERROR: cannot find application \"$APPBIN\" in bundle. Did you forget to run make?"
	exit
fi

echo "application: ${APPNAME}"
echo "bundle:      ${BUNDLE}"
echo


### make install ####################################################
#makeInstall "$APPBIN"
echo

getSourceFramework()
{
	framework="$FWPATH/$1"
		
	# check for non framework, like libQtCLuScene.4.dylib
	dylib=`expr "${framework}" : '.*\(dylib\)$'`
	if [ "x${dylib}" = "x" ] ; then
		framework="$FWPATH/$1.framework/Versions/4/$1"
	fi
	
	# is symlink ?
	if [ -h "${framework}" ] ; then
		symlink=`readlink "${framework}"`
		if [ $symlink[0] = '/' ] ; then
			$framework=$symlink
		else
			framework="$FWPATH/$symlink"
		fi
	fi
	echo "$framework"
}

getTargetFramework()
{
	echo "$FRAMEWORKS_PATH/$1"
}

relinkBinary()
{	
	# strip libs (-x is max allowable for shared libs)
	strip -x "${1}"
	
	# get dependencies
	frameworks_path=`getBinaryDependencies "$1"`
	
	# update framework/library paths
	changeBinaryPaths "$1" "$frameworks_path"
	echo
	
	# copy dependencies frameworks/libraries
	for framework_path in $frameworks_path ; do
		# get framework
		framework=`basename "${framework_path}"`
		
		# get filenames
		source=`getSourceFramework "$framework"`
		target=`getTargetFramework "$framework"`
		
		# copy file if needed
		if [ -e "${source}" ] ; then
			if [ ! -e "${target}" ] ; then
				echo "Copying & striping `basename \"${source}\"` framework/library..."
				path=`dirname "${target}"`
				mkdir -p "${path}"
				cp -f "${source}" "${path}"
				# strip libs (-x is max allowable for shared libs)
				strip -x "${target}"
				echo
				
				# set id in target
				echo "Updating $framework id..."
				install_name_tool -id @executable_path/../Frameworks/"${framework}" "${target}"
				echo
				
				# update framework/library paths
				changeBinaryPaths "$target" "`getBinaryDependencies "$target"`"
				echo
			fi
		fi
	done
}

relinkPlugins()
{
	tmp_plugins=`find "${1}" | egrep ".dylib"`
	
	old_ifs="$IFS"
	IFS=$'\n'
	count=0
	for plugin in $tmp_plugins ; do
		plugins[$count]="${plugin}"
		((count++))
	done
	IFS=$old_ifs
	
	count=${#plugins[@]}
	for ((i=0;i<$count;i++));
	do
		plugin=${plugins[${i}]}
		relinkBinary "$plugin"
	done
}

relinkBinary "$APPBIN"
relinkPlugins "$APP_PLUGINS_PATH"

exit

### query binary for frameworks #####################################
FRAMEWORKS=`getBinaryDependencies "$APPBIN"`
echoFrameworks "$FRAMEWORKS"
echo

### copy over frameworks ############################################
copyDependencies "$FRAMEWORKS_PATH" "$FRAMEWORKS"
echo

# remove unwanted parts
find "${FRAMEWORKS_PATH}" | egrep "debug|Headers" | xargs rm -rf

### set the identification names for frameworks #####################
setIds "$FRAMEWORKS" "$FRAMEWORKS_PATH"
echo

### change framework paths ##########################################
changeBinaryPaths "$APPBIN" "$FRAMEWORKS"
echo

### change location for bundled frameworks #########################
fixBundledFrameworks "$FRAMEWORKS" "$FRAMEWORKS_PATH"
echo

### copy plugins ###############################################
#copyAllPlugins
copyAccessiblePlugins "$APP_PLUGINS_PATH"
echo

### misc cleanup ###############################################
target="$BUNDLE/Contents"
find "${target}" | egrep "CVS" | xargs rm -rf
find "${target}" | egrep ".svn" | xargs rm -rf

### create disk image ###############################################
#echo "Creating disk image"
#imagedir="/tmp/$APPNAME.$$"
#mkdir $imagedir
#cp -R $BUNDLE $imagedir

# TODO: copy over additional files, if any
#hdiutil create -ov -srcfolder $imagedir -format UDBZ -volname "$APPNAME" "$APPNAME.dmg"
#hdiutil internet-enable -yes "$APPNAME.dmg"
#rm -rf $imagedir

echo "Done"