#! /bin/sh
if [ "$1" == "" ]
then
	echo "Usage: $0 <library>" >&2
	exit 1
fi
for file in $(ls /usr/bin/*); do
	linked=$(ldd $file 2> /dev/null)
	if [ -n "$linked" ]
	then
		linked=$(echo $linked | grep -o $1)
	else
		continue
	fi
	if [ -n "$linked" ]
	then
		basename $file
	fi
done

