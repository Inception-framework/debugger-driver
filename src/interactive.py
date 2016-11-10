#!/usr/bin/python3.5
import code
from collections import OrderedDict
from ctypes import cdll
lib = cdll.LoadLibrary('../Debug/libsuperspeed-jtag.so')

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

    def __init__(self):
        self.help()
        self.obj = lib.jtag_init()
        code.InteractiveConsole(locals=locals()).interact()

    def write(self, address, value):
        lib.jtag_write(self.obj, address, value, 32)

    def read(self, address):
        return lib.jtag_read_2(self.obj, address)

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

            DCB_DCRDR = hex(self.read(0xE000EDF8))

            self.write(0xE000EDF4, reg)

            reg = hex(self.read(0xE000EDF8))

            self.write(0xE000EDF8, DCB_DCRDR)

            return reg

    def write_reg(self, reg, value):

            DCB_DCRDR = hex(self.read(0xE000EDF8))

            self.write(0xE000EDF8, value)

            self.write(0xE000EDF4, reg |  (1 << 16))

            self.write(0xE000EDF8, DCB_DCRDR)

            return reg

    def show_dhcsr(self):

            value = self.read(0xE000EDF0)

            DHCSR(value).show()

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

        XBitField("S_RESET_ST", 0, size=1),
        XBitField("S_RETIRE_ST", 0, size=1),

        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),
        XBitField("RESERVED", 0, size=1),

        XBitField("S_LOCKUP", 0, size=1),
        XBitField("S_SLEEP", 0, size=1),
        XBitField("S_HALT", 0, size=1),
        XBitField("S_REGRDY", 0, size=1),

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

        XBitField("C_SNAPSTALL", 0, size=1),
        XBitField("RESERVED", 0, size=1),

        XBitField("C_MASKINTS", 0, size=1),
        XBitField("C_STEP", 0, size=1),
        XBitField("C_HALT", 0, size=1),
        XBitField("C_DEBUGEN", 0, size=1),
        # XShortField("DBGKEY", 0xA05F),
    ]


if __name__ == '__main__' :
    Interactive()
