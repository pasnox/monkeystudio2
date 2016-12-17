import os
import sys
import string
import tempfile

import Project
import Tools
import Qt
import Git
import Svn
import Wine

class MacOSBundle:
    suffixedBinaries = [ 'ld', 'otool' ]
    libMask = '*.dylib'
    debugMask = '_debug'
    isDebug = False
    
    def __init__(self, qtHost, bundlePath, deployAllQtPlugins = False, stripArchList = [ 'x86_64', 'ppc7400', 'ppc64' ]): # [ 'x86_64', 'i386', 'ppc7400', 'ppc64' ]
        self.qt = qtHost
        self.bundlePath = os.path.normpath( bundlePath )
        self.deployAllQtPlugins = deployAllQtPlugins
        self.stripArchList = stripArchList
        self.binariesPath = '/opt/mac/bin' if Tools.isLinuxOS() else '/usr/bin'
        self.binariesPrefix = 'i686-apple-darwin10-' if Tools.isLinuxOS() else ''
        self.use64BitsBinaries = True if Tools.isLinuxOS() else False
        self.fixedFilePathsList = {}
        self.dependenciesLinkList = {}
        self.dependenciesList = set()
    
    def suffix(self, command):
        return '64' if self.use64BitsBinaries and command in MacOSBundle.suffixedBinaries else ''
    
    def bin(self, command):
        return os.path.normpath( '%s/%s%s%s' % ( self.binariesPath, self.binariesPrefix, command, self.suffix( command ) ) )
    
    def pluginsFilePath(self):
        return os.path.normpath( '%s/Contents/plugins' % ( self.bundlePath ) )
    
    def frameworksFilePath(self):
        return os.path.normpath( '%s/Contents/Frameworks' % ( self.bundlePath ) )
    
    def resourcesFilePath(self):
        return os.path.normpath( '%s/Contents/Resources' % ( self.bundlePath ) )
    
    def qtPluginsFilePath(self, name = None):
        return os.path.normpath( '%s/qt/plugins/%s' % ( self.resourcesFilePath(), name if name else '' ) )
    
    def binaryFilePath(self):
        fileName = os.path.splitext( os.path.basename( self.bundlePath ) )[ 0 ]
        return os.path.normpath( '%s/Contents/MacOS/%s' % ( self.bundlePath, fileName ) )
    
    def binaries(self):
        binaries = Tools.getFilesList( '%s/Contents/MacOS' % ( self.bundlePath ), "*", True )
        binaries += Tools.getFilesList( self.pluginsFilePath(), self.libMask, True )
        return set( binaries )
    
    def getDependencies(self, binaryFilePath):
        # return cached dependencies if possible
        if binaryFilePath in self.dependenciesLinkList.keys():
            return self.dependenciesLinkList[ binaryFilePath ]
        
        items = set()
        fileName = os.path.basename( binaryFilePath )
        command = '"%s" -LX "%s"' % ( self.bin( 'otool' ), binaryFilePath )
        ok, output = Tools.executeAndGetOutput( command, showExecInfo = False, showError = True )
        
        if ok:
            for line in set( output.splitlines() ):
                line = line.strip();
                tmp = line.lower()
                
                # system frameworks are skipped
                if tmp.startswith( '/usr/lib' ) or tmp.startswith( '/system/library' ):
                    continue
                
                index = tmp.rfind( '(compatibility' )
                filePath = line[ : index ].strip()
                
                # accept only if base names are differents
                if fileName == os.path.basename( filePath ) or '@' in filePath:
                    continue
                
                # transform file path if needed
                fixedFilePath = filePath
                
                if not os.path.exists( fixedFilePath ):
                    if 'Qt' in fixedFilePath or 'phonon' in fixedFilePath:
                        dependencyName = os.path.basename( fixedFilePath )
                        index = fixedFilePath.find( '.framework' )
                        
                        if index != -1:
                            fixedFilePath = os.path.normpath( '%s/lib/%s%s' % ( self.qt.qt.macos, dependencyName, fixedFilePath[ index : ] ) )
                        else:
                            fixedFilePath = os.path.normpath( '%s/lib/%s' % ( self.qt.qt.macos, dependencyName ) )
                
                self.fixedFilePathsList[ fixedFilePath ] = filePath
                
                # add dependency
                items.add( fixedFilePath )
                items.update( self.getDependencies( fixedFilePath ) )
        
        # retains dependencies for future calls
        self.dependenciesLinkList[ binaryFilePath ] = items
        
        # dependencies computed !
        return items
    
    def getArchList(self, binaryFilePath):
        items = set()
        lipo = self.bin( 'lipo' )
        command = '"%s" -info "%s"' % ( lipo, binaryFilePath )
        ok, output = Tools.executeAndGetOutput( command, showError = True, showExecInfo = False )
        
        if ok:
            line = output.strip()
            lower = line.lower()
            
            if lower.startswith( 'non-fat file:' ) or lower.startswith( 'architectures in the fat file:' ):
                index = line.rfind( ':' ) +1
                line = line[ index : ].strip()
                items = set( line.split() )
        
        return items
    
    def relinkBinary(self, binaryFilePath):
        print 'Relinking %s...' % ( binaryFilePath )
        
        fileName = os.path.basename( binaryFilePath )
        install = self.bin( 'install_name_tool' )
        lipo = self.bin( 'lipo' )
        idCommand = '"%s" -id "@executable_path/../Frameworks/%s" "%s"' % ( install, fileName, binaryFilePath )
        
        # change id for frameworks / lib in @executable_path/../Frameworks path
        if os.path.dirname( binaryFilePath ).lower().endswith( 'frameworks' ):
            if not Tools.execute( idCommand, showError = True, showExecInfo = False ):
                return False
        
        # change linking of each dependencies
        dependencies = self.getDependencies( binaryFilePath )
        
        for dependency in dependencies:
            originalFilePath = self.fixedFilePathsList[ dependency ]
            fileName = os.path.basename( originalFilePath )
            changeCommand = '"%s" -change "%s" "@executable_path/../Frameworks/%s" "%s"' % ( install, originalFilePath, fileName, binaryFilePath )
            
            if not Tools.execute( changeCommand, showError = True, showExecInfo = False ):
                return False
        
        # strip arch list
        archList = list( self.getArchList( binaryFilePath ) )
        
        # no arch to remove
        if len( archList ) == 1 and not archList[ 0 ] in self.stripArchList:
            return True
        
        # remove each arch to strip
        for arch in self.stripArchList:
            if arch in archList:
                lipoCommand = '"%s" "%s" -remove %s -output "%s"' % ( lipo, binaryFilePath, arch, binaryFilePath )
                
                if not Tools.execute( lipoCommand, showError = True, showExecInfo = False ):
                    return False
        
        # all is fine
        return True
    
    def buildDependencies(self, show = False):
        # clear cached content
        self.fixedFilePathsList = {}
        self.dependenciesLinkList = {}
        self.dependenciesList = set()
        
        # get files to scan
        binaries = self.binaries()
        
        # build dependencies links - aka libs/frameworks binaries depends on
        for binary in binaries:
            self.getDependencies( binary )
        
        # build dependencies - aka libs/frameworks to copy / relink for the given bundle
        for filePath in self.dependenciesLinkList.keys():
            self.dependenciesList.update( self.dependenciesLinkList[ filePath ] )
        
        # get all qt plugins or only those having all dependencies available
        qtPlugins = Tools.getFilesList( self.qt.pluginsFilePath( os = 'macos' ), self.libMask, True )
        self.isDebug = self.debugMask in string.join( self.getDependencies( self.binaryFilePath() ) )
        
        for qtPlugin in qtPlugins:
            dependencies = self.getDependencies( qtPlugin )
            ok = True
            
            # check plugin dependencies are available
            if not self.deployAllQtPlugins:
                for dependency in dependencies:
                    if not dependency in self.dependenciesList:
                        ok = False
                        break
            
            # do not copy plugin
            if not ok:
                continue
            
            # take care of copying only debug files if needed, or only release one
            if ( self.isDebug and self.debugMask in qtPlugin ) or ( not self.isDebug and not self.debugMask in qtPlugin ):
                self.dependenciesList.add( qtPlugin )
                self.dependenciesList.update( self.dependenciesLinkList[ qtPlugin ] )
        
        # show computed things
        if show:
            print self.dependenciesLinkList
            print self.fixedFilePathsList
            print self.dependenciesList
            print self.isDebug
    
    def deploy(self, show = False):
        self.buildDependencies( show )
        
        # files to relink
        binaries = self.binaries()
        
        for binary in binaries:
            if not self.relinkBinary( binary ):
                return False
        
        # files to copy and relink
        qtPluginsPath = self.qt.pluginsFilePath( os = 'macos' )
        start = len( qtPluginsPath )
        bundleFrameworksPath = self.frameworksFilePath()
        bundleQtPluginsPath = self.qtPluginsFilePath()
        binaries = self.dependenciesList
        
        for binary in binaries:
            target = ''
            
            # qt plugin
            if binary.startswith( qtPluginsPath ):
                target = '%s/%s' % ( bundleQtPluginsPath, binary[ start : ] )
            # lib/framework
            else:
                target = '%s/%s' % ( bundleFrameworksPath, os.path.basename( binary ) )
            
            target = os.path.normpath( target )
            
            # copy dependency
            if not os.path.exists( target ):
                print 'Copying dependency %s to %s...' % ( os.path.basename( binary ), target )
                
                if not Tools.copy( binary, target ):
                    return False
            
            # relink dependency
            if not self.relinkBinary( target ):
                return False
        
        # all is fine !
        return True
    
    def createDmg(self, filePath):
        volumeName = os.path.splitext( os.path.basename( self.bundlePath ) )[ 0 ]
        mountPoint = tempfile.mkdtemp()
        
        if Tools.isMacOS():
            return False
        
        if Tools.isWindowsOS():
            return False
        
        if not Tools.deleteIfExists( filePath ):
            return False
        
        # get content size to put in dmg
        ok, output = Tools.executeAndGetOutput( 'du -sk "%s"' % ( self.bundlePath ) )
        
        if not ok:
            return False
        
        # get data size
        size = int( output.strip().split( '\t' )[ 0 ] )
        # add 1% of the original size for the file system
        size *= 1.01
        
        # create dmg file
        if not Tools.execute( 'dd if=/dev/zero of="%s" bs=1024 count=%i' % ( filePath, size ) ):
            return False
        
        # format dmg
        if not Tools.execute( '/sbin/mkfs.hfsplus -v "%s" "%s"' % ( volumeName, filePath ) ):
            return False
        
        # mount dmg
        if not Tools.execute( 'sudo mount -o loop,rw,uid=pasnox,sync,umask=0000 -t hfsplus "%s" "%s"' % ( filePath, mountPoint ) ):
            return False
        
        # copy stuff in dmg
        if not Tools.copy( '%s/../*' % ( self.bundlePath ), mountPoint ):
            Tools.execute( 'sudo umount "%s"' % ( mountPoint ) )
            return False
        
        # umount dmg
        if not Tools.execute( 'sudo umount "%s"' % ( mountPoint ) ):
            return False
        
        # delete mount point
        Tools.deleteIfExists( mountPoint )
        
        # zip resulted dmg as it's not comrpessed
        if Tools.zipFolder( filePath, '%s.zip' % ( filePath ) ):
            Tools.deleteIfExists( filePath )
        
        # rock and roll !!
        return True
