	thumb_func_start DiceScreen_AddOamPiece
DiceScreen_AddOamPiece: @ 0x08025374
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	add r0, r1, #0
	add r5, r2, #0
	add r4, r3, #0
	ldr r2, [sp, #0x1C]
	ldr r6, [sp, #0x20]
	ldr r1, [sp, #0x24]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsl r5, r5, #0x10
	lsr r5, r5, #0x10
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r9, r2
	lsl r6, r6, #0x10
	lsr r6, r6, #0x10
	bl OamListAlloc
	add r7, r0, #0
	mov ip, r7
	mov r0, r8
	ldrh r2, [r0]
	mov r1, #0xFF
	lsl r1, r1, #8
	and r1, r2
	mov r3, #0xFF
	mov r0, #0xFF
	and r0, r2
	add r4, r4, r0
	and r4, r3
	orr r1, r4
	strh r1, [r7]
	mov r1, r8
	ldrh r3, [r1, #2]
	mov r1, #0xFE
	lsl r1, r1, #8
	and r1, r3
	ldr r2, _08025428 @ =0x000001FF
	add r0, r2, #0
	and r0, r3
	add r5, r5, r0
	and r5, r2
	orr r1, r5
	strh r1, [r7, #2]
	mov r2, r8
	ldrh r1, [r2, #4]
	ldr r2, _0802542C @ =0x0000FC0F
	and r2, r1
	mov r0, #0xF0
	and r0, r1
	lsl r0, r0, #1
	add r6, r6, r0
	orr r2, r6
	strh r2, [r7, #4]
	mov r0, #0x80
	lsl r0, r0, #1
	mov r3, r8
	ldrh r3, [r3, #4]
	and r0, r3
	cmp r0, #0
	beq _08025400
	add r0, r2, #0
	add r0, #0x10
	strh r0, [r7, #4]
_08025400:
	mov r0, #0xF0
	lsl r0, r0, #8
	mov r1, r8
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08025418
	mov r2, r9
	lsl r0, r2, #0xC
	ldrh r3, [r7, #4]
	orr r0, r3
	strh r0, [r7, #4]
_08025418:
	mov r0, ip
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08025428: .4byte 0x000001FF
_0802542C: .4byte 0x0000FC0F
	thumb_func_end DiceScreen_AddOamPiece

