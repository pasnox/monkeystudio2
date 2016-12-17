import os
import glob

import Project
import Tools
import Qt
import Git
import Svn

class Wine:
    def __init__(self):
        if Tools.isLinuxOS():
            self.wine = 'wine'
            self.prefix = '%s/.wine' % ( os.environ[ 'HOME' ] )
        elif Tools.isMacOS():
            self.wine = '/Applications/Wine.app/Contents/Resources/bin/wine'
            self.prefix = '%s/Wine Files' % ( os.environ[ 'HOME' ] )
        elif Tools.isWindowsOS():
            '''No wine needed'''
        
        self.rootDrive = 'z:'
        self.drive = '%s/drive_c' % ( self.prefix )
        self.programFiles = '%s/Program Files' % ( self.drive )
    
    def start(self, command, workingDirectory = None):
        return Tools.execute( '"%s" %s' % ( self.wine, command ), workingDirectory )
    
    def iscc(self, scriptFile, workingDirectory = None):
        return self.start( '"%s/Inno Setup 5/ISCC.exe" "%s"' % ( self.programFiles, scriptFile ), workingDirectory )
    
    def isccInstall(self, filePath):
        if not os.path.exists( filePath ):
            return False
        return self.start( '"%s%s" /silent' % ( self.rootDrive, filePath ) )
    
    def isccUninstall(self, filePath):
        if not os.path.exists( filePath ):
            return False
        if os.path.isdir( filePath ):
            for file in glob.glob( '%s/unins*.exe' % ( filePath ) ):
                if not self.isccUninstall( file ):
                    return False
            return True
        else:
            return self.start( '"%s%s" /silent' % ( self.rootDrive, filePath ) )
    
    def isccSetupToZip(self, setupFilePath, zipFilePath, defaultInstallDirectory):
        pf = '%s/%s' % ( self.programFiles, defaultInstallDirectory )
        sl = os.path.splitext( zipFilePath )[ 0 ]
        self.isccUninstall( pf )
        if not self.isccInstall( setupFilePath ):
            return False
        ok = Tools.createSymLink( pf, sl )
        if ok:
            ok = Tools.zipFolder( sl, zipFilePath, exclude = '*unins*.*' )
        Tools.deleteIfExists( sl )
        self.isccUninstall( pf )
        return ok
    
    def expandVariables(self):
        os.environ[ 'WINE_BINARY' ] = self.wine
        os.environ[ 'WINEPREFIX' ] = self.prefix # official used variable by wine binary
        os.environ[ 'WINE_PREFIX' ] = self.prefix
        os.environ[ 'WINE_ROOT_DRIVE' ] = self.rootDrive
        os.environ[ 'WINE_DRIVE' ] = self.drive
        os.environ[ 'WINE_PROGRAM_FILES' ] = self.programFiles