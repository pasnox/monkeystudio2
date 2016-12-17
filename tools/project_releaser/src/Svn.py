import commands

import Project
import Tools
import Qt
import Git
import Wine

class Svn:
    def __init__(self, url, workingCopy = None, login = None, password = None):
        self.url = url
        self.workingCopy = workingCopy
        self.login = login
        self.password = password
    
    def checkout(self, url, filePath, fromWorkingCopy = False):
        return Svn.staticCheckout( '%s/%s' % ( self.workingCopy, url ), filePath )
    
    def export(self, url, filePath, fromWorkingCopy = False):
        return Svn.staticExport( '%s/%s' % ( self.workingCopy if fromWorkingCopy else self.url, url ), filePath )
    
    @staticmethod
    def staticCheckout(url, filePath):
        return False
    
    @staticmethod
    def staticExport(url, filePath):
        Tools.deleteIfExists( filePath )
        return Tools.execute( 'svn export "%s" "%s"' % ( url, filePath ) )
    
    def workingCopyRevision(filePath):
        status, output = commands.getstatusoutput( 'export LANG=C && [ -f /usr/bin/svnversion ] && svnversion -c "$1" | sed "s/[^:]*:\([0-9]*\)[a-zA-Z]*/\1/"' )
        return output if os.WEXITSTATUS( status ) == 0 else '#Error'
