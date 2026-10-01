	thumb_func_start sub_0803435C
sub_0803435C: @ 0x0803435C
	push {r4, r5, lr}
	sub sp, #8
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _080343FA
	ldr r0, _08034404 @ =0x000007FF
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08034408 @ =0x08622AB4
	add r0, r0, r1
	mov r1, #0xFC
	lsl r1, r1, #2
	ldrh r0, [r0]
	cmp r0, r1
	bne _0803439C
	ldr r5, _0803440C @ =0x00000402
	mov r0, #0
	add r1, r5, #0
	bl sub_080090C8
	cmp r0, #0
	bgt _080343FA
	mov r0, #1
	add r1, r5, #0
	bl sub_080090C8
	cmp r0, #0
	bgt _080343FA
_0803439C:
	mov r0, #7
	ldrb r1, [r4, #0xA]
	and r0, r1
	cmp r0, #2
	bne _080343FA
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08008A1C
	cmp r0, #0
	ble _080343FA
	ldrh r0, [r4, #0xE]
	lsl r1, r0, #0x10
	ldrh r0, [r4, #0xC]
	orr r1, r0
	str r1, [sp, #4]
	lsl r0, r1, #0x13
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	mov r2, sp
	bl sub_08009C08
	cmp r0, #0
	beq _080343FA
	mov r0, #1
	ldrb r1, [r4, #2]
	and r0, r1
	mov r3, #0xD3
	cmp r0, #0
	beq _080343DE
	ldr r3, _08034410 @ =0x000080D3
_080343DE:
	ldrh r1, [r4, #0xC]
	ldrh r2, [r4, #0xE]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	add r1, sp, #4
	mov r2, #1
	mov r3, #0x30
	bl sub_08056094
_080343FA:
	mov r0, #0
	add sp, #8
	pop {r4, r5}
	pop {r1}
	bx r1
_08034404: .4byte 0x000007FF
_08034408: .4byte gUnk_08622AB4
_0803440C: .4byte 0x00000402
_08034410: .4byte 0x000080D3
	thumb_func_end sub_0803435C

