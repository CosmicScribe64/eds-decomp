	thumb_func_start sub_0802CA30
sub_0802CA30: @ 0x0802CA30
	push {r4, lr}
	ldr r0, _0802CA48 @ =0x02017A40
	mov r1, #0xF9
	lsl r1, r1, #2
	add r4, r0, r1
	ldrb r0, [r4]
	cmp r0, #0
	beq _0802CA4C
	cmp r0, #1
	beq _0802CA6C
	b _0802CA94
	.align 2, 0
_0802CA48: .4byte 0x02017A40
_0802CA4C:
	ldr r0, _0802CA60 @ =0x00000206
	ldr r1, _0802CA64 @ =0x00000712
	ldr r3, _0802CA68 @ =0x080827C4
	mov r2, #0xB
	bl sub_080602A4
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0802CA94
_0802CA60: .4byte 0x00000206
_0802CA64: .4byte 0x00000712
_0802CA68: .4byte gUnk_080827C4
_0802CA6C:
	mov r0, #0xF0
	bl sub_08052F38
	cmp r0, #0
	beq _0802CA94
	ldr r1, _0802CA8C @ =0x0201CFB0
	ldr r2, _0802CA90 @ =0x00000824
	add r0, r1, r2
	ldr r0, [r0]
	add r2, #8
	add r1, r1, r2
	ldr r1, [r1]
	bl sub_08017FF4
	mov r0, #1
	b _0802CA96
_0802CA8C: .4byte 0x0201CFB0
_0802CA90: .4byte 0x00000824
_0802CA94:
	mov r0, #0
_0802CA96:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0802CA30

