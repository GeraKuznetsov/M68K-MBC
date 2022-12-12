CPU=68000

PREFIX=m68k-elf
	
CC = m68k-elf-gcc
CFLAGS = -nostdlib -nodefaultlibs -nostdlib -fno-builtin
LD = m68k-elf-ld
LDFLAGS = -T link.ld

PROG = prog
OBJS = _main.o _sys.o

all: $(PROG)

$(PROG): $(OBJS):
	echo "hi"
	
_main.o: main.c
	$(CC) $(CFLAGS) main.c -o _main.o -c
	
_sys.o: lib/sys.c
	$(CC) $(CFLAGS) lib/sys.c -o _sys.o -c
	
