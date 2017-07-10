#!/bin/sh

name=inception

GCC_VERSION="$(gcc -dumpversion)"

echo "GCC version ${GCC_VERSION} detected"

make clean
if [ $? != 0 ]; then
	echo "FAILED! make clean"
        exit;
fi

rm -rf lib$name.a $name


make all
if [ $? != 0 ]; then
	echo "FAILED! make all"
        exit;
fi

#libtool --mode=compile g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c ../src/System.cpp -o src/System.o

find . -name "*.o" -and -not -name "main.o" -print0 | xargs -0 ar crv lib$name.a
if [ $? != 0 ]; then
	echo "FAILED! find . -name "*.o" -and -not -name "main.o" -print0 | xargs -0 ar crv lib$name.a"
        exit;
fi

sudo -S rm /usr/lib/gcc/x86_64-linux-gnu/${GCC_VERSION}/libinception.a
sudo -S cp libinception.a /usr/lib/gcc/x86_64-linux-gnu/${GCC_VERSION}/libinception.a

#sudo -S find / -name libinception.a
