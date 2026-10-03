	thumb_func_start GetGraveyardCardById
GetGraveyardCardById: @ 0x08009C08
	push {r4, r5, r6, r7, lr}
	add r6, r2, #0
	lsl r1, r1, #0x10
	lsr r5, r1, #0x10
	mov r3, #0
	ldr r4, _08009C48 @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _08009C4C @ =0x00000D64
	mul r1, r0
	add r0, r1, r4
	ldrb r2, [r0, #4]
	cmp r3, r2
	bge _08009C5C
	ldr r2, _08009C50 @ =0x00000904
	add r7, r4, r2
	add r4, r1, #0
	add r2, r0, #0
_08009C2C:
	add r1, r4, r7
	lsl r0, r3, #2
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, r5
	bne _08009C54
	add r0, r6, #0
	bl CopyDuelCard
	mov r0, #1
	b _08009C5E
	.align 2, 0
_08009C48: .4byte 0x020192E4
_08009C4C: .4byte 0x00000D64
_08009C50: .4byte 0x00000904
_08009C54:
	add r3, #1
	ldrb r0, [r2, #4]
	cmp r3, r0
	blt _08009C2C
_08009C5C:
	mov r0, #0
_08009C5E:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end GetGraveyardCardById

