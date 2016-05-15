#
#  msr-agent - Utility for help MSR access
#  file name: Makefile
#
# Copyright (c) 2016 Nicholas Feng <nicholasfeng.tw@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#

CC       = gcc -Wall
CFLAGS   = -g -O2 -fomit-frame-pointer -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64
LDFLAGS  =

BIN	= msr-agent

sbindir = /usr/sbin

all: $(BIN)

clean:
	rm -f *.o $(BIN)

distclean: clean
	rm -f *~ \#*

install: all
	install -m 755 $(BIN) $(sbindir)

.o:
	$(CC) $(LDFLAGS) -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -o $@ $<

.c:
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<
