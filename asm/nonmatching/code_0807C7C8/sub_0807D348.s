	thumb_func_start sub_0807D348
sub_0807D348: @ 0x0807D348
	push {r4, r5, r6, lr}
	ldr r0, _0807D39C @ =0x0201F780
	add r0, #0x22
	mov r6, #0
	mov r1, #0x50
	strb r1, [r0]
	ldr r5, _0807D3A0 @ =0x03000040
	ldr r0, _0807D3A4 @ =0x00004874
	add r2, r5, r0
	mov r0, #4
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #1
	orr r0, r1
	strb r0, [r2]
	ldr r1, _0807D3A8 @ =0x081A79A4
	ldr r2, _0807D3AC @ =0x00004859
	add r4, r5, r2
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0807D3B8
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0807D396
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	ldr r1, _0807D3B0 @ =0x0000485A
	add r0, r5, r1
	strb r6, [r0]
	ldr r2, _0807D3B4 @ =0x0000485B
	add r0, r5, r2
	strb r6, [r0]
_0807D396:
	mov r0, #0
	b _0807D3BA
	.align 2, 0
_0807D39C: .4byte 0x0201F780
_0807D3A0: .4byte 0x03000040
_0807D3A4: .4byte 0x00004874
_0807D3A8: .4byte gUnk_081A79A4
_0807D3AC: .4byte 0x00004859
_0807D3B0: .4byte 0x0000485A
_0807D3B4: .4byte 0x0000485B
_0807D3B8:
	mov r0, #1
_0807D3BA:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0807D348

