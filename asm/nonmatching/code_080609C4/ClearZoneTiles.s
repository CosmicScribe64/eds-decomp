	thumb_func_start ClearZoneTiles
ClearZoneTiles: @ 0x08060FD0
	push {lr}
	ldr r3, _08061000 @ =0x081A42A4
	lsl r1, r1, #3
	lsl r0, r0, #7
	add r1, r1, r0
	add r0, r1, r3
	ldr r0, [r0]
	cmp r0, #0
	bge _08060FE4
	add r0, #7
_08060FE4:
	asr r2, r0, #3
	add r0, r3, #4
	add r0, r1, r0
	ldr r0, [r0]
	cmp r0, #0
	bge _08060FF2
	add r0, #7
_08060FF2:
	asr r1, r0, #3
	add r0, r2, #0
	bl ClearTileBlock4x4
	pop {r0}
	bx r0
	.align 2, 0
_08061000: .4byte gDuelZonePositions
	thumb_func_end ClearZoneTiles

