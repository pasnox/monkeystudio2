import os

import Tools

'''
    GoogleCodeUpload class is a poo wrapper around commandline googlecode_upload.pl tool
    http://code.google.com/p/googlecode-upload
'''
class GoogleCodeUpload:
    def __init__(self, projectName = None, googleCodeScript = None):
        self.googleCodeScript = googleCodeScript if googleCodeScript else 'googlecode_upload.pl'
        self.project = projectName
        self.user = os.environ[ 'USER' ]
        self.password = None
    
    def generateCommandLine(self, parameters):
        options = []
        
        for key in parameters.keys():
            value = parameters[ key ]
            
            if key == 'pass' and not value:
                continue
            
            if key == 'user' and not value and 'GOOGLECODE_USER' in os.environ.keys():
                value = os.environ[ 'GOOGLECODE_USER' ]
            
            if value and ' ' in value:
                value = '"%s"' % ( value )
            
            if not value:
                options.append( '--%s' % ( key ) )
            else:
                options.append( '--%s=%s' % ( key, value ) )
            
        return '"%s" %s' % ( self.googleCodeScript, ' '.join( options ) )

    def version(self):
        ok, output = Tools.executeAndGetOutput( self.generateCommandLine( { 'version': None } ), showExecInfo = False )
        return output.split( ' ' )[ 1 ].strip()[ 1: ] if ok else '?.??'
    
    def upload(self, filePath, summary, labels, progress = True):
        parameters = {}
        parameters[ 'project' ] = self.project
        parameters[ 'user' ] = self.user
        parameters[ 'pass' ] = self.password
        parameters[ 'file' ] = filePath
        parameters[ 'summary' ] = summary
        parameters[ 'labels' ] = ','.join( labels )
        
        if progress:
            parameters[ 'progress' ] = None
        
        return Tools.execute( self.generateCommandLine( parameters ), usePipe = not progress )
