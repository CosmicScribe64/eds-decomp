	thumb_func_start sub_0802F154
sub_0802F154: @ 0x0802F154
	push {r4, lr}
	add r4, r0, #0
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08008A1C
	cmp r0, #0
	beq _0802F18A
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08047170
	cmp r0, #0
	beq _0802F18A
	ldr r2, _0802F190 @ =0x020192E4
	ldrb r0, [r4, #2]
	lsl r3, r0, #0x1F
	lsr r1, r3, #0x1F
	ldr r0, _0802F194 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #8]
	lsl r0, r0, #0x1B
	cmp r0, #0
	bge _0802F198
_0802F18A:
	mov r0, #0
	b _0802F1B8
	.align 2, 0
_0802F190: .4byte 0x020192E4
_0802F194: .4byte 0x00000D64
_0802F198:
	lsr r0, r3, #0x1F
	ldr r1, _0802F1C0 @ =0x000007FF
	ldrh r4, [r4]
	and r1, r4
	lsl r1, r1, #1
	ldr r2, _0802F1C4 @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #0
	bl sub_08044224
	mov r1, #0
	cmp r0, #0
	ble _0802F1B6
	mov r1, #1
_0802F1B6:
	add r0, r1, #0
_0802F1B8:
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0802F1C0: .4byte 0x000007FF
_0802F1C4: .4byte gUnk_08622AB4
	thumb_func_end sub_0802F154

