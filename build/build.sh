#!/bin/sh

name = avatar

cd ../Debug
make clean && make all

cd ../build/o
libtool --mode=compile g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c ../../src/benchmark/Benchmark.cpp
libtool --mode=compile g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c ../../src/jtag/Command.cpp
libtool --mode=compile g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c ../../src/jtag/CommandsFactory.cpp
libtool --mode=compile g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c ../../src/jtag/Jtag.cpp
libtool --mode=compile g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c ../../src/jtag/TDO.cpp
libtool --mode=compile g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c ../../src/jtag/ap/AccessPort.cpp
libtool --mode=compile g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c ../../src/jtag/ap/ahb_ap/AHB_AP.cpp
libtool --mode=compile g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c ../../src/jtag/ap/jtag_ap/JtagAP.cpp
libtool --mode=compile g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c ../../src/interface/Interface.cpp
libtool --mode=compile g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c ../../src/interface/Producer.cpp
libtool --mode=compile g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c ../../src/device/Device.cpp
libtool --mode=compile g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c ../../src/Decoder.cpp
libtool --mode=compile g++ -std=c++1y -fpic -I-lpthread -I-lusb-1.0 -O0 -g3 -Wall -c ../../src/superspeedjtag.cpp

libtool --mode=link g++ -o libavatar.la Command.lo CommandsFactory.lo Jtag.lo TDO.lo AccessPort.lo AHB_AP.lo JtagAP.lo Interface.lo Producer.lo Device.lo Decoder.lo superspeedjtag.lo Benchmark.lo -rpath /usr/local/lib -version-info 6:0:5

ar crv libavatar.a *.o

g++ -shared -Wl,-soname=avatar -o "libavatar.so" -lpthread -lusb-1.0

sudo -S rm /usr/lib/gcc/x86_64-linux-gnu/5/libavatar.a
sudo -S cp libavatar.a /usr/lib/gcc/x86_64-linux-gnu/5/

#sudo -S ldconfig

#ldconfig -p | grep avatar

#cd ../../test/ && g++ -std=c++1y test.cpp -lavatar -lpthread -lusb-1.0 -o test

sudo find / -name libavatar.a
