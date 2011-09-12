#!/bin/sh
# Copyright 2008 AZEVEDO Filipe

#### app name ###################################################

APP_NAME=$1
QMAKE=$2

if [ ! -f "$QMAKE" ] ; then
    QMAKE=qmake
fi

### get system configuration ########################################

# as long as we can find qmake, we don't need QTDIR
QT_LIBS_PATH=`$QMAKE -query QT_INSTALL_LIBS`
QT_PLUGINS_PATH=`$QMAKE -query QT_INSTALL_PLUGINS`

if [ ! -d $QT_LIBS_PATH/QtCore.framework ] ; then
    echo "ERROR: cannot find the Qt frameworks. Make sure Qt is installed"
    echo "and qmake is in your environment path."
    exit
fi

### get required user input #########################################
if [ -z "$APP_NAME" ] ; then
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
    APP_NAME=$userinput
fi

APP_BASENAME=$(basename "$APP_NAME")
BUNDLE="$APP_NAME.app"
APP_BIN="$BUNDLE/Contents/MacOS/$APP_BASENAME"
APP_PLUGINS_PATH="$BUNDLE/Contents/plugins"
APP_QT_PLUGINS_PATH="$BUNDLE/Contents/Resources/qt/plugins"
APP_FRAMEWORKS_PATH="$BUNDLE/Contents/Frameworks"

if [ ! -d "${BUNDLE}" ] ; then
    echo "ERROR: cannot find application bundle \"$BUNDLE\" in current directory"
    exit
fi

if [ ! -x "${APP_BIN}" ] ; then
    echo "ERROR: cannot find application \"$APP_BIN\" in bundle. Did you forget to run make?"
    exit
fi

echo "application: ${APP_NAME}"
echo "bundle:      ${BUNDLE}"
echo

### functions ######################################################
copyPlugins()
{
    path="$2/$1"
    if [ ! -d "$QT_PLUGINS_PATH/${1}" ] ; then
        echo "Skipping $1 plugins, folder does not exists"
    else
        echo "Copying $1 plugins..."
        if [ ! -d "${path}" ] ; then
            mkdir -p "${path}"
        fi
        cp -R "$QT_PLUGINS_PATH/${1}" "${2}"
    fi
}

copyAllPlugins()
{
    copyPlugins "accessible" "$APP_QT_PLUGINS_PATH"
    copyPlugins "codecs" "$APP_QT_PLUGINS_PATH"
    copyPlugins "designer" "$APP_QT_PLUGINS_PATH"
    copyPlugins "graphicssystems" "$APP_QT_PLUGINS_PATH"
    copyPlugins "iconengines" "$APP_QT_PLUGINS_PATH"
    copyPlugins "imageformats" "$APP_QT_PLUGINS_PATH"
    copyPlugins "inputmethods" "$APP_QT_PLUGINS_PATH"
    copyPlugins "phonon_backend" "$APP_QT_PLUGINS_PATH"
    copyPlugins "script" "$APP_QT_PLUGINS_PATH"
    copyPlugins "sqldrivers" "$APP_QT_PLUGINS_PATH"
}

changeBinaryPaths()
{
    echo "Changing framework paths for `basename "${1}"`..."
    for path in $2 ; do
        name=$(basename "$path")
        echo -n " $name"
        install_name_tool -change $path @executable_path/../Frameworks/$name "${1}"
    done
}

getSourceFramework()
{
    framework="$QT_LIBS_PATH/$1"
        
    # check for non framework, like libQtCLuScene.4.dylib
    dylib=`expr "${framework}" : '.*\(dylib\)$'`
    if [ "x${dylib}" = "x" ] ; then
        framework="$QT_LIBS_PATH/$1.framework/Versions/4/$1"
    fi
    
    # is symlink ?
    if [ -h "${framework}" ] ; then
        symlink=`readlink "${framework}"`
        if [ $symlink[0] = '/' ] ; then
            $framework=$symlink
        else
            framework="$QT_LIBS_PATH/$symlink"
        fi
    fi
    
    echo "$framework"
}

getTargetFramework()
{
    echo "$APP_FRAMEWORKS_PATH/$1"
}

getBinaryDependencies()
{
    tmp=""
    for n in `otool -LX "${1}" | grep 'Qt\|phonon'` ; do
        path=`echo $n | grep 'Qt\|phonon'`
        if [ $path ] ; then
            tmp="$tmp $path"
        fi
    done
    
    echo $tmp
}

setId()
{
    echo "Updating $1 id..."
    install_name_tool -id @executable_path/../Frameworks/"${1}" "${2}"
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
        echo
    done
}

relinkBinary()
{
    echo "Striping `basename \"${1}\"` binary..."
    # strip libs (-x is max allowable for shared libs)
    #strip -x "${1}"
    
    # set id in target
    if [ ! "x${2}" = "x" ] ; then
        setId "${2}" "${1}"
    fi
    
    # get dependencies
    frameworks_path=`getBinaryDependencies "$1"`
    
    # update framework/library paths
    changeBinaryPaths "$1" "$frameworks_path"
    
    # copy dependencies frameworks/libraries
    for framework_path in $frameworks_path ; do
        # get framework
        framework=$(basename "$framework_path")
        
        # get filenames
        source=`getSourceFramework "$framework"`
        target=`getTargetFramework "$framework"`
        
        # copy file if needed
        if [ -e "${source}" ] ; then
            if [ ! -e "${target}" ] ; then
                #echo "Copying & striping `basename \"${source}\"` framework/library..."
                path=`dirname "${target}"`
                mkdir -p "${path}"
                cp -f "${source}" "${target}"
                
                echo
                relinkBinary "${target}" "${framework}"
            fi
        fi
    done
}

createTmpDmg()
{
    echo "Creating temporary dmg..."
    hdiutil create -megabytes 200 tmp.dmg -layout NONE -fs HFS+ -volname "$APP_BASENAME" -ov
    tmp=`hdid tmp.dmg`
    disk=`echo $tmp | cut -f 1 -d\ `
}

prepareDmg()
{
    # Copy qt files
    echo "Copying qt files"
    cp "qt.conf" "$BUNDLE/Contents/Resources"
    if [ ! -d "$BUNDLE/Contents/Resources/qt/doc/qch" ] ; then
        mkdir -p "$BUNDLE/Contents/Resources/qt/doc/qch"
    fi
    cp -f "${QT_LIBS_PATH}/../doc/qch/"*.qch "$BUNDLE/Contents/Resources/qt/doc/qch"
    if [ ! -d "$BUNDLE/Contents/Resources/qt/translations" ] ; then
        mkdir -p "$BUNDLE/Contents/Resources/qt/translations"
    fi
    cp -f "$QT_LIBS_PATH/../translations/"* "$BUNDLE/Contents/Resources/qt/translations"
    # Copy original bundle
    echo "Copying $APP_BASENAME..."
    cp -fR "$BUNDLE" "/Volumes/$APP_BASENAME"
    
    # Copy standard files
    echo "Copying standard files..."
    cp -f ../../dev-readme "/Volumes/$APP_BASENAME"
    cp -f ../../GPL-2 "/Volumes/$APP_BASENAME"
    cp -f ../../GPL-3 "/Volumes/$APP_BASENAME"
    cp -f ../../LGPL-3 "/Volumes/$APP_BASENAME"
    cp -f ../../readme.txt "/Volumes/$APP_BASENAME"
    
    # cleanup
    echo "Cleaning up..."
    curPath=$PWD
    target="$BUNDLE/Contents"
    cd "$target"
    find . | egrep "CVS" | xargs rm -rf
    find . | egrep ".svn" | xargs rm -rf
    cd "$curPath"
}

createReleaseDmg()
{
    echo "Creating release dmg..."
    hdiutil eject "$disk"
    if [ -f "../${APP_BASENAME}_VERSION-svnRELEASE.dmg" ] ; then
        rm -f "../${APP_BASENAME}_VERSION-svnRELEASE.dmg"
    fi
    hdiutil convert -format UDZO tmp.dmg -o "../${APP_BASENAME}_VERSION-svnRELEASE.dmg"
    rm tmp.dmg
}

# Let go...
relinkBinary "$APP_BIN"
echo

copyAllPlugins
echo

echo "Cleaning debug libraries..."
cleanCurPath=$PWD
cleanTarget="$BUNDLE/Contents"
cd "$cleanTarget"
find . | egrep "*_debug.dylib" | xargs rm -rf
cd "$cleanCurPath"
echo "-n Finished."
echo

relinkPlugins "$APP_QT_PLUGINS_PATH"
echo

relinkPlugins "$APP_PLUGINS_PATH"
echo

createTmpDmg
echo

prepareDmg
echo

createReleaseDmg
echo

echo "Done"
