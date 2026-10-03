	thumb_func_start AiCanBeatAnyMonster
AiCanBeatAnyMonster: @ 0x08057C50
	push {r4, r5, lr}
	add r5, r0, #0
	mov r0, #0
	bl CountMonsters
	cmp r0, #0
	bne _08057C62
_08057C5E:
	mov r0, #1
	b _08057C8A
_08057C62:
	mov r4, #0
_08057C64:
	mov r0, #0x94
	mul r0, r4
	ldr r1, _08057C90 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08057C82
	add r0, r5, #0
	add r1, r4, #0
	bl AiCanBeatMonster
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08057C5E
_08057C82:
	add r4, #1
	cmp r4, #4
	ble _08057C64
	mov r0, #0
_08057C8A:
	pop {r4, r5}
	pop {r1}
	bx r1
_08057C90: .4byte 0x0201930C
	thumb_func_end AiCanBeatAnyMonster

