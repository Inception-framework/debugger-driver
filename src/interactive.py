# @Author: Corteggiani Nassim <Corteggiani>
# @Date:   15-Mar-2017
# @Email:  nassim.corteggiani@maximintegrated.com
# @Filename: interactive.py
# @Last modified by:   Corteggiani
# @Last modified time: 15-Mar-2017
# @License: GPLv3



#!/usr/bin/python3.5
import os
import code
from collections import OrderedDict
from ctypes import cdll
import struct

class Interactive(object):

# DBGKEY		= (0xA05F << 16)
# C_DEBUGEN	= (1 << 0)
# C_HALT		= (1 << 1)
# C_STEP		= (1 << 2)
# C_MASKINTS	= (1 << 3)
# S_REGRDY	= (1 << 16)
# S_HALT		= (1 << 17)
# S_SLEEP		= (1 << 18)
# S_LOCKUP	= (1 << 19)
# S_RETIRE_ST	= (1 << 24)
# S_RESET_ST	= (1 << 25)

    def help(self):
        print("Avatar SuperspeedJtag Interactive Mode V0.1")
        print("Author  : Corteggiani Nassim")
        print("Contact : nassim.corteggiani@maximintegrated.com")
        print("Please install rlwrap for 'up arrow history' : rlwrap python3.5 interactive.py")
        print("___________________________________________________")
        print("Supported functions : ")
        print("\t write(address, value) : AHB-AP memory write")
        print("\t read(address) : AHB-AP memory read")
        print("\t step() : single step")
        print("\t halt() : halt the cpu")
        print("\t resume() : exit debug state, start execution, exceptions activate according to exeption config rules")
        print("\t display_all_regs() :display all CPU registers")
        print("\t read_reg(id) : read a specific register")
        print("\t write_reg(id, value) : write a value into the specified register")
        print("\t load_binary_in_sram() : load binary in sram");

    def __init__(self, lib_path='../Debug/libinception.so', interactive=True):
        self.lib = cdll.LoadLibrary(lib_path)
        self.obj = self.lib.jtag_init()
        self.regs = OrderedDict()
        self.regs.update({ "R0" :  0 })
        self.regs.update({ "R1" :  1 })
        self.regs.update({ "R2" :  2 })
        self.regs.update({ "R3" :  3 })
        self.regs.update({ "R4" :  4 })
        self.regs.update({ "R5" :  5 })
        self.regs.update({ "R6" :  6 })
        self.regs.update({ "R7" :  7 })
        self.regs.update({ "R8" :  8 })
        self.regs.update({ "R9" :  9 })
        self.regs.update({ "R10" : 10 })
        self.regs.update({ "R11" : 11 })
        self.regs.update({ "R12" : 12 })
        self.regs.update({ "SP" : 13 })
        self.regs.update({ "LR" : 14 })
        self.regs.update({ "PC" : 15 })
        self.regs.update({ "CPSR" : 16})

        if(interactive==True):
            self.help()
            code.InteractiveConsole(locals=locals()).interact()

    def load_binary_in_sram(self,path,address):
        f=open(path,"rb")
        for i in range(0,int(os.path.getsize(path)/4)):
            self.lib.jtag_write(self.obj, address+i*4, int(struct.unpack("i",f.read(4))[0]), 32)
        f.close()

    def dump(self,path,address,value):
        f=open(path,"wb")
        for i in range(0,value):
            val = self.lib.jtag_read_u32(self.obj, address+i*4)
            f.write(struct.pack('1i',val))
        f.close()

    def dir_36(self,val):
        self.lib.jtag_write(self.obj,0x400F4000+0x2000,val<<6,32)
    def set_36(self):
        self.lib.jtag_write(self.obj,0x400F4000+0x2200,1<<6,32)
    def clear_36(self):
        self.lib.jtag_write(self.obj,0x400F4000+0x2280,1<<6,32)

    def read_36(self):
        print(self.lib.jtag_read(self.obj,0x400F4000+0x2100) & (1<<6))

    def write(self, address, value):
        self.lib.jtag_write(self.obj, address, value, 32)

    def read(self, address):
        value = self.lib.jtag_read_u32(self.obj, address)
        if value < 0 :
            value = value + 2**32
        print(hex(value))

    def read_csw(self):
        value = self.lib.jtag_control(self.obj)
        print(hex(value))


    #def load_binary_in_sram(self, path, address):
        #self.lib.load_binary_in_sdram(self.obj, path, address)
        #f=open(path,"rb")
        #b = f.read(4)
        #while(b!=""):
        #    value = int(struct.unpack("i",b)[0])
        #    self.write(address,value)
        #    address = address + 4
        #    print hex(address),hex(value)
        #    b = f.read(4)
        #f.close()

    def step(self,val):
        for i in range(0,val):
            self.write(0xE000EDF0, (0xA05F << 16) | (1<<2) | (1 << 0))
            self.read_reg(15)

    def halt(self):
        self.write(0xE000EDF0, (0xA05F << 16) | (1<<1) | (1 << 0))

    def resume(self):
        self.write(0xE000EDF0, (0xA05F << 16) | (0<<1) | (1 << 0))

    def enable_debug(self):
        self.write(0xE000EDF0, (0xA05F << 16) | (1 << 0))

    def display_all_regs(self):
        for reg, id in self.regs.items():
            print(reg+ " : ",end="")
            self.read_reg(id)

    def dump_all_regs(self):
        dumped_regs = OrderedDict()
        for reg, id in self.regs.items():
            self.lib.jtag_write(self.obj,0xE000EDF4,id,32)
            val = self.lib.jtag_read_u32(self.obj,0xE000EDF8)
            dumped_regs.update({reg : val})
        return dumped_regs


    def interrupt(self, id):

            self.write(0xE000E200, id)

    def share(self, message):

        self.write(0xE000EDF8, message)

    def read_reg(self, reg):

            # DCB_DCRDR = hex(self.read(0xE000EDF8))

            self.write(0xE000EDF4, reg)

            #reg = hex(self.read(0xE000EDF8))
            self.read(0xE000EDF8)

            # self.write(0xE000EDF8, DCB_DCRDR)

            #return reg

    def write_reg(self, reg, value):

            # DCB_DCRDR = hex(self.read(0xE000EDF8))

            self.write(0xE000EDF8, value)

            self.write(0xE000EDF4, reg |  (1 << 16))

            # self.write(0xE000EDF8, DCB_DCRDR)

            return reg
    def clear_all_regs(self):
            for reg,id in self.regs.items():
                if(reg != "SP" and reg != "PC"):
                    self.write_reg(id,0)

if __name__ == '__main__' :
    Interactive()
