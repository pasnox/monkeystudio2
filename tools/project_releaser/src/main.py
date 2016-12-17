#!/usr/bin/python
import sys
#import argparse

import MkS

if __name__ == '__main__':
    _version = '0.5.0'
    
    project = MkS.MkSProject()
    
    '''
    parser = argparse.ArgumentParser( description = 'A project source and package releaser.' )
    
    parser.add_argument( '-v', action='version', version='%(prog)s v{0}'.format( _version ) )
    
    parser.add_argument(
        '-name', dest = 'name',
        default = project.name,
        help = 'set the project name'
    )
    
    parser.add_argument(
        '-version', dest = 'version',
        default = project.version,
        help = 'set the project version'
    )
    
    parser.add_argument(
        '-company', dest = 'company',
        default = project.company,
        help = 'set the project company'
    )
    
    parser.add_argument(
        '-copyrights', dest = 'copyrights',
        default = project.copyrights,
        help = 'set the project copyrights'
    )
    
    parser.add_argument(
        '-description', dest = 'description',
        default = project.description,
        help = 'set the project description'
    )
    
    parser.add_argument(
        '-homepage', dest = 'homepage',
        default = project.urlHomepage,
        help = 'set the project homepage'
    )
    
    parser.add_argument(
        '-forums', dest = 'forums',
        default = project.urlForums,
        help = 'set the project forums'
    )
    
    parser.add_argument(
        '-issues', dest = 'issues',
        default = project.urlIssuesTracker,
        help = 'set the project issues'
    )
    '''
    
    '''parser.add_argument('integers', metavar='N', type=int, nargs='+',
                       help='an integer for the accumulator')'''
    
    '''parser.add_argument('--sum', dest='accumulate', action='store_const',
                       const=sum, default=max,
                       help='sum the integers (default: find the max)')'''

    #args = parser.parse_args()
    
    #project.version = args.version
    
    project.run()
    sys.exit( 0 )
