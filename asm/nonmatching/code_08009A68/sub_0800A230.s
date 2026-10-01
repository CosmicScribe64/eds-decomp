	thumb_func_start sub_0800A230
sub_0800A230: @ 0x0800A230
	push {r4, r5, lr}
	mov r4, #0
	ldr r2, _0800A280 @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _0800A284 @ =0x00000D64
	mul r0, r1
	add r2, r0, r2
	ldrb r3, [r2, #2]
	cmp r4, r3
	bge _0800A29C
	add r1, r0, #0
	ldr r5, _0800A288 @ =0x000007FF
	add r2, r3, #0
_0800A24C:
	ldr r0, _0800A28C @ =0x02019968
	add r0, r1, r0
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _0800A294
	and r0, r5
	lsl r0, r0, #2
	ldr r3, _0800A290 @ =0x08621DE0
	add r0, r0, r3
	ldr r3, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0800A294
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r3, r0
	lsr r0, r3, #0x11
	cmp r0, #2
	beq _0800A294
	add r0, r4, #0
	b _0800A2A0
_0800A280: .4byte 0x020192E4
_0800A284: .4byte 0x00000D64
_0800A288: .4byte 0x000007FF
_0800A28C: .4byte 0x02019968
_0800A290: .4byte gUnk_08621DE0
_0800A294:
	add r1, #4
	add r4, #1
	cmp r4, r2
	blt _0800A24C
_0800A29C:
	mov r0, #1
	neg r0, r0
_0800A2A0:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0800A230
	.align 2, 0

