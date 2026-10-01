	thumb_func_start sub_0807D0A0
sub_0807D0A0: @ 0x0807D0A0
	push {r4, lr}
	bl sub_0806F01C
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0807D102
	ldr r1, _0807D0CC @ =0x03000040
	ldr r2, _0807D0D0 @ =0x00004872
	add r0, r1, r2
	ldrh r3, [r0]
	add r2, r3, #0
	add r4, r1, #0
	cmp r2, #0
	beq _0807D0D8
	ldr r0, _0807D0D4 @ =0x0201F780
	strh r3, [r0]
	mov r1, #1
	ldrb r2, [r0, #2]
	orr r1, r2
	mov r2, #2
	orr r1, r2
	b _0807D0EA
_0807D0CC: .4byte 0x03000040
_0807D0D0: .4byte 0x00004872
_0807D0D4: .4byte 0x0201F780
_0807D0D8:
	ldr r0, _0807D10C @ =0x0201F780
	strh r2, [r0]
	mov r1, #2
	neg r1, r1
	ldrb r2, [r0, #2]
	and r1, r2
	mov r2, #3
	neg r2, r2
	and r1, r2
_0807D0EA:
	strb r1, [r0, #2]
	ldr r0, _0807D110 @ =0x00004859
	add r1, r4, r0
	mov r2, #0
	mov r0, #1
	strb r0, [r1]
	ldr r1, _0807D114 @ =0x0000485A
	add r0, r4, r1
	strb r2, [r0]
	add r1, #1
	add r0, r4, r1
	strb r2, [r0]
_0807D102:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0807D10C: .4byte 0x0201F780
_0807D110: .4byte 0x00004859
_0807D114: .4byte 0x0000485A
	thumb_func_end sub_0807D0A0

