#!/usr/bin/python3.5
import code
from ctypes import cdll
lib = cdll.LoadLibrary('../Debug/libsuperspeed-jtag.so')

class Interactive(object):
    def help(self):
        print("Avatar SuperspeedJtag Interactive Mode V0.1")
        print("Author  : Corteggiani Nassim")
        print("Contact : nassim.corteggiani@maximintegrated.com")
        print("___________________________________________________")

    def __init__(self):
        self.help()
        self.obj = lib.jtag_init()
        code.InteractiveConsole(locals=locals()).interact()

    def write(self, address, value):
        lib.jtag_write(self.obj, address, value, 32);

    def read(self, address, value):
        return lib.jtag_write(self.obj, address, value, 32);

if __name__ == '__main__' :
    Interactive()
