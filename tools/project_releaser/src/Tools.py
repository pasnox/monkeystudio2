import os
import shlex
import shutil
import platform
import subprocess
import commands
import glob
import re
import fnmatch

import Project
import Qt
import Git
import Svn
import Wine

def isLinuxOS():
    return platform.system() == 'Linux'

def isMacOS():
    return platform.system() == 'Darwin'

def isWindowsOS():
    return platform.system() == 'Windows'

def grep(string, list):
    expr = re.compile( string )
    return filter( expr.search, list )

def banner(string, ch = '=', length = 80):
    text = ' %s ' % ( string )
    text = text.center( length, ch )
    print ch *length
    print text
    print ch *length

def deleteIfExists(filePath):
    if filePath == os.environ[ 'HOME' ]:
        return False
    if not os.path.exists( filePath ):
        return True
    if os.path.isdir( filePath ) and not os.path.islink( filePath ):
        shutil.rmtree( filePath )
    else:
        os.remove( filePath )
    return not os.path.exists( filePath )

def createDirectory(filePath):
    if os.path.exists( filePath ):
        if os.path.isdir( filePath ):
            return True
        else:
            return False
    os.makedirs( filePath )
    return os.path.exists( filePath )

def createSymLink(source, target):
    if os.path.exists( target ):
        if os.path.islink( target ):
            return os.path.realpath( target ) == source
        return False
    os.symlink( source, target )
    return os.path.exists( target )

def wildCardCopy(source, target):
    for path in glob.iglob( source ):
        if not copy( path, target ):
            return False
    return True

def copy(source, target):
    if '*' in source or '?' in source:
        return wildCardCopy( source, target )
    
    if not os.path.exists( source ):
        return False
    
    sourceBasename = os.path.basename( source )
    targetBasename = os.path.basename( target )
    
    if not createDirectory( os.path.dirname( target ) ):
        return False
    
    if os.path.isdir( source ):
        targetPath = target
        
        if os.path.exists( target ):
            if sourceBasename == targetBasename:
                return False
            
            targetPath = '%s/%s' % ( target, os.path.basename( source ) )
        
            if not createDirectory( target ):
                return False
        
        shutil.copytree( source, targetPath, True )
    else:
        targetPath = target
        
        if sourceBasename == targetBasename:
            targetPath = os.path.dirname( targetPath )
        
        if not createDirectory( targetPath ):
            return False
        
        shutil.copy2( source, targetPath )
    
    return os.path.exists( target )

def wildCardMove(source, target):
    for path in glob.iglob( source ):
        if not move( path, target ):
            return False
    return True

def move(source, target):
    if '*' in source or '?' in source:
        return wildCardMove( source, target )
    if not os.path.exists( target ):
        if not createDirectory( target ):
            return False
    shutil.move( source, target )
    return os.path.exists( '%s/%s' % ( target, os.path.basename( source ) ) )

def getFilesList(path, pattern = None, recursive = False):
    files = []
    for file in os.listdir( path ):
        file = '%s/%s' % ( path, file )
        if os.path.isfile( file ) and ( not pattern or fnmatch.fnmatch( file, pattern ) ):
            files.append( os.path.normpath( file ) )
        elif recursive and os.path.isdir( file ):
            files.extend( getFilesList( file, pattern, recursive ) )
    return files

def getFoldersList(path, pattern = None, recursive = False):
    files = []
    for file in os.listdir( path ):
        file = '%s/%s' % ( path, file )
        if os.path.isdir( file ) and ( not pattern or fnmatch.fnmatch( file, pattern ) ):
            files.append( os.path.normpath( file ) )
            if recursive:
                files.extend( getFoldersList( file, pattern, recursive ) )
    return files

def execute(command, workingDirectory = None, showError = True, showExecInfo = True, usePipe = True):
    if showExecInfo:
        if workingDirectory:
            print ' - From: %s' % ( workingDirectory )
        print ' - Executing: %s' % command
    args = shlex.split( command )
    if usePipe:
        runner = subprocess.Popen( args, cwd = workingDirectory, stdout = subprocess.PIPE, stderr= subprocess.STDOUT, shell = False )
    else:
        runner = subprocess.Popen( args, cwd = workingDirectory, shell = False )
    output, error = runner.communicate()
    if runner.returncode != 0:
        if showError:
            print ' - Exit Code: %i' % ( runner.returncode )
            print output
        return False
    return True

def executeAndGetOutput(command, workingDirectory = None, showError = False, showExecInfo = True):
    if showExecInfo:
        if workingDirectory:
            print ' - From: %s' % ( workingDirectory )
        print ' - Executing: %s' % command
    args = shlex.split( command )
    runner = subprocess.Popen( args, cwd = workingDirectory, stdout = subprocess.PIPE, stderr= subprocess.STDOUT, shell = False )
    output, error = runner.communicate()
    if runner.returncode != 0:
        if showError:
            print ' - Exit Code: %i' % ( runner.returncode )
            print output
    return ( runner.returncode == 0, output )

def tarGzFolder(source, target, options = None):
    directory = os.path.dirname( source )
    folderName = os.path.basename( source )
    return execute( 'tar czf "%s" "%s"' % ( target, folderName ), directory )

def zipFolder(source, target, options = None, exclude = None, include = None):
    directory = os.path.dirname( source )
    folderName = os.path.basename( source )
    e = '-x %s' % ( exclude ) if exclude else ''
    i = '-i %s' % ( include ) if include else ''
    o = '"%s"' % ( options ) if options else ''
    return execute( 'zip -q -r -9 %s "%s" "%s" %s %s' % ( o, target, folderName, e, i ), directory )

def buildQtProject(projectFilePath, qtHost, qtMkSpec):
    directory = os.path.dirname( projectFilePath )
    filename = os.path.basename( projectFilePath )
    jobs = os.environ[ 'PROJECT_MAKE_JOBS' ]
    make = 'make -j %s' % ( jobs if jobs else '1' )
    makeInstall = 'make -j %s install kleen' % ( jobs if jobs else '1' )
    qmake = '"%s" -r %s "%s"' % ( qtHost.bin( 'qmake' ), '-spec "%s"' %( qtMkSpec ) if qtMkSpec else '', filename )
    
    if not os.path.exists( directory ):
        return False
    
    if os.path.exists( '%s/bin' % ( os.path.dirname( projectFilePath ) ) ):
        if not deleteIfExists( '%s/bin' % ( os.path.dirname( projectFilePath ) ) ):
            return False
    
    # not important if it fails
    execute( '%s distclean' % ( make ), directory, False )
    
    if not execute( qmake, directory ):
        return False
    
    if not execute( '%s distclean' % ( make ), directory ):
        return False
    
    if not execute( qmake, directory ):
        return False
    
    if not execute( make, directory ):
        return False
    
    execute( makeInstall, directory )
    
    return True
