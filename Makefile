# Copyright 2013 <chaishushan{AT}gmail.com>. All rights reserved.
# Use of this source code is governed by a BSD-style
# license that can be found in the LICENSE file.

TARG=hello

CC=g++
CCFLAGS=-I. -std=c++11
LDFLAGS=

ifeq ($(OS),Windows_NT)
	EXT:=.exe
endif

$(TARG)$(EXT): hello.cc
	$(CC) $(CCFLAGS) -o $@ $<

clean:
	-rm $(TARG)$(EXT)

