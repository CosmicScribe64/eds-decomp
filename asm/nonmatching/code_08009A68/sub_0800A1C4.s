	thumb_func_start sub_0800A1C4
sub_0800A1C4: @ 0x0800A1C4
	push {r4, r5, lr}
	mov r4, #0
	ldr r2, _0800A208 @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _0800A20C @ =0x00000D64
	mul r0, r1
	add r2, r0, r2
	ldrb r3, [r2, #2]
	cmp r4, r3
	bge _0800A224
	ldr r5, _0800A210 @ =0x000007FF
	add r2, r0, #0
_0800A1DE:
	ldr r0, _0800A214 @ =0x02019968
	add r0, r2, r0
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _0800A21C
	and r0, r5
	lsl r0, r0, #2
	ldr r1, _0800A218 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0800A21C
	add r0, r4, #0
	b _0800A228
	.align 2, 0
_0800A208: .4byte 0x020192E4
_0800A20C: .4byte 0x00000D64
_0800A210: .4byte 0x000007FF
_0800A214: .4byte 0x02019968
_0800A218: .4byte gUnk_08621DE0
_0800A21C:
	add r2, #4
	add r4, #1
	cmp r4, r3
	blt _0800A1DE
_0800A224:
	mov r0, #1
	neg r0, r0
_0800A228:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0800A1C4
	.align 2, 0

