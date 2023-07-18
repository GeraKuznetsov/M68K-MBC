/*
 * Modify the ROM and RAM base and sz (size) variables below to
 * suit your systems memory layout.
 *
 *   base indicates at what address a memory begins
 *   sz indicates the size of that memory
 *
 * Values can be specified in decimal, hexadecimal, or as units such
 * as 1K or 1M.
 *
 * Space for the stack may also be reserved here. The heap will then
 * consume all remaining RAM between the end of the .bss section and
 * the lowest address reserved for the stack.
 *
 * Since there are no controls in place to ensure the stack does not
 * grow into the heap, the user is responsible for reserving sufficient
 * stack space to prevent it overwriting the upper areas of the heap.
 */
_rom_base = 0;
_rom_sz = 1M;
_ram_base = 1M;
_ram_sz = 2M;

/* must be mult of 16 */
_stack_sz = 4K;

/*
 * Dont modify below this line (unless you know what youre doing),
 * except to add user interrupt vectors.
 */

/*STARTUP(bin/crt/crt0.o)*/
OUTPUT_ARCH(m68k)

__rom_end = (_rom_base + _rom_sz);
__ram_end = (_ram_base + _ram_sz);

__evt_org = _rom_base;
__text_org = _rom_base + 0x400;
__text_sz = _rom_sz - 0x400;
__data_org = _ram_base;
__data_sz = _ram_sz;

MEMORY {
    evt          (r!ax) : ORIGIN = __evt_org, LENGTH = 0x400
    text         (rx!w) : ORIGIN = __text_org, LENGTH = __text_sz
    data        (rwx!a) : ORIGIN = __data_org, LENGTH = __data_sz
}

SECTIONS {
    .evt : {
        /* Reset vector */
        LONG(__ram_end);                /* Initial SSP */
        LONG(ABSOLUTE(main));         /* Initial PC */

        /* Exception vector table */
        LONG(DEFINED(BusError) ? ABSOLUTE(BusError) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(AddressError) ? ABSOLUTE(AddressError) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(IllegalInstruction) ? ABSOLUTE(IllegalInstruction) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(ZeroDivide) ? ABSOLUTE(ZeroDivide) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(CHKInstruction) ? ABSOLUTE(CHKInstruction) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(TRAPVInstruction) ? ABSOLUTE(TRAPVInstruction) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(PrivilegeViolation) ? ABSOLUTE(PrivilegeViolation) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(Trace) ? ABSOLUTE(Trace) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(Line1010Emulator) ? ABSOLUTE(Line1010Emulator) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(Line1111Emulator) ? ABSOLUTE(Line1111Emulator) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(HardwareBreakpoint) ? ABSOLUTE(HardwareBreakpoint) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(CoprocessorProtocolViolation) ? ABSOLUTE(CoprocessorProtocolViolation) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(FormatError) ? ABSOLUTE(FormatError) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(UninitializedInterruptVector) ? ABSOLUTE(UninitializedInterruptVector) : ABSOLUTE(DefaultInterrupt));
        LONG(0); /* Unassigned, reserved */
        LONG(0); /* Unassigned, reserved */
        LONG(0); /* Unassigned, reserved */
        LONG(0); /* Unassigned, reserved */
        LONG(0); /* Unassigned, reserved */
        LONG(0); /* Unassigned, reserved */
        LONG(0); /* Unassigned, reserved */
        LONG(0); /* Unassigned, reserved */
        LONG(DEFINED(SpuriousInterrupt) ? ABSOLUTE(SpuriousInterrupt) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(IRQ1) ? ABSOLUTE(IRQ1) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(IRQ2) ? ABSOLUTE(IRQ2) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(IRQ3) ? ABSOLUTE(IRQ3) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(IRQ4) ? ABSOLUTE(IRQ4) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(IRQ5) ? ABSOLUTE(IRQ5) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(IRQ6) ? ABSOLUTE(IRQ6) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(IRQ7) ? ABSOLUTE(IRQ7) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(TRAP0) ? ABSOLUTE(TRAP0) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(TRAP1) ? ABSOLUTE(TRAP1) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(TRAP2) ? ABSOLUTE(TRAP2) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(TRAP3) ? ABSOLUTE(TRAP3) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(TRAP4) ? ABSOLUTE(TRAP4) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(TRAP5) ? ABSOLUTE(TRAP5) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(TRAP6) ? ABSOLUTE(TRAP6) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(TRAP7) ? ABSOLUTE(TRAP7) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(TRAP8) ? ABSOLUTE(TRAP8) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(TRAP9) ? ABSOLUTE(TRAP9) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(TRAP10) ? ABSOLUTE(TRAP10) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(TRAP11) ? ABSOLUTE(TRAP11) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(TRAP12) ? ABSOLUTE(TRAP12) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(TRAP13) ? ABSOLUTE(TRAP13) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(TRAP14) ? ABSOLUTE(TRAP14) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(TRAP15) ? ABSOLUTE(TRAP15) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(FPBranchOrSetUnorderedCondition) ? ABSOLUTE(FPBranchOrSetUnorderedCondition) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(FPInexactResult) ? ABSOLUTE(FPInexactResult) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(FPZeroDivide) ? ABSOLUTE(FPZeroDivide) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(FPUnderflow) ? ABSOLUTE(FPUnderflow) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(FPOperandError) ? ABSOLUTE(FPOperandError) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(FPOverflow) ? ABSOLUTE(FPOverflow) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(FPSignallingNAN) ? ABSOLUTE(FPSignallingNAN) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(FPUnimplementedDataType) ? ABSOLUTE(FPUnimplementedDataType) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(MMUConfigurationError) ? ABSOLUTE(MMUConfigurationError) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(MMUIllegalOperation) ? ABSOLUTE(MMUIllegalOperation) : ABSOLUTE(DefaultInterrupt));
        LONG(DEFINED(MMUAccessLevelViolation) ? ABSOLUTE(MMUAccessLevelViolation) : ABSOLUTE(DefaultInterrupt));
        LONG(0); /* Unassigned, reserved */
        LONG(0); /* Unassigned, reserved */
        LONG(0); /* Unassigned, reserved */
        LONG(0); /* Unassigned, reserved */
        LONG(0); /* Unassigned, reserved */
/*
 *
 * User Interrupt Vectors
 *
 *   Vector #         Address
 *   Hex    Dec       Hex      Dec
 *   40-FF  64-255    100-3FC  256-1020
 *
 * To calculate the offset within the .evt section to place your vector, use
 * the following calculation (assuming vector number = 0x50):
 *
 *   0x50 * 4 = 0x140
 *
 * You could then place your vector at that position in the vector table as
 * follows:
 *
 *   . = 0x140;
 *   LONG(ABSOLUTE(NameOfHandlerFunction));
 */
    } > evt
	. = 0x400;	
	
	.sysvt : {
		LONG(DEFINED(Sys_GetEvent)?ABSOLUTE(Sys_GetEvent) :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_open)?ABSOLUTE(f_open)             :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_close)?ABSOLUTE(f_close)           :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_read )?ABSOLUTE(f_read )           :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_write )?ABSOLUTE(f_write )         :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_lseek)?ABSOLUTE(f_lseek)           :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_truncate)?ABSOLUTE(f_truncate)     :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_sync)?ABSOLUTE(f_sync)             :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_opendir)?ABSOLUTE(f_opendir)       :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_closedir)?ABSOLUTE(f_closedir)     :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_readdir)?ABSOLUTE(f_readdir)       :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_findfirst) ?ABSOLUTE(f_findfirst)  :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_findnext)?ABSOLUTE(f_findnext)     :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_mkdir)?ABSOLUTE(f_mkdir)           :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_unlink)?ABSOLUTE(f_unlink)         :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_rename)?ABSOLUTE(f_rename)         :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_stat)?ABSOLUTE(f_stat)             :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_chmod)?ABSOLUTE(f_chmod)           :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_utime)?ABSOLUTE(f_utime)           :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_chdir )?ABSOLUTE(f_chdir )         :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_chdrive)?ABSOLUTE(f_chdrive)       :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_getcwd)?ABSOLUTE(f_getcwd)         :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_getfree)?ABSOLUTE(f_getfree)       :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_getlabel0)?ABSOLUTE(f_getlabel0)   :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_setlabel)?ABSOLUTE(f_setlabel)     :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_forward)?ABSOLUTE(f_forward)       :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_expand)?ABSOLUTE(f_expand)         :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_mount)?ABSOLUTE(f_mount)           :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_mkfs)?ABSOLUTE(f_mkfs)             :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_fdisk)?ABSOLUTE(f_fdisk)           :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_setcp)?ABSOLUTE(f_setcp)           :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_putc)?ABSOLUTE(f_putc)             :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_puts)?ABSOLUTE(f_puts)             :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_printf)?ABSOLUTE(f_printf)         :  ABSOLUTE(UNDEF_SYSCALL));
		LONG(DEFINED(f_gets)?ABSOLUTE(f_gets)                     :  ABSOLUTE(UNDEF_SYSCALL));
		
		LONG(DEFINED(GetFATFS)?ABSOLUTE(GetFATFS)                     :  ABSOLUTE(UNDEF_SYSCALL));
		
		LONG(DEFINED(Sys_RunProc)?ABSOLUTE(Sys_RunProc)                     :  ABSOLUTE(UNDEF_SYSCALL));
	} > text
	
   .text : {
        _text_start = .;
        *(.text .text.*)
        . = ALIGN(0x10);
        _text_end = .;
    } > text

    .rodata : {
        _rodata_start = .;
        *(.rodata .rodata.*)
        . = ALIGN(0x10);
        _rodata_end = .;
    } > text

    .data :  AT(_rodata_end){
        _data_start = .;
        *(.data .data.*)
        . = ALIGN(0x10);
        _data_end = .;
    } > data

    .bss : {
        _bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        . = ALIGN(0x10);
        _bss_end = .;
    } > data

    .stack : {
        _stack_start = .;
        . += _stack_sz;
        _stack_end = .;
    } > data

    .heap : {
        _heap_start = .;
		_HEAP_START = .;
        /*. += (_ram_sz - _stack_sz - SIZEOF(.bss) - SIZEOF(.data));
        _heap_end = .;*/
    } > data

}