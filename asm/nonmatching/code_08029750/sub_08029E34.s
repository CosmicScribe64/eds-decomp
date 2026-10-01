	thumb_func_start sub_08029E34
sub_08029E34: @ 0x08029E34
	push {r4, lr}
	ldr r1, _08029E60 @ =0x0819A72C
	ldr r0, _08029E64 @ =0x03000040
	ldr r2, _08029E68 @ =0x00004859
	add r4, r0, r2
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08029E6C
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08029E5A
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08029E5A:
	mov r0, #0
	b _08029E6E
	.align 2, 0
_08029E60: .4byte gUnk_0819A72C
_08029E64: .4byte 0x03000040
_08029E68: .4byte 0x00004859
_08029E6C:
	mov r0, #1
_08029E6E:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08029E34

