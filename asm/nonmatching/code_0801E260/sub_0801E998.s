	thumb_func_start sub_0801E998
sub_0801E998: @ 0x0801E998
	push {r4, lr}
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldr r3, _0801EA00 @ =0x02017A30
	mov r2, #0
	strb r2, [r3, #0xA]
	strb r2, [r3, #0xB]
	strb r2, [r3, #0xC]
	strb r2, [r3, #0xD]
	lsl r1, r1, #7
	mov r2, #0x7F
	ldrb r4, [r3, #5]
	and r2, r4
	orr r2, r1
	strb r2, [r3, #5]
	ldr r2, _0801EA04 @ =0x00007FFF
	and r2, r0
	ldr r1, _0801EA08 @ =0xFFFF8000
	ldrh r4, [r3, #4]
	and r1, r4
	orr r1, r2
	strh r1, [r3, #4]
	ldr r2, _0801EA0C @ =0x08198EF8
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r0, [r0]
	str r0, [r3]
	lsl r1, r1, #0x11
	lsr r1, r1, #0x11
	cmp r1, #1
	beq _0801E9DA
	cmp r1, #5
	bne _0801E9EE
_0801E9DA:
	bl sub_08077BA0
	ldr r1, _0801EA10 @ =0x020192E0
	ldr r0, _0801EA14 @ =0x00001B12
	add r1, r1, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0801E9EE:
	ldr r1, _0801EA18 @ =0x0201CFB0
	mov r0, #3
	neg r0, r0
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
_0801EA00: .4byte 0x02017A30
_0801EA04: .4byte 0x00007FFF
_0801EA08: .4byte 0xFFFF8000
_0801EA0C: .4byte gUnk_08198EF8
_0801EA10: .4byte 0x020192E0
_0801EA14: .4byte 0x00001B12
_0801EA18: .4byte 0x0201CFB0
	thumb_func_end sub_0801E998

