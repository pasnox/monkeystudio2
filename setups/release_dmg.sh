#!/bin/sh
#-------------------------------------------------------------------------------
echo "Creating temporary dmg..."
hdiutil create -megabytes 200 tmp.dmg -layout NONE -fs HFS+ -volname "Monkey Studio" -ov
tmp=`hdid tmp.dmg`
disk=`echo $tmp | cut -f 1 -d\ `

echo "Copying Monkey Studio..."
cp -R "../bin/Monkey Studio.app" "/Volumes/Monkey Studio/"

echo "Copying standard files..."
cp ../dev-readme "/Volumes/Monkey Studio/"
cp ../GPL-2 "/Volumes/Monkey Studio/"
cp ../GPL-3 "/Volumes/Monkey Studio/"
cp ../LGPL-3 "/Volumes/Monkey Studio/"
cp ../readme.txt "/Volumes/Monkey Studio/"

echo "Creating release dmg..."
hdiutil eject "$disk"
hdiutil convert -format UDZO tmp.dmg -o "Monkey Studio.dmg"
rm tmp.dmg
