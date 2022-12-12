#!/bin/bash
m68k-elf-gcc -nostdlib -nodefaultlibs -fno-builtin -m68000 crt0.s -o bin/crt0.o -c
m68k-elf-gcc -nostdlib -nodefaultlibs -nostdlib -fno-builtin -m68000 lib/sys.c -o bin/_sys.o -c
m68k-elf-gcc -nostdlib -nodefaultlibs -nostdlib -fno-builtin -m68000 main.c -o bin/_main.o -c -fno-delete-null-pointer-checks
m68k-elf-gcc -nostdlib -nodefaultlibs -nostdlib -fno-builtin -m68000 lib/sd.c -o bin/_sd.o -c -fno-delete-null-pointer-checks
m68k-elf-ld -T link.ld bin/_*.o -o bin/bin.out
m68k-elf-objcopy -O binary  bin/bin.out OS.bin
#hexyl OS.bin
