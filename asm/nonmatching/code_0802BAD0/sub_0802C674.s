	thumb_func_start sub_0802C674
sub_0802C674: @ 0x0802C674
	push {r4, r5, r6, lr}
	add r6, r0, #0
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r5, r0, #0x18
	lsr r3, r1, #0x18
	mov r1, #1
	and r1, r5
	mov r0, #0x94
	add r2, r3, #0
	mul r2, r0
	ldr r0, _0802C6E8 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802C6EC @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	cmp r3, #4
	bgt _0802C774
	cmp r4, #0
	beq _0802C774
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _0802C774
	ldrh r0, [r6]
	add r1, r5, #0
	add r2, r3, #0
	bl sub_0802B1B8
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802C774
	ldr r2, _0802C6F0 @ =0x000007FF
	and r2, r4
	lsl r0, r2, #2
	ldr r1, _0802C6F4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0802C774
	lsl r0, r2, #1
	ldr r1, _0802C6F8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0802C6FC @ =0x00000776
	cmp r1, r0
	bne _0802C700
	mov r0, #3
	b _0802C762
	.align 2, 0
_0802C6E8: .4byte 0x00000D64
_0802C6EC: .4byte 0x0201930C
_0802C6F0: .4byte 0x000007FF
_0802C6F4: .4byte gUnk_08621DE0
_0802C6F8: .4byte gUnk_08622AB4
_0802C6FC: .4byte 0x00000776
_0802C700:
	cmp r1, r0
	blt _0802C710
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0802C710
	mov r0, #1
	b _0802C762
_0802C710:
	ldr r0, _0802C734 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0802C738 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0802C742
	cmp r0, #0x16
	bgt _0802C73C
	cmp r0, #0x15
	beq _0802C746
	b _0802C74E
	.align 2, 0
_0802C734: .4byte 0x000007FF
_0802C738: .4byte gUnk_08621DE0
_0802C73C:
	cmp r0, #0x17
	beq _0802C74A
	b _0802C74E
_0802C742:
	mov r0, #7
	b _0802C762
_0802C746:
	mov r0, #8
	b _0802C762
_0802C74A:
	mov r0, #9
	b _0802C762
_0802C74E:
	ldr r0, _0802C76C @ =0x000007FF
	and r4, r0
	lsl r0, r4, #2
	ldr r1, _0802C770 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0802C762:
	cmp r0, #2
	bne _0802C774
	mov r0, #1
	b _0802C776
	.align 2, 0
_0802C76C: .4byte 0x000007FF
_0802C770: .4byte gUnk_08621DE0
_0802C774:
	mov r0, #0
_0802C776:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0802C674

