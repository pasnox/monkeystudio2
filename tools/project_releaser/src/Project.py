import os

import Tools
import Qt
import Git
import Svn
import Wine

class Project:
    def __init__(self):
        # Project
        self.shortName = None
        self.name = None
        self.version = None
        self.company = None
        self.copyrights = None
        self.description = None
        self.urlHomepage = None
        self.urlForums = None
        self.urlIssuesTracker = None
        self.target = None
        
        # Build
        self.makeJobs = 1
        
        # Qt triplets
        self.qtLinux = None
        self.qtMac = None
        self.qtWindows = None
        
        # QtHost
        self.qt = None
        
        self.wine = None
        self.svnList = {}
        self.gitList = {}

    def __del__(self):
        '''Cleanup'''
    
    def expandVariables(self):
        os.environ[ 'PROJECT_SHORT_NAME' ] = self.shortName
        os.environ[ 'PROJECT_NAME' ] = self.name
        os.environ[ 'PROJECT_VERSION' ] = self.version
        os.environ[ 'PROJECT_COMPANY' ] = self.company
        os.environ[ 'PROJECT_COPYRIGHTS' ] = self.copyrights
        os.environ[ 'PROJECT_DESCRIPTION' ] = self.description
        os.environ[ 'PROJECT_URL_HOMEPAGE' ] = self.urlHomepage
        os.environ[ 'PROJECT_URL_FORUMS' ] = self.urlForums
        os.environ[ 'PROJECT_URL_ISSUES_TRACKER' ] = self.urlIssuesTracker
        os.environ[ 'PROJECT_TARGET' ] = self.target
        os.environ[ 'PROJECT_MAKE_JOBS' ] = str( self.makeJobs )
        
        if self.qt:
            self.qt.expandVariables()
        
        if self.wine:
            self.wine.expandVariables()
    
    def run(self):
        Tools.banner( 'Reimplement your own Project::run() function' )
