#!/bin/bash

base="m68k-elf"
# -D OS

gen_params="-Iinclude -m68000"
inc_lib="-Ilib/include"
libgcc_params="-msoft-float -MMD -MP -O3 -m68000"
fat_params="-nostdlib -nodefaultlibs -nostdlib -fno-builtin -m68000"
string_params="-nostdlib -nodefaultlibs -nostdlib -fno-builtin -m68000"

rm -rf bin
mkdir bin
mkdir bin/fat
mkdir bin/string
mkdir bin/libgcc
mkdir bin/driver
mkdir bin/crt

#compile_libgcc=1
#compile_string=1
#compile_fat=1

#if [[ "$compile_libgcc" -eq 1 ]]
#then
#	rm bin/libgcc/*
	$base-gcc $inc_lib $libgcc_params lib/libgcc/ashldi3.c -o bin/libgcc/ashldi3.o -c
	$base-gcc $inc_lib $libgcc_params lib/libgcc/ashrdi3.c -o bin/libgcc/ashrdi3.o -c
	$base-gcc $inc_lib $libgcc_params lib/libgcc/divsi3.S -o bin/libgcc/divsi3.o -c
	#m68k-linux-gnu-gcc $libgcc_params libgcc/lb1sf68.S -o ../bin/libgcc/lb1sf68.o -c
	$base-gcc $inc_lib $libgcc_params lib/libgcc/lshrdi3.c -o bin/libgcc/lshrdi3.o -c
	$base-gcc $inc_lib $libgcc_params lib/libgcc/modsi3.S -o bin/libgcc/modsi3.o -c
	$base-gcc $inc_lib $libgcc_params lib/libgcc/muldi3.c -o bin/libgcc/muldi3.o -c
	$base-gcc $inc_lib $libgcc_params lib/libgcc/mulsi3.S -o bin/libgcc/mulsi3.o -c
	$base-gcc $inc_lib $libgcc_params lib/libgcc/udivsi3.S -o bin/libgcc/udivsi3.o -c
	$base-gcc $inc_lib $libgcc_params lib/libgcc/umodsi3.S -o bin/libgcc/umodsi3.o -c
#fi
#
#if [[ "$compile_string" -eq 1 ]]
#then
#	rm bin/string/*
	$base-gcc $inc_lib $string_params lib/string/memset.c -o bin/string/memset.o -c
	$base-gcc $inc_lib $string_params lib/string/memcpy.c -o bin/string/memcpy.o -c
	$base-gcc $inc_lib $string_params lib/string/memcmp.c -o bin/string/memcmp.o -c
	$base-gcc $inc_lib $string_params lib/string/strchr.c -o bin/string/strchr.o -c
	$base-gcc $inc_lib $string_params lib/string/strchrnul.c -o bin/string/strchrnul.o -c
	$base-gcc $inc_lib $string_params lib/string/strlen.c -o bin/string/strlen.o -c
	$base-gcc $inc_lib $string_params lib/string/strcpy.c -o bin/string/strcpy.o -c
#fi
#
#if [[ "$compile_fat" -eq 1 ]]
#then
#	rm bin/fat/*
	$base-gcc $inc_lib $fat_params driver/fat/diskio.c -o bin/fat/diskio.o -c
	$base-gcc $inc_lib $fat_params driver/fat/ff.c -o bin/fat/ff.o -c
	$base-gcc $inc_lib $fat_params driver/fat/ffsystem.c -o bin/fat/ffsystem.o -c
	$base-gcc $inc_lib $fat_params driver/fat/ffunicode.c -o bin/fat/ffunicode.o -c
	$base-gcc $inc_lib $fat_params driver/fat/fattime.c -o bin/fat/fattime.o -c
#fi

#rm bin/driver/*
$base-gcc $inc_lib -nostdlib -nodefaultlibs -nostdlib -fno-builtin -m68000 -O3 driver/PS2.c -o bin/driver/PS2.o -c 
$base-gcc $inc_lib -nostdlib -nodefaultlibs -nostdlib -fno-builtin -m68000 -O3 driver/sys.c -o bin/driver/sys.o -c
$base-gcc $inc_lib -nostdlib -nodefaultlibs -nostdlib -fno-builtin -m68000 -O3 driver/SPI.c -o bin/driver/SPI.o -c 
$base-gcc $inc_lib -nostdlib -nodefaultlibs -nostdlib -fno-builtin -m68000 -O3 driver/EventQ.c -o bin/driver/EventQ.o -c 



$base-gcc $inc_lib -m68000 crt0.s -o bin/crt/crt0.o -c
$base-gcc $inc_lib -nostdlib -nodefaultlibs -nostdlib -fno-builtin -m68000 -O3 main.c -o bin/main.o -c
$base-gcc $inc_lib -nostdlib -nodefaultlibs -nostdlib -fno-builtin -m68000 -O3 TextBuff.c -o bin/TextBuff.o -c 
$base-gcc $inc_lib -nostdlib -nodefaultlibs -nostdlib -fno-builtin -m68000 -O3 malloc.c -o bin/malloc.o -c 
$base-gcc $inc_lib -nostdlib -nodefaultlibs -nostdlib -fno-builtin -m68000 -O3 lib/sprintf/sprintf.c -o bin/sprintf.o -c

#$base-ld -T link.ld bin/*.o bin/libgcc/*.o bin/fat/*.o bin/string/*.o -o bin/bin.out
$base-ld -T link.ld bin/*.o bin/libgcc/*.o bin/driver/*.o -o bin/bin.out
#m68k-linux-gnu-ld -T link.ld bin/_*.o -o bin/bin.out

$base-objcopy -O binary bin/bin.out bin/OS.bin
#hexyl OS.bin
