#NO_APP
	.file	"main.c"
	.globl	VGA_PORT
	.section	.rodata
	.align	2
	.type	VGA_PORT, @object
	.size	VGA_PORT, 4
VGA_PORT:
	.long	4194304
	.globl	PS2R_SYSW
	.align	2
	.type	PS2R_SYSW, @object
	.size	PS2R_SYSW, 4
PS2R_SYSW:
	.long	3145728
	.text
	.align	2
	.globl	EINT
	.type	EINT, @function
EINT:
	link.w %fp,#0
#APP
| 35 "main.c" 1
	and.w  #0xF8FF, %sr 
| 0 "" 2
#NO_APP
	nop
	unlk %fp
	rts
	.size	EINT, .-EINT
	.align	2
	.globl	DINT
	.type	DINT, @function
DINT:
	link.w %fp,#0
#APP
| 39 "main.c" 1
	or.w  #0x0700, %sr 
| 0 "" 2
#NO_APP
	nop
	unlk %fp
	rts
	.size	DINT, .-DINT
	.align	2
	.globl	writeString
	.type	writeString, @function
writeString:
	link.w %fp,#0
	move.l #3145728,%a0
	move.b #0,(%a0)
	jra .L4
.L5:
	move.l 20(%fp),%d0
	lsl.l #7,%d0
	add.l 16(%fp),%d0
	add.l %d0,%d0
	add.l %d0,%d0
	move.l 12(%fp),%a0
	add.l %d0,%a0
	move.l 8(%fp),%a1
	move.b (%a1),%d0
	move.b %d0,%d0
	and.w #255,%d0
	move.w %d0,(%a0)
	move.l 20(%fp),%d0
	lsl.l #7,%d0
	add.l 16(%fp),%d0
	add.l %d0,%d0
	add.l %d0,%d0
	addq.l #2,%d0
	move.l 12(%fp),%a0
	add.l %d0,%a0
	move.l 24(%fp),%d0
	move.w %d0,(%a0)
	addq.l #1,16(%fp)
	addq.l #1,8(%fp)
.L4:
	move.l 8(%fp),%a0
	move.b (%a0),%d0
	tst.b %d0
	jne .L5
	move.l #3145728,%a0
	move.b #1,(%a0)
	nop
	unlk %fp
	rts
	.size	writeString, .-writeString
	.globl	y
	.section	.bss
	.type	y, @object
	.size	y, 1
y:
	.zero	1
	.section	.rodata
.LC0:
	.string	"Hello World!"
	.text
	.align	2
	.globl	main
	.type	main, @function
main:
	link.w %fp,#-8
	move.l #3145728,%a0
	move.b #0,(%a0)
	clr.l -4(%fp)
	jra .L7
.L10:
	clr.l -8(%fp)
	jra .L8
.L9:
	moveq #64,%d1
	swap %d1
	move.l -8(%fp),%d0
	lsl.l #7,%d0
	add.l -4(%fp),%d0
	add.l %d0,%d0
	add.l %d0,%d0
	move.l %d1,%a0
	add.l %d0,%a0
	move.w #0,(%a0)
	moveq #64,%d1
	swap %d1
	move.l -8(%fp),%d0
	lsl.l #7,%d0
	add.l -4(%fp),%d0
	add.l %d0,%d0
	add.l %d0,%d0
	addq.l #2,%d0
	move.l %d1,%a0
	add.l %d0,%a0
	move.w #0,(%a0)
	addq.l #1,-8(%fp)
.L8:
	moveq #59,%d0
	cmp.l -8(%fp),%d0
	jge .L9
	addq.l #1,-4(%fp)
.L7:
	moveq #79,%d0
	cmp.l -4(%fp),%d0
	jge .L10
	move.l #3145728,%a0
	move.b #1,(%a0)
	move.b y,%d0
	move.b %d0,%d1
	addq.b #1,%d1
	move.b %d1,y
	move.b %d0,%d0
	and.l #255,%d0
	moveq #64,%d1
	swap %d1
	pea 56.w
	move.l %d0,-(%sp)
	clr.l -(%sp)
	move.l %d1,-(%sp)
	pea .LC0
	jsr writeString
	lea (20,%sp),%sp
	jsr EINT
.L11:
	jra .L11
	.size	main, .-main
	.section	.rodata
.LC1:
	.string	"IRQ1"
	.text
	.align	2
	.globl	IRQ1
	.type	IRQ1, @function
IRQ1:
	link.w %fp,#0
	movem.l #49344,-(%sp)
	move.b y,%d0
	move.b %d0,%d1
	addq.b #1,%d1
	move.b %d1,y
	move.b %d0,%d0
	and.l #255,%d0
	moveq #64,%d1
	swap %d1
	pea 56.w
	move.l %d0,-(%sp)
	clr.l -(%sp)
	move.l %d1,-(%sp)
	pea .LC1
	jsr writeString
	lea (20,%sp),%sp
	nop
	movem.l -16(%fp),#771
	unlk %fp
	rte
	.size	IRQ1, .-IRQ1
	.section	.rodata
.LC2:
	.string	"IRQ2"
	.text
	.align	2
	.globl	IRQ2
	.type	IRQ2, @function
IRQ2:
	link.w %fp,#0
	movem.l #49344,-(%sp)
	move.b y,%d0
	move.b %d0,%d1
	addq.b #1,%d1
	move.b %d1,y
	move.b %d0,%d0
	and.l #255,%d0
	moveq #64,%d1
	swap %d1
	pea 56.w
	move.l %d0,-(%sp)
	clr.l -(%sp)
	move.l %d1,-(%sp)
	pea .LC2
	jsr writeString
	lea (20,%sp),%sp
	nop
	movem.l -16(%fp),#771
	unlk %fp
	rte
	.size	IRQ2, .-IRQ2
	.section	.rodata
.LC3:
	.string	"IRQ3"
	.text
	.align	2
	.globl	IRQ3
	.type	IRQ3, @function
IRQ3:
	link.w %fp,#0
	movem.l #49344,-(%sp)
	move.b y,%d0
	move.b %d0,%d1
	addq.b #1,%d1
	move.b %d1,y
	move.b %d0,%d0
	and.l #255,%d0
	moveq #64,%d1
	swap %d1
	pea 56.w
	move.l %d0,-(%sp)
	clr.l -(%sp)
	move.l %d1,-(%sp)
	pea .LC3
	jsr writeString
	lea (20,%sp),%sp
	nop
	movem.l -16(%fp),#771
	unlk %fp
	rte
	.size	IRQ3, .-IRQ3
	.globl	toHex
	.section	.rodata
.LC4:
	.string	"0123456789ABCDEF"
	.data
	.align	2
	.type	toHex, @object
	.size	toHex, 4
toHex:
	.long	.LC4
	.text
	.align	2
	.globl	IRQ4
	.type	IRQ4, @function
IRQ4:
	link.w %fp,#-4
	movem.l #57536,-(%sp)
	move.l #3145728,%a0
	move.b (%a0),-1(%fp)
	move.l toHex,%d1
	move.b -1(%fp),%d0
	lsr.b #4,%d0
	move.b %d0,%d0
	and.l #255,%d0
	moveq #15,%d2
	and.l %d2,%d0
	move.l %d1,%a0
	add.l %d0,%a0
	move.b (%a0),%d0
	move.b %d0,-4(%fp)
	move.l toHex,%d1
	moveq #0,%d0
	move.b -1(%fp),%d0
	moveq #15,%d2
	and.l %d2,%d0
	move.l %d1,%a0
	add.l %d0,%a0
	move.b (%a0),%d0
	move.b %d0,-3(%fp)
	clr.b -2(%fp)
	move.b y,%d0
	move.b %d0,%d1
	addq.b #1,%d1
	move.b %d1,y
	move.b %d0,%d0
	and.l #255,%d0
	moveq #64,%d1
	swap %d1
	pea 56.w
	move.l %d0,-(%sp)
	clr.l -(%sp)
	move.l %d1,-(%sp)
	move.l %fp,%d0
	subq.l #4,%d0
	move.l %d0,-(%sp)
	jsr writeString
	lea (20,%sp),%sp
	nop
	movem.l -24(%fp),#775
	unlk %fp
	rte
	.size	IRQ4, .-IRQ4
	.section	.rodata
.LC5:
	.string	"IRQ5"
	.text
	.align	2
	.globl	IRQ5
	.type	IRQ5, @function
IRQ5:
	link.w %fp,#0
	movem.l #49344,-(%sp)
	move.b y,%d0
	move.b %d0,%d1
	addq.b #1,%d1
	move.b %d1,y
	move.b %d0,%d0
	and.l #255,%d0
	moveq #64,%d1
	swap %d1
	pea 56.w
	move.l %d0,-(%sp)
	clr.l -(%sp)
	move.l %d1,-(%sp)
	pea .LC5
	jsr writeString
	lea (20,%sp),%sp
	nop
	movem.l -16(%fp),#771
	unlk %fp
	rte
	.size	IRQ5, .-IRQ5
	.section	.rodata
.LC6:
	.string	"IRQ6"
	.text
	.align	2
	.globl	IRQ6
	.type	IRQ6, @function
IRQ6:
	link.w %fp,#0
	movem.l #49344,-(%sp)
	move.b y,%d0
	move.b %d0,%d1
	addq.b #1,%d1
	move.b %d1,y
	move.b %d0,%d0
	and.l #255,%d0
	moveq #64,%d1
	swap %d1
	pea 56.w
	move.l %d0,-(%sp)
	clr.l -(%sp)
	move.l %d1,-(%sp)
	pea .LC6
	jsr writeString
	lea (20,%sp),%sp
	nop
	movem.l -16(%fp),#771
	unlk %fp
	rte
	.size	IRQ6, .-IRQ6
	.section	.rodata
.LC7:
	.string	"IRQ7"
	.text
	.align	2
	.globl	IRQ7
	.type	IRQ7, @function
IRQ7:
	link.w %fp,#0
	movem.l #49344,-(%sp)
	move.b y,%d0
	move.b %d0,%d1
	addq.b #1,%d1
	move.b %d1,y
	move.b %d0,%d0
	and.l #255,%d0
	moveq #64,%d1
	swap %d1
	pea 56.w
	move.l %d0,-(%sp)
	clr.l -(%sp)
	move.l %d1,-(%sp)
	pea .LC7
	jsr writeString
	lea (20,%sp),%sp
	nop
	movem.l -16(%fp),#771
	unlk %fp
	rte
	.size	IRQ7, .-IRQ7
	.ident	"GCC: (GNU) 6.3.0"
