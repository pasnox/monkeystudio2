#!/usr/bin/python
import sys
import os
import tempfile

import Tools

def unpackPaxFile(filePath, targetPath):
    print '- Unpacking %s...' % ( filePath )
    tmpFolder = tempfile.mkdtemp()
    tmpFilePath = '%s/%s' % ( tmpFolder, os.path.basename( filePath ) )
    gunzip = 'gunzip "%s"' % ( tmpFilePath )
    pax = 'pax -r -v -s \',^/,,\' -f "%s/%s"' % ( tmpFolder, os.path.splitext( os.path.basename( filePath ) )[ 0 ] )
    if not Tools.copy( filePath, tmpFolder ):
        Tools.deleteIfExists( tmpFolder )
        return False
    ok, output = Tools.executeAndGetOutput( gunzip, tmpFolder, True, True )
    if ok:
        ok, output = Tools.executeAndGetOutput( pax, targetPath, True, True )
    Tools.deleteIfExists( tmpFolder )
    return ok

def conformToStandardQtInstall( targetPath ):
    print '- Conforming to standard install %s...' % ( targetPath )
    if not Tools.move( '%s/Library/Frameworks/*' % ( targetPath ), '%s/usr/lib' % ( targetPath ) ):
        return False
    if not Tools.deleteIfExists( '%s/Library' % ( targetPath ) ):
        return False
    
    if not Tools.move( '%s/Developer/Applications/Qt/imports' % ( targetPath ), '%s/usr' % ( targetPath ) ):
        return False
    if not Tools.move( '%s/Developer/Applications/Qt/plugins' % ( targetPath ), '%s/usr' % ( targetPath ) ):
        return False
    if not Tools.move( '%s/Developer/Applications/Qt/translations' % ( targetPath ), '%s/usr' % ( targetPath ) ):
        return False
    if not Tools.move( '%s/Developer/Applications/Qt/phrasebooks' % ( targetPath ), '%s/usr' % ( targetPath ) ):
        return False
    if not Tools.move( '%s/Developer/Applications/Qt/*' % ( targetPath ), '%s/usr/bin' % ( targetPath ) ):
        return False
    if not Tools.move( '%s/Developer/Documentation/Qt/*' % ( targetPath ), '%s/usr/doc' % ( targetPath ) ):
        return False
    if not Tools.deleteIfExists( '%s/Developer' % ( targetPath ) ):
        return False
    
    if not Tools.move( '%s/usr/local/Qt*/mkspecs' % ( targetPath ), '%s/usr' % ( targetPath ) ):
        return False
    if not Tools.deleteIfExists( '%s/usr/local' % ( targetPath ) ):
        return False
    
    if not Tools.move( '%s/usr/*' % ( targetPath ), targetPath ):
        return False
    if not Tools.deleteIfExists( '%s/usr' % ( targetPath ) ):
        return False
    
    if not Tools.deleteIfExists( '%s/mkspecs/default' % ( targetPath ) ):
        return False
    if not Tools.createSymLink( '%s/mkspecs/macx-g++' % ( targetPath ), '%s/mkspecs/default' % ( targetPath ) ):
        return False
    
    return True

if __name__ == '__main__':
    # mount point is the result of using dmg2img then mount the img file in /mnt
    mountPath = '/mnt'
    exportPath = '/home/pasnox/Developpement/OS/OSX-Libraries/Qt/4.8.0'
    packagesPath = '%s/packages' % ( mountPath )
    packages = Tools.getFoldersList( packagesPath, '*.pkg' )
    
    if not os.path.exists( exportPath ):
        if not Tools.createDirectory( exportPath ):
            exportPath = os.environ[ 'PWD' ]
    
    for package in packages:
        paxFilePath = '%s/Contents/Archive.pax.gz' % ( package )
        if not os.path.exists( paxFilePath ):
            print 'Error on %s' % ( paxFilePath )
            sys.exit( 1 )
        elif not unpackPaxFile( paxFilePath, exportPath ):
            print 'Error on unpacking %s' % ( paxFilePath )
            sys.exit( 2 )
    
    if not conformToStandardQtInstall( exportPath ):
        print 'Error on conforming %s' % ( exportPath )
        sys.exit( 3 )
    
    print ''
    print 'All done.'
    sys.exit( 0 )