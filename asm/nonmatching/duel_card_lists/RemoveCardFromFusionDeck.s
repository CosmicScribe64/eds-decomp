	thumb_func_start RemoveCardFromFusionDeck
RemoveCardFromFusionDeck: @ 0x080080D8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	add r5, r0, #0
	add r7, r1, #0
	mov r4, #0
	ldr r2, _08008154 @ =0x020192E4
	mov r0, #1
	and r0, r5
	ldr r1, _08008158 @ =0x00000D64
	mul r0, r1
	add r1, r0, r2
	ldrb r3, [r1, #5]
	cmp r4, r3
	bge _0800816E
	add r6, r0, #0
	ldr r0, _0800815C @ =0x00000A44
	add r0, r0, r2
	mov ip, r0
	mov r8, r2
	add r2, r1, #0
	mov r3, #0
_08008106:
	mov r1, ip
	add r0, r6, r1
	add r0, r0, r3
	ldr r1, [r7]
	ldr r0, [r0]
	cmp r1, r0
	bne _08008164
	ldrb r0, [r2, #5]
	sub r0, #1
	strb r0, [r2, #5]
	add r6, r4, #0
	cmp r4, r0
	bge _0800814E
	mov r0, #1
	and r0, r5
	ldr r1, _08008158 @ =0x00000D64
	mul r0, r1
	ldr r1, _08008160 @ =0x02019D28
	mov r5, r8
	add r2, r0, r5
	add r5, r3, #4
	add r7, r0, r1
	lsl r0, r4, #2
	add r4, r0, r7
_08008136:
	add r1, r7, r5
	add r0, r4, #0
	str r2, [sp, #0]
	bl CopyDuelCard
	add r5, #4
	add r4, #4
	add r6, #1
	ldr r2, [sp, #0]
	ldrb r0, [r2, #5]
	cmp r6, r0
	blt _08008136
_0800814E:
	mov r0, #1
	b _08008170
	.align 2, 0
_08008154: .4byte 0x020192E4
_08008158: .4byte 0x00000D64
_0800815C: .4byte 0x00000A44
_08008160: .4byte 0x02019D28
_08008164:
	add r3, #4
	add r4, #1
	ldrb r1, [r2, #5]
	cmp r4, r1
	blt _08008106
_0800816E:
	mov r0, #0
_08008170:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end RemoveCardFromFusionDeck

