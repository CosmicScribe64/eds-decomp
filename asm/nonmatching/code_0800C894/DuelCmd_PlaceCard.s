	thumb_func_start DuelCmd_PlaceCard
DuelCmd_PlaceCard: @ 0x0800D784
	push {r4, r5, r6, lr}
	sub sp, #8
	ldr r4, _0800D7CC @ =0x020185C0
	ldrh r1, [r4]
	lsr r0, r1, #0xF
	ldrb r1, [r4, #2]
	ldrh r2, [r4, #2]
	lsr r3, r2, #8
	mov r5, #1
	and r5, r3
	mov r2, #2
	and r3, r2
	lsl r3, r3, #0x18
	lsr r3, r3, #0x19
	ldrh r6, [r4, #6]
	lsl r2, r6, #0x10
	ldrh r6, [r4, #4]
	orr r2, r6
	str r2, [sp, #4]
	str r5, [sp, #0]
	add r2, sp, #4
	bl PlaceMonsterCard
	bl DrawAllAreaTiles
	ldr r0, _0800D7D0 @ =0x0000080D
	add r4, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	add sp, #8
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_0800D7CC: .4byte 0x020185C0
_0800D7D0: .4byte 0x0000080D
	thumb_func_end DuelCmd_PlaceCard

