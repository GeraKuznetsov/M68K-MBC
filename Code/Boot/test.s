#NO_APP
	.file	"test.c"
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	link.w %fp,#0
	move.l %d2,-(%sp)
	move.w #0,3145728
	move.l #4194304,%a1
.L2:
	move.l %a1,%d1
	add.l #30720,%d1
	move.l %a1,%d0
.L3:
	move.l %d0,%a0
	move.w #0,(%a0)+
	move.w #0,(%a0)
	add.l #512,%d0
	cmp.l %d0,%d1
	jne .L3
	addq.l #4,%a1
	cmp.l #4194624,%a1
	jne .L2
	sub.l %a0,%a0
	moveq #0,%d0
.L5:
	move.l (%a0)+,%d1
	add.l %d1,%d0
	cmp.l #131072,%a0
	jne .L5
	lea romCheckMsg+1,%a1
	moveq #82,%d1
	move.l #4194304,%a0
	and.w #255,%d1
	move.w %d1,(%a0)
	move.w #7,2(%a0)
	move.l %a0,%d2
	addq.l #4,%d2
	move.b (%a1)+,%d1
	jeq .L33
.L19:
	move.l %d2,%a0
	and.w #255,%d1
	move.w %d1,(%a0)
	move.w #7,2(%a0)
	move.l %a0,%d2
	addq.l #4,%d2
	move.b (%a1)+,%d1
	jne .L19
.L33:
	lea toHex,%a1
	move.l %d0,%d1
	moveq #28,%d2
	lsr.l %d2,%d1
	move.b (%a1,%d1.l),%d1
	and.w #255,%d1
	move.w %d1,4(%a0)
	move.w #7,6(%a0)
	move.l %d0,%d1
	clr.w %d1
	swap %d1
	lsr.w #8,%d1
	moveq #15,%d2
	and.l %d2,%d1
	move.b (%a1,%d1.l),%d1
	and.w #255,%d1
	move.w %d1,8(%a0)
	move.w #7,10(%a0)
	move.l %d0,%d1
	clr.w %d1
	swap %d1
	lsr.w #4,%d1
	and.l %d2,%d1
	move.b (%a1,%d1.l),%d1
	and.w #255,%d1
	move.w %d1,12(%a0)
	move.w #7,14(%a0)
	move.l %d0,%d1
	clr.w %d1
	swap %d1
	and.l %d2,%d1
	move.b (%a1,%d1.l),%d1
	and.w #255,%d1
	move.w %d1,16(%a0)
	move.w #7,18(%a0)
	move.l %d0,%d1
	moveq #12,%d2
	lsr.l %d2,%d1
	moveq #15,%d2
	and.l %d2,%d1
	move.b (%a1,%d1.l),%d1
	and.w #255,%d1
	move.w %d1,20(%a0)
	move.w #7,22(%a0)
	move.l %d0,%d1
	lsr.l #8,%d1
	and.l %d2,%d1
	move.b (%a1,%d1.l),%d1
	and.w #255,%d1
	move.w %d1,24(%a0)
	move.w #7,26(%a0)
	move.l %d0,%d1
	lsr.l #4,%d1
	and.l %d2,%d1
	move.b (%a1,%d1.l),%d1
	and.w #255,%d1
	move.w %d1,28(%a0)
	move.w #7,30(%a0)
	and.l %d2,%d0
	move.b (%a1,%d0.l),%d0
	and.w #255,%d0
	move.w %d0,32(%a0)
	move.w #7,34(%a0)
	move.w #1,3145728
	move.l #1048576,%a0
.L7:
	moveq #-1,%d0
	move.l %d0,(%a0)+
	cmp.l #2097152,%a0
	jne .L7
	move.l #1048576,%a0
.L10:
	moveq #-1,%d1
	cmp.l (%a0)+,%d1
	jne .L12
	cmp.l #2097152,%a0
	jne .L10
	move.l #1048576,%a0
.L11:
	clr.l (%a0)+
	cmp.l #2097152,%a0
	jne .L11
	move.l #1048576,%a0
.L13:
	tst.l (%a0)+
	jne .L12
	cmp.l #2097152,%a0
	jne .L13
	lea ramPassMsg+1,%a1
	moveq #82,%d0
	move.l #4194560,%a0
.L14:
	and.w #255,%d0
	move.w %d0,(%a0)
	move.w #7,2(%a0)
	addq.l #4,%a0
	move.b (%a1)+,%d0
	jne .L14
	lea bootMsg+1,%a1
	moveq #66,%d0
	move.l #4194816,%a0
.L17:
	and.w #255,%d0
	move.w %d0,(%a0)
	move.w #7,2(%a0)
	addq.l #4,%a0
	move.b (%a1)+,%d0
	jne .L17
	move.w #1,3145728
.L18:
	jra .L18
.L12:
	lea ramFailMsg+1,%a1
	moveq #82,%d0
	move.l #4194560,%a0
.L9:
	and.w #255,%d0
	move.w %d0,(%a0)
	move.w #7,2(%a0)
	addq.l #4,%a0
	move.b (%a1)+,%d0
	jne .L9
	move.w #1,3145728
.L16:
	jra .L16
	.size	main, .-main
	.globl	bootMsg
	.section	.rodata
	.type	bootMsg, @object
	.size	bootMsg, 5
bootMsg:
	.string	"BOOT"
	.globl	ramFailMsg
	.type	ramFailMsg, @object
	.size	ramFailMsg, 21
ramFailMsg:
	.string	"RAM 0: FAIL, NO BOOT"
	.globl	ramPassMsg
	.type	ramPassMsg, @object
	.size	ramPassMsg, 12
ramPassMsg:
	.string	"RAM 0: PASS"
	.globl	romCheckMsg
	.type	romCheckMsg, @object
	.size	romCheckMsg, 15
romCheckMsg:
	.string	"ROM CHECKSUM: "
	.globl	toHex
	.type	toHex, @object
	.size	toHex, 16
toHex:
	.byte	48
	.byte	49
	.byte	50
	.byte	51
	.byte	52
	.byte	53
	.byte	54
	.byte	55
	.byte	56
	.byte	57
	.byte	65
	.byte	66
	.byte	67
	.byte	68
	.byte	69
	.byte	70
	.globl	VGA_PORT
	.align	2
	.type	VGA_PORT, @object
	.size	VGA_PORT, 4
VGA_PORT:
	.long	4194304
	.globl	IO_PORT
	.align	2
	.type	IO_PORT, @object
	.size	IO_PORT, 4
IO_PORT:
	.long	3145728
	.ident	"GCC: (GNU) 6.3.0"
