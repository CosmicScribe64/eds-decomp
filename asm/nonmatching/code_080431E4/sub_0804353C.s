	thumb_func_start sub_0804353C
sub_0804353C: @ 0x0804353C
	push {r4, lr}
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	mov r3, #0
	ldr r2, _0804356C @ =0x0819A990
	ldr r0, [r2]
	lsl r0, r0, #6
	lsr r0, r0, #0x13
	cmp r0, #0
	beq _08043588
	ldr r0, _08043570 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	add r1, r2, #0
	ldr r4, _08043574 @ =0x08622AB4
	add r0, r0, r4
	ldrh r4, [r0]
_0804355E:
	ldr r0, [r2]
	lsl r0, r0, #6
	lsr r0, r0, #0x13
	cmp r0, r4
	bne _08043578
	add r0, r3, #0
	b _0804358C
_0804356C: .4byte gUnk_0819A990
_08043570: .4byte 0x000007FF
_08043574: .4byte gUnk_08622AB4
_08043578:
	add r1, #4
	add r2, #4
	add r3, #1
	ldr r0, [r1]
	lsl r0, r0, #6
	lsr r0, r0, #0x13
	cmp r0, #0
	bne _0804355E
_08043588:
	mov r0, #1
	neg r0, r0
_0804358C:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0804353C
	.align 2, 0

