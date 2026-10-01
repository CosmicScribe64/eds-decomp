	thumb_func_start sub_0802F5B8
sub_0802F5B8: @ 0x0802F5B8
	push {r4, r5, lr}
	add r4, r0, #0
	lsl r2, r2, #0x10
	lsr r5, r2, #0x10
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08008860
	cmp r0, #0
	beq _0802F5F0
	ldr r2, _0802F5E8 @ =0x020192E4
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802F5EC @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	cmp r0, r5
	beq _0802F5F0
	mov r0, #1
	b _0802F5F2
	.align 2, 0
_0802F5E8: .4byte 0x020192E4
_0802F5EC: .4byte 0x00000D64
_0802F5F0:
	mov r0, #0
_0802F5F2:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0802F5B8

