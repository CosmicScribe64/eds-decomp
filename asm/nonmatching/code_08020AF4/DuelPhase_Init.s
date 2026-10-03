	thumb_func_start DuelPhase_Init
DuelPhase_Init: @ 0x08021834
	push {r4, lr}
	ldr r3, _08021888 @ =0x020192E0
	ldr r0, _0802188C @ =0x03000040
	ldr r1, _08021890 @ =0x00004870
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1F
	ldr r2, _08021894 @ =0x00001B12
	add r1, r3, r2
	lsr r0, r0, #0x1F
	lsl r0, r0, #1
	mov r2, #3
	neg r2, r2
	ldrb r4, [r1]
	and r2, r4
	orr r2, r0
	strb r2, [r1]
	ldr r4, _08021898 @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r4, #1]
	and r0, r1
	cmp r0, #0
	beq _080218A4
	mov r0, #2
	and r2, r0
	cmp r2, #0
	beq _080218A4
	ldr r2, _0802189C @ =0x00001B10
	add r1, r3, r2
	ldrh r0, [r1]
	add r0, #1
	strh r0, [r1]
	ldr r0, _080218A0 @ =0x0000F001
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelLink_SendMessage
	mov r0, #8
	strb r0, [r4]
	mov r0, #0
	b _080218A6
_08021888: .4byte 0x020192E0
_0802188C: .4byte 0x03000040
_08021890: .4byte 0x00004870
_08021894: .4byte 0x00001B12
_08021898: .4byte 0x02015EE8
_0802189C: .4byte 0x00001B10
_080218A0: .4byte 0x0000F001
_080218A4:
	mov r0, #1
_080218A6:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end DuelPhase_Init

