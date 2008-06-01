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

# package version
package_version="$version"+svn"$revision"

# generate complete name of package
package="$package_name"_"$package_version"
package_new="$package_name"-"$package_version"

# generate package orig
package_orig=$package.orig.tar.gz

createSourceOrig()
{
	# parameters
	# 1 = revision
	# 2 = package
	# 3 = package orig
	
	# export repository
	svn export -r $1 -q svn://monkeystudio.org/monkeyrepos/v2/branches/dev ./$2
	
	# create orig tarball
	tar czf ../$3 ./$2
	
	# delete svn export
	rm -R ./$2
}

updateChangeLog()
{
	# parameters
	# 1 = package orig
	# 2 = package version
	
	# get fullname and email of packager associate with gpg id
	export DEBFULLNAME=''
	export DEBEMAIL="$(gpg --list-key $id | grep -m 1 ^uid | sed 's/^uid[ ]*//')"
	
	# update changelog
	uupdate -u ../$1 -v $2
}

# creating source tarball
echo "Creating source tarball from svn..."
createSourceOrig $revision $package $package_orig

# updating the change log
updateChangeLog $package_orig $package_version

# delete temporary folder
rm -R ../"$package_new".orig

# go into new package
echo "Going into new package"
cd ../$package_new

# creating source package
echo "Creating source package..."
debuild -S -sa --lintian-opts -i -k0$id

# building package
echo "Building package..."
sudo pbuilder build --logfile ../log.txt ../$package-0ubuntu1.dsc

# copying deb file
echo "Copying deb file..."
cp /var/cache/pbuilder/result/"$package"_"$arch".deb ..

# done
echo "Done !"
