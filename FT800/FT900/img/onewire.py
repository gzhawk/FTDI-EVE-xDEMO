#!/usr/bin/env python

"""
On Mac, need to make sure that the VSP driver not to attach, or you'll see FT_DEVICE_NOT_OPENED.
If you see this message, try removing /System/Library/Extensions/*FTDI*

On Linux, do: "sudo rmmod ftdi_sio; sudo rmmod usbserial" to remove VCP.

"""

import os
import sys
import ctypes
import time
import array
import struct
# from debugtools import *

########################################################################
# D2XX definitions

def check(f):
    if f != 0:
        names = [
            "FT_OK",
            "FT_INVALID_HANDLE",
            "FT_DEVICE_NOT_FOUND",
            "FT_DEVICE_NOT_OPENED",
            "FT_IO_ERROR",
            "FT_INSUFFICIENT_RESOURCES",
            "FT_INVALID_PARAMETER",
            "FT_INVALID_BAUD_RATE",
            "FT_DEVICE_NOT_OPENED_FOR_ERASE",
            "FT_DEVICE_NOT_OPENED_FOR_WRITE",
            "FT_FAILED_TO_WRITE_DEVICE",
            "FT_EEPROM_READ_FAILED",
            "FT_EEPROM_WRITE_FAILED",
            "FT_EEPROM_ERASE_FAILED",
            "FT_EEPROM_NOT_PRESENT",
            "FT_EEPROM_NOT_PROGRAMMED",
            "FT_INVALID_ARGS",
            "FT_NOT_SUPPORTED",
            "FT_OTHER_ERROR"]
        raise IOError("Error in MPSSE function (status %d: %s)" % (f, names[f]))

FT_OPEN_BY_DESCRIPTION =   2

FT_BITS_8		= 8
FT_STOP_BITS_2		= 2
FT_PARITY_NONE		= 0

# Debugger command write
# access data space - 16-bit memory

########################################################################
# Debugger interface definitions

COMMAND_WRITE_DATA_BYTE	= 0x80
COMMAND_READ_DATA_BYTE	= 0xC0

COMMAND_WRITE_DATA_8BYTE	= 0xB0
COMMAND_WRITE_DATA_16BYTE	= 0xB4
COMMAND_WRITE_DATA_32BYTE	= 0xB8
COMMAND_WRITE_DATA_64BYTE	= 0xBC

DBG_RSADDR0 = 0x00
DBG_RSADDR1 = 0x01
DBG_RSADDR2 = 0x02
DBG_FSADDR0 = 0x03
DBG_FSADDR1 = 0x04
DBG_FSADDR2 = 0x05
DBG_BLENGTH0 = 0x06
DBG_BLENGTH1 = 0x07
DBG_BLENGTH2 = 0x08
DBG_COMMAND = 0x09
DBG_SEMAPHORE = 0x0B
DBG_CONFIG = 0x0C
DBG_STATUS = 0x0D
DBG_DRWDATA = 0x80

def pad_ff(s, l):
    return s + (chr(0xff) * (l - len(s)))
            
class Onewire(object):
    def __init__(self):
        if sys.platform.startswith('linux'):
            self.d2xx = ctypes.cdll.LoadLibrary("libftd2xx.so")
        elif sys.platform.startswith('darwin'):
            self.d2xx = ctypes.cdll.LoadLibrary("libftd2xx.1.1.0.dylib")
        else:
            self.d2xx = ctypes.windll.LoadLibrary("ftd2xx")
        print 'D2XX library loaded OK'
        sys.stdout.flush()

        self.ftHandle = ctypes.c_void_p()
        stt = self.d2xx.FT_OpenEx("UMFTPD2A A",  FT_OPEN_BY_DESCRIPTION, ctypes.byref(self.ftHandle))
        if stt != 0:
            stt = self.d2xx.FT_OpenEx("VII Debugger Module",  FT_OPEN_BY_DESCRIPTION, ctypes.byref(self.ftHandle))
            if stt != 0:
                raise IOError("Error: Programmer not found %d", stt)

        check(self.d2xx.FT_SetBaudRate(self.ftHandle, 1000000))
        check(self.d2xx.FT_SetDataCharacteristics(self.ftHandle, FT_BITS_8, FT_STOP_BITS_2, FT_PARITY_NONE))
        check(self.d2xx.FT_SetTimeouts(self.ftHandle, 1000, 0))

        self.s = ctypes.create_string_buffer(65536)

    def delay(self):
        time.sleep(0.001)

    def write(self, s, isread = False):
        # print hexdump(s)
        # if not isread:
        #     print "W " + " ".join(["%02x" % ord(c) for c in s])
        dwNumBytesSent = ctypes.c_uint()
        check(self.d2xx.FT_Write(self.ftHandle, s, len(s), ctypes.byref(dwNumBytesSent)))
        assert dwNumBytesSent.value == len(s)

    def onewire_write(self, command, addr0, addr1, data):
        self.write(chr(command) + chr(addr0) + chr(addr1) + chr(data))
        self.delay()

    def onewire_block_write(self, command, addr0, addr1, data):
        self.write(chr(command) + chr(addr0) + chr(addr1) + data)
        self.delay()

    def onewire_read(self, command, addr0, addr1):
        self.write(chr(command) + chr(addr0) + chr(addr1), True)

        dwNumBytesRead = ctypes.c_uint()
        check(self.d2xx.FT_Read(self.ftHandle, self.s, 1, ctypes.byref(dwNumBytesRead)))
        return ord(self.s[0])

    def setTube(self, d):
        """ Transmit 8 bytes by writing to registers 0x18-1f """
        self.write(struct.pack("<BBBII", 0xb0, 0x18, 0x00, d, 0xffffffff))

    def getTube(self):
        """ Recv 8 bytes from registers 0x18-0x1f """
        self.write(chr(0xf0) + chr(0x18) + chr(0x00), True)

        dwNumBytesRead = ctypes.c_uint()
        check(self.d2xx.FT_Read(self.ftHandle, self.s, 8, ctypes.byref(dwNumBytesRead)))
        (d, hs) = struct.unpack("II", self.s[:8])
        return (hs, d)

    def status_reg(self):
        # read out status register
	self.onewire_write(COMMAND_WRITE_DATA_BYTE, DBG_COMMAND, 0x00, 0x05)
	# 0x80 register store data of spi flash read back
	return self.onewire_read(COMMAND_READ_DATA_BYTE, DBG_DRWDATA, 0x00)

    def status(self):
	return self.onewire_read(COMMAND_READ_DATA_BYTE, DBG_DRWDATA, 0x00)

    def set24(self, r, v):
        # print 'reg', r, '<=', v
        self.onewire_write(COMMAND_WRITE_DATA_BYTE, r + 0, 0x00, v & 0xff)
        self.onewire_write(COMMAND_WRITE_DATA_BYTE, r + 1, 0x00, (v >> 8) & 0xff)
        self.onewire_write(COMMAND_WRITE_DATA_BYTE, r + 2, 0x00, (v >> 16) & 0xff)

    def command(self, c):
        assert 0 <= c < 256
	self.onewire_write(COMMAND_WRITE_DATA_BYTE, DBG_COMMAND, 0x00, c)

    def lock(self):
        sem = self.onewire_read(COMMAND_READ_DATA_BYTE, DBG_SEMAPHORE, 0x00)
        if sem != 0:
            print "Could not lock, but continuing anyway"
            sys.stdout.flush()
            # sys.exit(1)

    def unlock(self):
        self.onewire_write(COMMAND_WRITE_DATA_BYTE, DBG_SEMAPHORE, 0x00, 0x01)

    def reset(self):
        self.lock()
        self.set24(DBG_BLENGTH0, 0xff)
        self.set24(DBG_FSADDR0, 0x0000)
        self.set24(DBG_RSADDR0, 0x0000)
        self.command(0xe6)
        for i in range(4):
            self.onewire_block_write(COMMAND_WRITE_DATA_64BYTE, DBG_DRWDATA, 0x00, " " * 64)

    def loadflash(self, fn):
        pgm = open(fn, "rb").read()
        MAXPGM = (256 - 0) * 1024
        if len(pgm) == 0 or len(pgm) > MAXPGM:
            print "ERROR, program %s is not a valid binary executable" % sys.argv[1]
            sys.stdout.flush()
            sys.exit(1)

        self.lock()

        # set SPI flash clock by CONFIG register
        self.onewire_write(COMMAND_WRITE_DATA_BYTE, DBG_CONFIG, 0x00, 0x03)

        # set SPI flash write enable
        self.command(0x06)

        if len(pgm) < 0x40000:
            print 'Binary less than 256K, Adding bootloader'
            sys.stdout.flush()

            # Find the bootloader, a 4K binary
            BLBASE = 1024 * (256 - 4)
            bootloaderbin = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'forthboot.bin')
            bootfile = open(bootloaderbin, "rb")
            bootfile.seek(BLBASE)
            bootloader = bootfile.read()
            assert len(bootloader) == 4096, "Bootloader is not valid"

            pgm = pad_ff(pgm, 0x40000 - 4096) + bootloader
            print hex(len(pgm))

        if 1:
            bb = array.array('B', pgm)
            hexfile = open("pm.hex", "w")
            for ab in bb:
                print >>hexfile, "%02x" % ab

        print 'erasing...'
        sys.stdout.flush()

	# set SPI flash write enable
        self.command(0x06)
	# erase whole chip
        self.command(0x60)
        time.sleep(1)

        status = self.status()
        while status & 1:
            print 'still erasing'
            time.sleep(.1)
            status = self.status()
        print 'Now status is', status
        sys.stdout.flush()

        blksz = 256

        self.set24(DBG_BLENGTH0, blksz - 1)

        def writeblk(addr, s, islast = 0):
            if set(s) == set(chr(0xff)):
                return
            print '%05x:' % addr
            sys.stdout.flush()            
            self.command(0x06)
            status = self.status_reg()
            while (status & 2) == 0:
                self.command(0x06)
                status = self.status_reg()

            self.set24(DBG_FSADDR0, addr)

            if not islast:
		self.command(0xE8)
            else:
                # last writing shall do CPU reset after transfer done. 
		self.command(0xEA)

            for j in range(0, blksz, 64):
                sub64 = s[j:j+64]
                self.onewire_block_write(COMMAND_WRITE_DATA_64BYTE, DBG_DRWDATA, 0x00, sub64)

        blks = range(0, len(pgm), blksz)
        for i in blks:
            sub = pgm[i:i+blksz].ljust(blksz, chr(0xff))
            assert len(sub) == blksz
            writeblk(i, sub, i == blks[-1])

    def loadpm(self, fn):
        t0 = time.time()
        pgm = open(fn, "rb").read()
        pgm = pad_ff(pgm, (len(pgm) + 63) & ~63)

        self.echo1()

        self.command(0x06)
        self.set24(DBG_RSADDR0, 0)
        self.set24(DBG_BLENGTH0, len(pgm) - 1)
        self.command(0xe1)
        for j in range(0, len(pgm), 64):
            sub64 = pgm[j:j+64]
            self.onewire_block_write(COMMAND_WRITE_DATA_64BYTE, DBG_DRWDATA, 0x00, sub64)
        took = time.time() - t0
        print '%d bytes took %.1fs, %.1f bytes/s' % (len(pgm), took, len(pgm) / took)
        self.unlock()
        sys.stdout.flush()

    def echo1(self):
        self.setupchan()
        return

        self.setTube(0xb0000000)
        t = time.time()
        while True:
            (hs,d1) = self.getTube()
            assert hs in (0xffffffff, 0x00000000, 0xdddddddd), "Unexpected HS value %08x" % hs
            if hs == 0xdddddddd or hs == 0xeeeeeeee:
                break
            if (time.time() - t) > 6:
                assert 0, "timeout, HS is %08x" % hs
        assert d1 == 499, "Unexpected response %08x" % d1
        print d1

    def setupchan(self):
        self.lock()

        # Enable interrupt
        self.onewire_write(COMMAND_WRITE_DATA_BYTE, 0x10, 0x00, 1)

        # Verify interrupt enable
        assert self.onewire_read(COMMAND_READ_DATA_BYTE, 0x10, 0x00) == 1

        # Release lock
        # self.onewire_write(COMMAND_WRITE_DATA_BYTE, 0x0b, 0x00, 1)

    def echo(self):
        self.reset()

        time.sleep(.1)

        self.setupchan()

        i = 0
        while True:
            i += 1
            # if (i % 100) == 0:
            print i, 'echos'
            self.setTube(0xb0000000)
            t = time.time()
            while True:
                (hs,d1) = self.getTube()
                assert hs in (0xffffffff, 0x00000000, 0xdddddddd), "Unexpected HS value %08x" % hs
                if hs == 0xdddddddd or hs == 0xeeeeeeee:
                    break
                if (time.time() - t) > 6:
                    assert 0, "timeout, HS is %08x" % hs
            assert d1 == 499, "Unexpected response %08x" % d1

    def unused():
        self.onewire_write(COMMAND_WRITE_DATA_BYTE, DBG_BLENGTH0, 0x00, 0xff)
        self.onewire_write(COMMAND_WRITE_DATA_BYTE, DBG_BLENGTH1, 0x00, 0xff)
        self.onewire_write(COMMAND_WRITE_DATA_BYTE, DBG_BLENGTH2, 0x00, 0x00)

        # set write address
        self.onewire_write(COMMAND_WRITE_DATA_BYTE, DBG_RSADDR0, 0x00, 0x00)
        self.onewire_write(COMMAND_WRITE_DATA_BYTE, DBG_RSADDR1, 0x00, 0x00)
        self.onewire_write(COMMAND_WRITE_DATA_BYTE, DBG_RSADDR2, 0x00, 0x00)

        # set command
        self.onewire_write(COMMAND_WRITE_DATA_BYTE, DBG_COMMAND, 0x00, 0xE1); # command -- 0xE1 DBG to PM 2

        for i in range(0, len(s), 64):
            sub = s[i:i+64]
            assert len(sub) == 64
            self.onewire_block_write(COMMAND_WRITE_DATA_64BYTE, DBG_DRWDATA, 0x00, sub); # transfer 64 Bytes data

    def manu_id(self):
        # read out semaphore to check which controler occurs this bit.
        sem = self.onewire_read(COMMAND_READ_DATA_BYTE, DBG_SEMAPHORE, 0x00)

        if sem != 0:
            print "Press reset on board and retry"
            sys.stdout.flush()
            # sys.exit(1)

        # set SPI flash clock by CONFIG register
        self.onewire_write(COMMAND_WRITE_DATA_BYTE, DBG_CONFIG, 0x00, 0x03)

        # set SPI flash write enable
        self.onewire_write(COMMAND_WRITE_DATA_BYTE, DBG_COMMAND, 0x00, 0x06)

        # read out status register
        self.onewire_write(COMMAND_WRITE_DATA_BYTE, DBG_COMMAND, 0x00, 0x05)
        # 0x80 register store data of spi flash read back
        data = self.onewire_read(COMMAND_READ_DATA_BYTE, DBG_DRWDATA, 0x00)
        print 'data', data

        # read out Flash manufacture ID
        self.onewire_write(COMMAND_WRITE_DATA_BYTE, DBG_COMMAND, 0x00, 0x9F)

        # 0x80 register store data of spi flash read back
        byte2 = self.onewire_read(COMMAND_READ_DATA_BYTE, DBG_DRWDATA, 0x00)
        time.sleep(0.010)
        byte1 = self.onewire_read(COMMAND_READ_DATA_BYTE, DBG_DRWDATA, 0x00)
        time.sleep(0.010)
        byte0 = self.onewire_read(COMMAND_READ_DATA_BYTE, DBG_DRWDATA, 0x00)
        ManuID = (byte2 << 16) | (byte1 << 8) | byte0
        print byte0, byte1, byte2
        print "Read out flash memory manufacture ID is [0x%08x]" % ManuID
        sys.stdout.flush()

def usage(*_):
    print "usage: onewire <command> [<args>]"
    print
    print "commands:"
    print
    print "    reset                    reset CPU"
    print "    loadflash <exefile>      load binary <exefile> into device"
    sys.exit(1)

if __name__ == '__main__':
    args = sys.argv[1:]
    if args == []:
        usage()

    c = Onewire()
    print args
    f = { 'loadflash'   : c.loadflash,
          'loadpm'      : c.loadpm,
          'echo'        : c.echo,
          'reset'       : c.reset
        }.get(args[0], usage)
    f(*args[1:])

