#!/bin/sh

rm -rf html
rm -rf monkeystudiodocs
rm -f wikify.py

svn co http://monkeystudio.googlecode.com/svn/wiki/ monkeystudiodocs

wget http://cspace.googlecode.com/svn/wiki/wikify.py
chmod a+x wikify.py
mkdir html

./wikify.py -s monkeystudiodocs/ -d html/ -p TestPage --stale --levels 5 --mainpage=TestPage