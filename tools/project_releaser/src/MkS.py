import tempfile
import shutil
import os

import Project
import Tools
import Qt
import Git
import Svn
import Wine
import MacOSBundle
import GoogleCodeUpload

class MkSProject(Project.Project):
    def __init__(self):
        Project.Project.__init__(self)
        
        self.success = False
        
        self.shortName = 'mks'
        self.name = 'Monkey Studio IDE'
        self.version = '1.9.0.4'
        #self.version = 'dev'
        #self.version = 'trunk'
        self.company = 'The Monkey Studio Team'
        self.copyrights = '2005 - 2012 Azevedo Filipe & The Monkey Studio Team'
        self.description = 'Free, Fast and Flexible cross-platform IDE'
        self.urlHomepage = 'http://monkeystudio.org'
        self.urlForums = '%s/forum' % ( self.urlHomepage )
        self.urlIssuesTracker = 'https://bugs.launchpad.net/monkeystudio/+filebug'
        self.target = '%s/Releases/%s' % ( os.environ[ 'HOME' ], self.shortName )
        
        self.makeJobs = 4
        
        self.qtLinux = Qt.QtTriplet(
            #'%s/Developpement/OS/Unix-Libraries/Qt/4.8.3' % ( os.environ[ 'HOME' ] ), # linux
            '/usr', # linux
            '%s/Developpement/OS/OSX-Libraries/Qt/4.8.3-carbon' % ( os.environ[ 'HOME' ] ), # mac
            '%s/Developpement/OS/Win32-Libraries/Qt/4.8.3' % ( os.environ[ 'HOME' ] ) # windows
        )
        
        self.qtMac = Qt.QtTriplet(
            '', # linux
            '/usr/local/Trolltech/4.7.4-carbon-lgpl', # mac
            '/usr/local/Trolltech/win32/4.7.4' # windows
        )
        
        self.qt = Qt.QtHost( self )
        
        self.wine = Wine.Wine()
        
        self.svnList[ 'mks' ] = Svn.Svn( 'svn://svn.tuxfamily.org/svnroot/monkeystudio/mks' )
        
        if Tools.isLinuxOS():
            self.svnList[ 'mks' ].workingCopy = '%s/Developpement/C++/Qt4/mks' % ( os.environ[ 'HOME' ] )
        elif Tools.isMacOS():
            self.svnList[ 'mks' ].workingCopy = '%s/Developpement/mks' % ( os.environ[ 'HOME' ] )
        
        self.gitList[ 'fresh' ] = Git.Git( 'git://github.com/pasnox/fresh.git' )
        
        # Custom variables
        self.baseName = '%s_%s' % ( self.shortName, self.version )
        self.sourceName = '%s-src' % ( self.baseName )
        self.tmpFolder = tempfile.mkdtemp()
        self.logFolder = '%s/log' % ( self.tmpFolder )
        self.packagesFolder = '%s/packages' % ( self.tmpFolder )
        self.sourcesFolder = '%s/%s' % ( self.tmpFolder, self.sourceName )
        self.projectFile = '%s/monkeystudio.pro' % ( self.sourcesFolder )
        self.tgzFile = '%s/%s.tar.gz' % ( self.packagesFolder, self.sourceName )
        self.zipFile = '%s/%s.zip' % ( self.packagesFolder, self.sourceName )
        self.winSetup = '%s/%s.exe' % ( self.packagesFolder, self.baseName )
        self.winZip = '%s/%s-win32.zip' % ( self.packagesFolder, self.baseName )
        self.macDmg = '%s/%s.dmg' % ( self.packagesFolder, self.baseName )
        self.macZip = '%s/%s-mac.zip' % ( self.packagesFolder, self.baseName )
        
        os.environ[ 'PWD' ] = self.tmpFolder
        os.chdir( self.tmpFolder )
    
    def __del__(self):
        Project.Project.__del__(self)
        
        if self.success:
            Tools.deleteIfExists( self.tmpFolder )
        
        Tools.banner( 'Releasing finished (%s)' % ( 'Ok' if self.success else 'Fails' ) )
    
    def expandVariables(self):
        Project.Project.expandVariables(self)
        os.environ[ 'PROJECT_BASE_NAME' ] = self.baseName
        os.environ[ 'PROJECT_SOURCE_NAME' ] = self.sourceName
        os.environ[ 'PROJECT_TMP_FOLDER' ] = self.tmpFolder
        os.environ[ 'PROJECT_LOG_FOLDER' ] = self.logFolder
        os.environ[ 'PROJECT_PACKAGES_FOLDER' ] = self.packagesFolder
        os.environ[ 'PROJECT_SOURCES_FOLDER' ] = self.sourcesFolder
        os.environ[ 'PROJECT_PROJECT_FILE' ] = self.projectFile
        os.environ[ 'PROJECT_TGZ_FILE' ] = self.tgzFile
        os.environ[ 'PROJECT_ZIP_FILE' ] = self.zipFile
        os.environ[ 'PROJECT_WIN_SETUP' ] = self.winSetup
        os.environ[ 'PROJECT_WIN_ZIP' ] = self.winZip
        os.environ[ 'PROJECT_MAC_DMG' ] = self.macDmg
        os.environ[ 'PROJECT_MAC_ZIP' ] = self.macZip
        
        # Windows Setup expands
        os.environ[ 'QT_WINDOWS_MINGW_DOWNLOAD' ] = 'http://get.qt.nokia.com/qt/source/qt-win-opensource-4.7.5-mingw.exe'
        os.environ[ 'WINDOWS_MINGW_DOWNLOAD' ] = 'http://get.qt.nokia.com/misc/MinGW-gcc440_1.zip'
        os.environ[ 'QT_DOWNLOAD' ] = 'http://qt.nokia.com/downloads'
        os.environ[ 'WINDOWS_DLLS_PATH' ] = '%s/Developpement/OS/Win32-Libraries/bin' % ( os.environ[ 'HOME' ] )
        os.environ[ 'SETUP_ICON' ] = '%s/tools/data/windows/setup.ico' % ( self.sourcesFolder )
        os.environ[ 'SETUP_LEFT_BANNER' ] = '%s/tools/data/windows/left-banner.bmp' % ( self.sourcesFolder )
        os.environ[ 'SETUP_TOP_BANNER' ] = '%s/tools/data/windows/top-banner.bmp' % ( self.sourcesFolder )
        os.environ[ 'SETUP_SOURCE_DIRECTORY' ] = self.sourcesFolder
        os.environ[ 'SETUP_OUTPUT_DIRECTORY' ] = self.packagesFolder
        os.environ[ 'SETUP_OUTPUT_NAME' ] = os.path.splitext( os.path.basename( self.winSetup ) )[ 0 ]
        os.environ[ 'SCRIPT_PATH' ] = '%s/tools/data/windows' % ( self.sourcesFolder )
    
    def getPackageInformations(self, filePath):
        fileName = os.path.basename( filePath )
        summary = None
        labels = None
        
        # Sources
        if fileName in self.tgzFile:
            summary = 'Source Tgz Archive %s' % ( self.version )
            labels = [ 'Type-Source', 'OpSys-All' ]
        elif fileName in self.zipFile:
            summary = 'Source Zip Archive %s' % ( self.version )
            labels = [ 'Type-Source', 'OpSys-All' ]
        # Windows
        elif fileName in self.winSetup:
            summary = 'Windows Installer %s' % ( self.version )
            labels = [ 'Type-Installer', 'OpSys-Windows' ]
        elif fileName in self.winZip:
            summary = 'Windows Zip Archive (Portable App) %s' % ( self.version )
            labels = [ 'Type-Archive', 'OpSys-Windows' ]
        # Mac OSX
        elif fileName in self.macDmg or fileName in '%s.zip' % ( self.macDmg ):
            summary = 'Mac OS X i386 Package %s' % ( self.version )
            labels = [ 'Type-Package', 'OpSys-OSX' ]
        elif fileName in self.macZip:
            summary = 'Mac OS X i386 Archive %s' % ( self.version )
            labels = [ 'Type-Archive', 'OpSys-OSX' ]
        
        if self.version != 'dev':
            labels.append( 'Status-Stable' )
        
        if self.version != 'trunk' and self.version != 'dev' and '.' in self.version:
            labels.append( 'Featured' )
        
        return summary, labels
    
    def run(self):
        self.success = False
        
        Tools.banner( 'Releasing "%s" using tmp folder "%s"' % ( self.name, self.tmpFolder ) )
        
        Tools.banner( 'Expanding variables...' )
        self.expandVariables()
        
        Tools.banner( 'Creating directories...' )
        Tools.createDirectory( self.target )
        Tools.createDirectory( self.logFolder )
        Tools.createDirectory( self.packagesFolder )
        
        # check target files existance
        tgzFileExists = os.path.exists( '%s/%s' % ( self.target, os.path.basename( self.tgzFile ) ) )
        zipFileExists = os.path.exists( '%s/%s' % ( self.target, os.path.basename( self.zipFile ) ) )
        winSetupFileExists = os.path.exists( '%s/%s' % ( self.target, os.path.basename( self.winSetup ) ) )
        winZipFileExists = os.path.exists( '%s/%s' % ( self.target, os.path.basename( self.winZip ) ) )
        macDmgFileExists = os.path.exists( '%s/%s' % ( self.target, os.path.basename( self.macDmg ) ) )
        macZipDmgFileExists = os.path.exists( '%s/%s.zip' % ( self.target, os.path.basename( self.macDmg ) ) )
        macZipFileExists = os.path.exists( '%s/%s' % ( self.target, os.path.basename( self.macZip ) ) )
        
        Tools.banner( 'Exporting sources...' )
        tools = 'v2/tools'
        if self.version == 'dev':
            sources = 'v2/branches/dev'
        elif self.version == 'trunk':
            sources = 'v2/trunk'
        else:
            sources = 'v2/tags/version-%s' % ( self.version )
        if not self.svnList[ 'mks' ].export( sources, self.sourcesFolder, True ):
            print 'Can\'t export sources'
            return
        if not self.svnList[ 'mks' ].export( tools, '%s/tools' % ( self.sourcesFolder ), True ):
            print 'Can\'t export tools'
            return
        
        Tools.banner( 'Creating sources packages...' )
        if tgzFileExists:
            print ' - Skipping creation of %s' % ( self.tgzFile )
        else:
            if not Tools.tarGzFolder( self.sourcesFolder, self.tgzFile ):
                print 'Can\'t create tgz file'
                return
        if zipFileExists:
            print ' - Skipping creation of %s' % ( self.zipFile )
        else:
            if not Tools.zipFolder( self.sourcesFolder, self.zipFile ):
                print 'Can\'t create zip file'
                return
        
        Tools.banner( 'Crossbuilding for windows...' )
        if winSetupFileExists and winZipFileExists:
            print ' - Skipping crossbuild for windows'
        else:
            if not Tools.buildQtProject( self.projectFile, self.qt, self.qt.windowsMkSpec ):
                print 'Can\'t build for windows'
                return
        
        Tools.banner( 'Creating windows setup...' )
        if winSetupFileExists:
            print ' - Skipping creation of %s' % ( self.winSetup )
        else:
            if not self.wine.iscc( '%s%s/tools/data/windows/monkeystudio.iss' % ( self.wine.rootDrive, self.sourcesFolder ) ):
                print 'Can\'t create windows setup'
                return
        
        Tools.banner( 'Creating windows zip...' )
        if winZipFileExists:
            print ' - Skipping creation of %s' % ( self.winZip )
        else:
            if not self.wine.isccSetupToZip( self.winSetup, self.winZip, self.name ):
                print 'Can\'t create windows zip'
                return
        
        '''Tools.banner( 'Crossbuilding for mac os x...' )
        if ( macDmgFileExists or macZipDmgFileExists ) and macZipFileExists:
            print ' - Skipping crossbuild for mac os x'
        else:
            if not Tools.buildQtProject( self.projectFile, self.qt, self.qt.macosMkSpec ):
                print 'Can\'t build for mac os x'
                return
        
        Tools.banner( 'Relinking the mac os x bundle' )
        bundle = MacOSBundle.MacOSBundle( self.qt, '%s/bin/MonkeyStudio.app' % ( self.sourcesFolder ), True )
        if macDmgFileExists or macZipDmgFileExists:
            print ' - Skipping relinking of mac os x bundle'
        else:
            if not bundle.deploy():
                print 'Can\'t deploy'
                return
        
        Tools.banner( 'Creating mac dmg...' )
        if macDmgFileExists or macZipDmgFileExists:
            print ' - Skipping creation of %s' % ( self.macDmg )
        else:
            if not bundle.createDmg( self.macDmg ):
                print 'Can\'t create mac dmg'
                return
        
        Tools.banner( 'Creating mac zip...' )
        if macZipFileExists:
            print ' - Skipping creation of %s' % ( self.macZip )
        else:
            if not Tools.zipFolder( '%s/bin/.' % ( self.sourcesFolder ), self.macZip ):
                print 'Can\'t create mac zip'
                return'''
        
        Tools.banner( 'Moving packages...' )
        Tools.createDirectory( self.target )
        for fileName in os.listdir( self.packagesFolder ):
            sourceFilePath = '%s/%s' % ( self.packagesFolder, fileName )
            targetFilePath = '%s/%s' % ( self.target, fileName )
            if not os.path.isdir( sourceFilePath ):
                if Tools.deleteIfExists( targetFilePath ):
                    shutil.move( sourceFilePath, targetFilePath )
        
        Tools.banner( 'Uploading packages...' )
        gcu = GoogleCodeUpload.GoogleCodeUpload( 'monkeystudio' )
        for fileName in os.listdir( self.target ):
            print ' - Uploading %s...' % ( fileName )
            sourceFilePath = '%s/%s' % ( self.target, fileName )
            summary, labels = self.getPackageInformations( sourceFilePath )
            if not gcu.upload( sourceFilePath, summary, labels):
                print 'Can\'t upload %s' % ( fileName )
                return
        
        self.success = True
    