	thumb_func_start sub_080725B0
sub_080725B0: @ 0x080725B0
	push {r4, r5, lr}
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	lsl r2, r2, #0x10
	lsr r4, r2, #0x10
	cmp r0, #0xF
	bls _080725C4
	b _08072772
_080725C4:
	lsl r0, r0, #2
	ldr r1, _080725D0 @ =0x080725D4
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_080725D0: .4byte 0x080725D4
_080725D4:
	.4byte _08072614
	.4byte _08072624
	.4byte _0807263C
	.4byte _08072652
	.4byte _0807266A
	.4byte _08072682
	.4byte _08072696
	.4byte _080726AE
	.4byte _080726C4
	.4byte _080726DA
	.4byte _080726F2
	.4byte _08072706
	.4byte _0807271E
	.4byte _08072736
	.4byte _0807274C
	.4byte _08072764
_08072614:
	mov r0, #0xF
	add r1, r4, #0
	and r1, r0
	lsl r0, r1, #4
	orr r0, r1
	lsl r1, r0, #8
	orr r0, r1
	b _08072772
_08072624:
	mov r3, #0xF
	add r2, r4, #0
	and r2, r3
	lsl r0, r2, #4
	orr r0, r2
	add r1, r5, #0
	and r1, r3
	lsl r1, r1, #4
	orr r1, r2
	lsl r1, r1, #8
	orr r0, r1
	b _08072772
_0807263C:
	mov r3, #0xF
	add r0, r4, #0
	and r0, r3
	lsl r2, r0, #4
	orr r0, r2
	add r1, r5, #0
	and r1, r3
	orr r1, r2
	lsl r1, r1, #8
	orr r0, r1
	b _08072772
_08072652:
	mov r3, #0xF
	add r1, r4, #0
	and r1, r3
	lsl r0, r1, #4
	orr r0, r1
	add r2, r5, #0
	and r2, r3
	lsl r1, r2, #4
	orr r1, r2
	lsl r1, r1, #8
	orr r0, r1
	b _08072772
_0807266A:
	mov r1, #0xF
	add r2, r4, #0
	and r2, r1
	add r0, r5, #0
	and r0, r1
	lsl r0, r0, #4
	orr r0, r2
	lsl r1, r2, #4
	orr r1, r2
	lsl r1, r1, #8
	orr r0, r1
	b _08072772
_08072682:
	mov r2, #0xF
	add r0, r4, #0
	and r0, r2
	add r1, r5, #0
	and r1, r2
	lsl r1, r1, #4
	orr r0, r1
	lsl r1, r0, #8
	orr r0, r1
	b _08072772
_08072696:
	mov r0, #0xF
	add r1, r4, #0
	and r1, r0
	add r2, r5, #0
	and r2, r0
	lsl r0, r2, #4
	orr r0, r1
	lsl r1, r1, #4
	orr r1, r2
	lsl r1, r1, #8
	orr r0, r1
	b _08072772
_080726AE:
	mov r1, #0xF
	add r0, r4, #0
	and r0, r1
	add r2, r5, #0
	and r2, r1
	lsl r1, r2, #4
	orr r0, r1
	orr r1, r2
	lsl r1, r1, #8
	orr r0, r1
	b _08072772
_080726C4:
	mov r1, #0xF
	add r0, r5, #0
	and r0, r1
	add r2, r4, #0
	and r2, r1
	lsl r1, r2, #4
	orr r0, r1
	orr r1, r2
	lsl r1, r1, #8
	orr r0, r1
	b _08072772
_080726DA:
	mov r0, #0xF
	add r1, r5, #0
	and r1, r0
	add r2, r4, #0
	and r2, r0
	lsl r0, r2, #4
	orr r0, r1
	lsl r1, r1, #4
	orr r1, r2
	lsl r1, r1, #8
	orr r0, r1
	b _08072772
_080726F2:
	mov r2, #0xF
	add r0, r5, #0
	and r0, r2
	add r1, r4, #0
	and r1, r2
	lsl r1, r1, #4
	orr r0, r1
	lsl r1, r0, #8
	orr r0, r1
	b _08072772
_08072706:
	mov r1, #0xF
	add r2, r5, #0
	and r2, r1
	add r0, r4, #0
	and r0, r1
	lsl r0, r0, #4
	orr r0, r2
	lsl r1, r2, #4
	orr r1, r2
	lsl r1, r1, #8
	orr r0, r1
	b _08072772
_0807271E:
	mov r3, #0xF
	add r1, r5, #0
	and r1, r3
	lsl r0, r1, #4
	orr r0, r1
	add r2, r4, #0
	and r2, r3
	lsl r1, r2, #4
	orr r1, r2
	lsl r1, r1, #8
	orr r0, r1
	b _08072772
_08072736:
	mov r3, #0xF
	add r0, r5, #0
	and r0, r3
	lsl r2, r0, #4
	orr r0, r2
	add r1, r4, #0
	and r1, r3
	orr r1, r2
	lsl r1, r1, #8
	orr r0, r1
	b _08072772
_0807274C:
	mov r3, #0xF
	add r2, r5, #0
	and r2, r3
	lsl r0, r2, #4
	orr r0, r2
	add r1, r4, #0
	and r1, r3
	lsl r1, r1, #4
	orr r1, r2
	lsl r1, r1, #8
	orr r0, r1
	b _08072772
_08072764:
	mov r0, #0xF
	add r1, r5, #0
	and r1, r0
	lsl r0, r1, #4
	orr r0, r1
	lsl r1, r0, #8
	orr r0, r1
_08072772:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_080725B0

