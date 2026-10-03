	thumb_func_start DuelCmd_ExodiaWinScene
DuelCmd_ExodiaWinScene: @ 0x08017024
	push {r4, r5, lr}
	ldr r5, _0801703C @ =0x020185C0
	ldr r0, _08017040 @ =0x0000080A
	add r4, r5, r0
	ldrb r1, [r4]
	lsl r0, r1, #0x19
	lsr r0, r0, #0x19
	cmp r0, #0
	beq _08017044
	cmp r0, #1
	beq _08017064
	b _0801707A
_0801703C: .4byte 0x020185C0
_08017040: .4byte 0x0000080A
_08017044:
	mov r0, #1
	mov r1, #0
	bl DuelScene_Start
	ldrb r2, [r4]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r4]
	b _0801707A
_08017064:
	bl DuelScene_Run
	cmp r0, #0
	beq _0801707A
	ldr r2, _08017080 @ =0x0000080D
	add r1, r5, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0801707A:
	pop {r4, r5}
	pop {r0}
	bx r0
_08017080: .4byte 0x0000080D
	thumb_func_end DuelCmd_ExodiaWinScene

