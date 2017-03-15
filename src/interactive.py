# @Author: Corteggiani Nassim <Corteggiani>
# @Date:   15-Mar-2017
# @Email:  nassim.corteggiani@maximintegrated.com
# @Filename: interactive.py
# @Last modified by:   Corteggiani
# @Last modified time: 15-Mar-2017
# @License: GPLv3



#!/usr/bin/python3.5
import code
from collections import OrderedDict
from ctypes import cdll
lib = cdll.LoadLibrary('../Debug/libinception.so')

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
        print("\t display_all_regs() :display all CPU registers")
        print("\t read_reg(id) : read a specific register")
        print("\t write_reg(id, value) : write a value into the specified register")
        print("\t show_dhcsr() : read the dhcsr register and cast content")

    def __init__(self):
        self.help()
        self.obj = lib.jtag_init()
        code.InteractiveConsole(locals=locals()).interact()

    def write(self, address, value):
        lib.jtag_write(self.obj, address, value, 32)

    def read(self, address):
        return lib.jtag_read_u32(self.obj, address)

    def step(self):
        self.write(0xE000EDF0, (0xA05F << 16) | (1<<2) | (1 << 0))

    def halt(self):
        self.write(0xE000EDF0, (0xA05F << 16) | (1<<1) | (1 << 0))

    def enable_debug(self):
        self.write(0xE000EDF0, (0xA05F << 16) | (1 << 0))

    def display_all_regs(self):
        regs = { "r0" : 0,
            "r1" : 1,
            "r2" : 2,
            "r3" : 3,
            "r4" : 4,
            "r5" : 5,
            "r6" : 7,
            "r8" : 8,
            "r9" : 9,
            "r10" : 10,
            "r11" : 11,
            "SP" : 12,
            "r13" : 13,
            "r14" : 14,
            "pc" : 15
        }

        o_regs = OrderedDict(regs)

        for reg, id in o_regs.items():
            print(reg+ " : " + self.read_reg(id))

    def interrupt(self, id):

            self.write(0xE000E200, id)

    def share(self, message):

        self.write(0xE000EDF8, message)

    def read_reg(self, reg):

            # DCB_DCRDR = hex(self.read(0xE000EDF8))

            self.write(0xE000EDF4, reg)

            reg = hex(self.read(0xE000EDF8))

            # self.write(0xE000EDF8, DCB_DCRDR)

            return reg

    def write_reg(self, reg, value):

            # DCB_DCRDR = hex(self.read(0xE000EDF8))

            self.write(0xE000EDF8, value)

            self.write(0xE000EDF4, reg |  (1 << 16))

            # self.write(0xE000EDF8, DCB_DCRDR)

            return reg

    def show_dhcsr(self):

            value = self.read(0xE000EDF0)

            if value < 0 :
                value = value + 2**32

            print(hex(value))

            DHCSR(value.to_bytes(4, byteorder='big')).show()

import scapy
from scapy.all import *

class DHCSR(Packet):
    name = "DHCSR Register\r\n"
    fields_desc=[

        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),

        XBitField("S_RESET_ST_________", 0, size=1),
        XBitField("S_RETIRE_ST________", 0, size=1),

        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),

        XBitField("S_LOCKUP___________", 0, size=1),
        XBitField("S_SLEEP____________", 0, size=1),
        XBitField("S_HALT_____________", 0, size=1),
        XBitField("S_REGRDY___________", 0, size=1),

        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),

        XBitField("C_SNAPSTALL__________", 0, size=1),
        XBitField("RESERVED", 0, size=1),

        XBitField("C_MASKINTS___________", 0, size=1),
        XBitField("C_STEP_______________", 0, size=1),
        XBitField("C_HALT_______________", 0, size=1),
        XBitField("C_DEBUGEN____________", 0, size=1),
        # XShortField("DBGKEY", 0xA05F),
    ]


if __name__ == '__main__' :
    Interactive()