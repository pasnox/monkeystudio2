#!/bin/sh
# parameters
# 1 = monkey version, default 1.8.x
# 2 = monkey svn revision, default xxxx
# 3 = architecture, default i386
# 4 = gpg id, default Filipe AZEVEDO

# package name
package_name=monkeystudio

# monkey version
version=$1
if [ "$version" = '' ]
then
	version=1.8.x
fi

# monkey revision
revision=$2
if [ "$revision" = '' ]
then
	revision=xxxx
fi

# architecture
arch=$3
if [ "$arch" = '' ]
then
	arch=i386
fi

# gpg id
id=$4
if [ "$id" = '' ]
then
	id=ED11858D
fi

# generate complete name of package
package="$package_name"_"$version"+"$revision"

# creating source tarball
echo "Creating source tarball..."
#tar czf ../$package.orig.tar.gz .

# creating source package
echo "Creating source package..."
debuild -S -sa --lintian-opts -i -k0$id

# building package
echo "Building package..."
sudo pbuilder build --logfile ../log.txt ../$package-0ubuntu1.dsc

# copying deb file
echo "Copying deb file..."
cp /var/cache/pbuilder/result/$package-0ubuntu1_"$arch".deb ..

# done
echo "Done !"
