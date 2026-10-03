	thumb_func_start Campaign_SelectOpponent
Campaign_SelectOpponent: @ 0x0801BCFC
	push {r4, r5, r6, lr}
	ldr r5, _0801BD18 @ =0x03000040
	ldr r0, _0801BD1C @ =0x0000488A
	add r6, r5, r0
	ldrh r3, [r6]
	lsl r2, r3, #0x14
	lsr r0, r2, #0x18
	cmp r0, #1
	beq _0801BD8E
	cmp r0, #1
	bgt _0801BD20
	cmp r0, #0
	beq _0801BD26
	b _0801BE04
_0801BD18: .4byte 0x03000040
_0801BD1C: .4byte 0x0000488A
_0801BD20:
	cmp r0, #2
	beq _0801BDAE
	b _0801BE04
_0801BD26:
	ldr r0, _0801BD6C @ =0x00004888
	add r1, r5, r0
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, #0
	bne _0801BD7C
	bl OpponentSelect_Run
	cmp r0, #0
	beq _0801BDE6
	ldrh r2, [r6]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801BD70 @ =0xFFFFF00F
	and r0, r2
	orr r0, r1
	strh r0, [r6]
	ldr r1, _0801BD74 @ =0x00004859
	add r0, r5, r1
	strb r4, [r0]
	ldr r3, _0801BD78 @ =0x0000485A
	add r0, r5, r3
	strb r4, [r0]
	add r1, #2
	add r0, r5, r1
	strb r4, [r0]
	b _0801BDE6
	.align 2, 0
_0801BD6C: .4byte 0x00004888
_0801BD70: .4byte 0xFFFFF00F
_0801BD74: .4byte 0x00004859
_0801BD78: .4byte 0x0000485A
_0801BD7C:
	lsr r0, r2, #0x18
	add r0, #1
	mov r1, #0xFF
	and r0, r1
	lsl r0, r0, #4
	ldr r1, _0801BDEC @ =0xFFFFF00F
	and r1, r3
	orr r1, r0
	strh r1, [r6]
_0801BD8E:
	bl Campaign_StartPreDuelDialogue
	ldr r2, _0801BDF0 @ =0x03000040
	ldr r3, _0801BDF4 @ =0x0000488A
	add r2, r2, r3
	ldrh r3, [r2]
	lsl r1, r3, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801BDEC @ =0xFFFFF00F
	and r0, r3
	orr r0, r1
	strh r0, [r2]
_0801BDAE:
	bl CB_Bustup
	cmp r0, #0
	beq _0801BDE6
	ldr r2, _0801BDF0 @ =0x03000040
	ldr r0, _0801BDF4 @ =0x0000488A
	add r4, r2, r0
	ldrh r3, [r4]
	lsl r1, r3, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801BDEC @ =0xFFFFF00F
	and r0, r3
	orr r0, r1
	strh r0, [r4]
	ldr r1, _0801BDF8 @ =0x00004859
	add r0, r2, r1
	mov r1, #0
	strb r1, [r0]
	ldr r3, _0801BDFC @ =0x0000485A
	add r0, r2, r3
	strb r1, [r0]
	ldr r0, _0801BE00 @ =0x0000485B
	add r2, r2, r0
	strb r1, [r2]
_0801BDE6:
	mov r0, #0
	b _0801BE06
	.align 2, 0
_0801BDEC: .4byte 0xFFFFF00F
_0801BDF0: .4byte 0x03000040
_0801BDF4: .4byte 0x0000488A
_0801BDF8: .4byte 0x00004859
_0801BDFC: .4byte 0x0000485A
_0801BE00: .4byte 0x0000485B
_0801BE04:
	mov r0, #1
_0801BE06:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end Campaign_SelectOpponent

