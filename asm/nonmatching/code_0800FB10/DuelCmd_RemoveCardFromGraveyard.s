	thumb_func_start DuelCmd_RemoveCardFromGraveyard
DuelCmd_RemoveCardFromGraveyard: @ 0x08010124
	push {r4, lr}
	sub sp, #4
	ldr r4, _08010158 @ =0x020185C0
	ldrh r1, [r4, #4]
	lsl r0, r1, #0x10
	ldrh r1, [r4, #2]
	orr r0, r1
	str r0, [sp, #0]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	mov r1, sp
	bl RemoveCardFromGraveyard
	bl DrawAllAreaTiles
	ldr r0, _0801015C @ =0x0000080D
	add r4, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0
_08010158: .4byte 0x020185C0
_0801015C: .4byte 0x0000080D
	thumb_func_end DuelCmd_RemoveCardFromGraveyard

