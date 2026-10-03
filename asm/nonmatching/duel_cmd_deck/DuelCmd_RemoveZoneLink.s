	thumb_func_start DuelCmd_RemoveZoneLink
DuelCmd_RemoveZoneLink: @ 0x0800EB10
	push {r4, lr}
	ldr r4, _0800EB3C @ =0x020185C0
	ldrh r0, [r4, #4]
	ldrh r1, [r4, #2]
	ldrh r2, [r4, #6]
	bl RemoveZoneLink
	bl DuelScreen_DrawCursorInfo
	bl DrawAllAreaTiles
	ldr r0, _0800EB40 @ =0x0000080D
	add r4, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0800EB3C: .4byte 0x020185C0
_0800EB40: .4byte 0x0000080D
	thumb_func_end DuelCmd_RemoveZoneLink

