	thumb_func_start sub_0806D198
sub_0806D198: @ 0x0806D198
	push {r4, lr}
	ldr r1, _0806D1C4 @ =0x081A724C
	ldr r0, _0806D1C8 @ =0x03000040
	ldr r2, _0806D1CC @ =0x0000485A
	add r4, r0, r2
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0806D1D0
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0806D1BE
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_0806D1BE:
	mov r0, #0
	b _0806D1D2
	.align 2, 0
_0806D1C4: .4byte gUnk_081A724C
_0806D1C8: .4byte 0x03000040
_0806D1CC: .4byte 0x0000485A
_0806D1D0:
	mov r0, #1
_0806D1D2:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0806D198

