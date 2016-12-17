import os

import Project
import Tools
import Git
import Svn
import Wine

class QtTriplet:
    def __init__(self, linux = None, macos = None, windows = None):
        self.linux = linux;
        self.macos = macos;
        self.windows = windows;

class QtHost:
    suffixedBinaries = [ 'assistant', 'linguist', 'lupdate', 'pinentry', 'qtconfig', 'designer', 'lrelease', 'moc', 'qmake', 'uic' ]
    
    def __init__(self, project):
        self.useQtSuffix = True if Tools.isLinuxOS() and project.qtLinux.linux == '/usr' else False
        
        if Tools.isLinuxOS():
            self.qt = project.qtLinux
            self.qtHost = self.qt.linux;
            self.linuxMkSpec = '%s/.qt/mkspecs/linux-x11-g++' % ( os.environ[ "HOME" ] )
            self.macosMkSpec = '%s/.qt/mkspecs/macx-x11-g++42' % ( os.environ[ "HOME" ] )
            self.windowsMkSpec = '%s/.qt/mkspecs/win32-x11-g++' % ( os.environ[ 'HOME' ] )
        elif Tools.isMacOS():
            self.qt = project.qtMac
            self.qtHost = self.qt.macos;
            self.linuxMkSpec = '%s/mkspecs/4.6.x/linux-osx-g++' % ( os.environ[ "HOME" ] )
            self.macosMkSpec = '%s/mkspecs/4.6.x/macx-osx-g++42' % ( os.environ[ "HOME" ] )
            self.windowsMkSpec = '%s/mkspecs/4.6.x/win32-osx-g++' % ( os.environ[ 'HOME' ] )
        elif Tools.isWindowsOS():
            self.qt = project.qtWindows
            self.qtHost = self.qt.windows;
    
    def expandVariables(self):
        os.environ[ 'QT_HOST_USE_SUFFIX' ] = '1' if self.useQtSuffix else '0'
        os.environ[ 'QT_LINUX_PATH' ] = self.qt.linux
        os.environ[ 'QT_MACOS_PATH' ] = self.qt.macos
        os.environ[ 'QT_WINDOWS_PATH' ] = self.qt.windows
        os.environ[ 'CROSS_WIN32_QT_PATH' ] = self.qt.windows
    
    def pluginsFilePath(self, name = None, os = None):
        if os == 'linux':
            return '%s/plugins/%s' % ( self.qt.linux, name if name else '' )
        elif os == 'macos':
            return '%s/plugins/%s' % ( self.qt.macos, name if name else '' )
        elif os == 'windows':
            return '%s/plugins/%s' % ( self.qt.windows, name if name else '' )
        return '%s/plugins/%s' % ( self.qtHost, name if name else '' )
        
    
    def suffix(self, command):
        return '-qt4' if self.useQtSuffix and command in QtHost.suffixedBinaries else ''
    
    def bin(self, command):
        return '%s/bin/%s%s' % ( self.qtHost, command, self.suffix( command ) )
