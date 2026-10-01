	thumb_func_start sub_0802CBEC
sub_0802CBEC: @ 0x0802CBEC
	push {r4, r5, lr}
	ldr r0, _0802CC04 @ =0x02017A40
	mov r1, #0xF9
	lsl r1, r1, #2
	add r5, r0, r1
	ldrb r4, [r5]
	cmp r4, #0
	beq _0802CC08
	cmp r4, #1
	beq _0802CC28
	b _0802CC96
	.align 2, 0
_0802CC04: .4byte 0x02017A40
_0802CC08:
	ldr r0, _0802CC1C @ =0x00000206
	ldr r1, _0802CC20 @ =0x00000712
	ldr r3, _0802CC24 @ =0x08082810
	mov r2, #0xB
	bl sub_080602A4
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	b _0802CC96
_0802CC1C: .4byte 0x00000206
_0802CC20: .4byte 0x00000712
_0802CC24: .4byte gUnk_08082810
_0802CC28:
	mov r0, #1
	bl sub_08052F38
	cmp r0, #0
	beq _0802CC96
	ldr r0, _0802CC78 @ =0x0201CFB0
	ldr r2, _0802CC7C @ =0x00000824
	add r1, r0, r2
	ldr r3, [r1]
	ldr r1, _0802CC80 @ =0x0000082C
	add r0, r0, r1
	ldr r2, [r0]
	and r4, r3
	lsl r0, r2, #2
	ldr r1, _0802CC84 @ =0x00000D64
	mul r1, r4
	add r0, r0, r1
	ldr r1, _0802CC88 @ =0x02019968
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x13
	ldr r1, _0802CC8C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0802CC90
	add r0, r3, #0
	add r1, r2, #0
	mov r2, #0
	mov r3, #1
	bl sub_080193D4
	mov r0, #1
	b _0802CC98
	.align 2, 0
_0802CC78: .4byte 0x0201CFB0
_0802CC7C: .4byte 0x00000824
_0802CC80: .4byte 0x0000082C
_0802CC84: .4byte 0x00000D64
_0802CC88: .4byte 0x02019968
_0802CC8C: .4byte gUnk_08621DE0
_0802CC90:
	mov r0, #3
	bl sub_08077AEC
_0802CC96:
	mov r0, #0
_0802CC98:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0802CBEC
	.align 2, 0

