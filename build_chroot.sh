#!/bin/sh
Target=$1
libs=`find $Target -type f -perm 755 -exec "ldd" {} \; \
	|cut -d \> -f 2|cut -d \( -f 1| \
	uniq|sed -e 's/^[ \t]*//g' -e '/^$/d' `

echo "All libs:"
for lib in $libs
do
	if [ ! -f $Target$lib -a -f $lib ] ; then
		echo $lib
		dir=`dirname $Target$lib`
		if [ ! -d $dir ] ; then
			mkdir -pv $dir
		fi
		cp -av $lib $Target$lib
		if [ -h $lib ] ; then
			source=`dirname $lib`/`readlink $lib`
			cp -av $source $Target$source
		fi
	fi
done


echo "Done!"
