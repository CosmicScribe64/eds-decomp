	thumb_func_start DuelCmd_ClearZoneCardNoRedraw
DuelCmd_ClearZoneCardNoRedraw: @ 0x080119B0
	ldr r2, _080119E4 @ =0x020185C0
	mov r0, #7
	ldrh r1, [r2, #2]
	and r0, r1
	ldrh r1, [r2]
	lsr r3, r1, #0xF
	mov r1, #0x94
	mul r1, r0
	ldr r0, _080119E8 @ =0x00000D64
	mul r0, r3
	add r1, r1, r0
	ldr r0, _080119EC @ =0x0201930C
	add r1, r1, r0
	ldr r0, _080119F0 @ =0xFFFFF000
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	ldr r0, _080119F4 @ =0x0000080D
	add r2, r2, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	strb r0, [r2]
	bx lr
	.align 2, 0
_080119E4: .4byte 0x020185C0
_080119E8: .4byte 0x00000D64
_080119EC: .4byte 0x0201930C
_080119F0: .4byte 0xFFFFF000
_080119F4: .4byte 0x0000080D
	thumb_func_end DuelCmd_ClearZoneCardNoRedraw

