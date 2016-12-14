#!/bin/sh

name = avatar

make clean && make all

#libtool --mode=compile g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c ../src/System.cpp -o src/System.o

find . -name "*.o" -and -not -name "main.o" -print0 | xargs -0 ar crv libinception.a

#sudo -S rm /usr/lib/gcc/x86_64-linux-gnu/5/libinception.a
#sudo -S cp libinception.a /usr/lib/gcc/x86_64-linux-gnu/5/

#sudo -S find / -name libinception.a
