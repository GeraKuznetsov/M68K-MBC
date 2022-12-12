#!/bin/bash
m68k-elf-gcc -nostdlib -nodefaultlibs -nostdlib -fno-builtin -m68000 lib/fat/diskio.c -o bin/_diskio.o -c
m68k-elf-gcc -nostdlib -nodefaultlibs -nostdlib -fno-builtin -m68000 lib/fat/ff.c -o bin/_ff.o -c
m68k-elf-gcc -nostdlib -nodefaultlibs -nostdlib -fno-builtin -m68000 lib/fat/ffsystem.c -o bin/_ffsystem.o -c
m68k-elf-gcc -nostdlib -nodefaultlibs -nostdlib -fno-builtin -m68000 lib/fat/ffunicode.c -o bin/_ffunicode.o -c
