#!/usr/bin/env python
""" Script for convert svn log to MkS changelog.
Reformats text, skips commit without comments
Usage:
	svn log 
"""
import sys
import re
import time

def main():
	separator = ('-' * 72 + '\n')
	line = sys.stdin.readline()
	while line != separator:
		line = sys.stdin.readline()
	
	while len(line):
		line = sys.stdin.readline()
		rex = re.compile ('r\d+ \| (\w+) \|[ \d\-:\+]+\(([\w,\d ]+)\) \| (\d+)')
		match = rex.match (line)
		if match:
			who, when, comment_len = match.groups()
			comment_len = int (comment_len) -1
			if comment_len:
				sys.stdin.readline() # skip empty line
				comment = ''
				for i in range (int(comment_len)):
					comment += sys.stdin.readline()
				print '------------------------------------------------------------------------\n%s    (%s)\n%s' % (who, when, comment)
			else:
				pass #skip entries without comment
				#print who, when, 'no comment'

main()
