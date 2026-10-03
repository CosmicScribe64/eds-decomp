	thumb_func_start DuelCmd_SetPositionLocked
DuelCmd_SetPositionLocked: @ 0x080128A4
	push {r4, lr}
	ldr r2, _080128E8 @ =0x020185C0
	ldrh r0, [r2]
	lsr r1, r0, #0xF
	mov r0, #0x94
	ldrh r4, [r2, #2]
	add r3, r4, #0
	mul r3, r0
	ldr r0, _080128EC @ =0x00000D64
	mul r0, r1
	add r3, r3, r0
	ldr r0, _080128F0 @ =0x0201930C
	add r3, r3, r0
	mov r1, #1
	ldrh r0, [r2, #4]
	and r1, r0
	lsl r1, r1, #2
	mov r0, #5
	neg r0, r0
	ldrb r4, [r3, #7]
	and r0, r4
	orr r0, r1
	strb r0, [r3, #7]
	ldr r0, _080128F4 @ =0x0000080D
	add r2, r2, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	strb r0, [r2]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080128E8: .4byte 0x020185C0
_080128EC: .4byte 0x00000D64
_080128F0: .4byte 0x0201930C
_080128F4: .4byte 0x0000080D
	thumb_func_end DuelCmd_SetPositionLocked

