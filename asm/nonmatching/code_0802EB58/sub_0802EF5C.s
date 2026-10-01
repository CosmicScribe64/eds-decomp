	thumb_func_start sub_0802EF5C
sub_0802EF5C: @ 0x0802EF5C
	push {r4, r5, lr}
	add r3, r0, #0
	add r5, r1, #0
	lsl r2, r2, #0x10
	lsr r4, r2, #0x10
	ldr r2, _0802EF8C @ =0x020192E4
	ldrb r1, [r3, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802EF90 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldr r1, _0802EF94 @ =0x0000031F
	ldrh r0, [r0]
	cmp r0, r1
	bls _0802EF98
	add r0, r3, #0
	add r1, r5, #0
	add r2, r4, #0
	bl sub_0802EB00
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _0802EF9A
_0802EF8C: .4byte 0x020192E4
_0802EF90: .4byte 0x00000D64
_0802EF94: .4byte 0x0000031F
_0802EF98:
	mov r0, #0
_0802EF9A:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0802EF5C

