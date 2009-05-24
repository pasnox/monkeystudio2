#!/usr/bin/env python
"""Script for convert svn log to MkS changelog.
Reformats text, skips commits without comments
Usage:
	svn log > svnlog-to-changelog.py
"""

import sys
import re

def main():
	separator = ('-' * 72 + '\n')
	line = sys.stdin.readline()
	while line != separator:
		line = sys.stdin.readline()
	
	while len(line):
		line = sys.stdin.readline()
		rex = re.compile ('(r\d+) \| (\w+) \|[ \d\-:\+]+\(([\w,\d ]+)\) \| (\d+)')
		match = rex.match (line)
		if match:
			revision, who, when, comment_len = match.groups()
			comment_len = int (comment_len) -1
			if comment_len:
				sys.stdin.readline() # skip empty line
				comment = ''
				for i in range (int(comment_len)):
					comment += sys.stdin.readline()
				print '-------- %s    %s(%s) --------\n%s' % (who, revision, when, comment)
			else:
				pass #skip entries without comment
				#print who, when, 'no comment'

if __name__ == '__main__':
	if len(sys.argv) > 1:
		print __doc__
	else:
		main()
