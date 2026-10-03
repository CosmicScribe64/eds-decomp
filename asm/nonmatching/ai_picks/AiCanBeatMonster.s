	thumb_func_start AiCanBeatMonster
AiCanBeatMonster: @ 0x08057BAC
	push {r4, r5, r6, lr}
	add r6, r0, #0
	add r5, r1, #0
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _08057BFC @ =0x0201930C
	add r4, r1, r0
	mov r0, #1
	ldrb r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	beq _08057C08
	mov r0, #0
	add r1, r5, #0
	bl GetZoneCardDef
	add r1, r0, #0
	mov r0, #2
	ldrb r4, [r4, #6]
	and r0, r4
	cmp r0, #0
	bne _08057C12
	ldr r0, _08057C00 @ =0x03000040
	ldr r2, _08057C04 @ =0x00004870
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r2, r0, #0x1A
	lsr r0, r2, #0x1B
	cmp r0, #0xA
	bhi _08057C12
	add r0, #5
	mul r1, r0
	add r0, r1, #0
	cmp r1, #0
	bge _08057BF6
	add r0, #0xF
_08057BF6:
	asr r1, r0, #4
	b _08057C12
	.align 2, 0
_08057BFC: .4byte 0x0201930C
_08057C00: .4byte 0x03000040
_08057C04: .4byte 0x00004870
_08057C08:
	mov r0, #0
	add r1, r5, #0
	bl GetZoneCardAtk
	add r1, r0, #0
_08057C12:
	cmp r1, r6
	blt _08057C40
	cmp r1, r6
	bne _08057C48
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _08057C44 @ =0x0201930C
	add r1, r1, r0
	mov r0, #1
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08057C48
	mov r0, #1
	bl CountMonsters
	add r4, r0, #0
	mov r0, #0
	bl CountMonsters
	cmp r4, r0
	ble _08057C48
_08057C40:
	mov r0, #1
	b _08057C4A
_08057C44: .4byte 0x0201930C
_08057C48:
	mov r0, #0
_08057C4A:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end AiCanBeatMonster

