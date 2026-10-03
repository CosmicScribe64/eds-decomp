	thumb_func_start TextCanvasToTiles
TextCanvasToTiles: @ 0x08075114
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	add r3, r0, #0
	lsl r1, r1, #0x10
	mov r0, #0xF0
	lsl r0, r0, #0xC
	and r0, r1
	lsr r4, r0, #0x10
	lsl r0, r4, #4
	orr r4, r0
	lsl r0, r4, #0x10
	lsr r4, r0, #0x10
	lsl r0, r4, #8
	orr r4, r0
	lsl r0, r4, #0x10
	lsr r4, r0, #0x10
	mov r0, #0
	str r0, [sp, #4]
	ldr r2, _0807520C @ =0x02000000
	mov r5, #0x80
	lsl r5, r5, #9
	add r1, r2, r5
	ldr r6, _08075210 @ =0x00010001
	add r0, r2, r6
	ldrb r0, [r0]
	ldrb r1, [r1]
	mul r0, r1
	mov sl, r2
	ldr r7, [sp, #4]
	cmp r7, r0
	bge _080751FA
	ldr r0, _08075214 @ =0x0000FFF0
	and r0, r4
	str r0, [sp, #8]
	ldr r1, _08075218 @ =0x0000FF0F
	mov r9, r1
	ldr r5, _0807521C @ =0x0000F0FF
	mov r8, r5
	ldr r6, _08075220 @ =0x00000FFF
	mov ip, r6
_0807516C:
	ldr r7, [sp, #4]
	lsl r0, r7, #6
	add r2, r0, r2
	add r0, r7, #0
	add r0, #1
	str r0, [sp, #0]
	mov r5, #0xF
_0807517A:
	strh r4, [r3]
	ldrb r0, [r2]
	cmp r0, #0
	beq _08075190
	mov r1, sp
	ldrh r1, [r1, #8]
	strh r1, [r3]
	ldr r0, [sp, #8]
	ldrb r6, [r2]
	orr r0, r6
	strh r0, [r3]
_08075190:
	ldrb r0, [r2, #1]
	cmp r0, #0
	beq _080751A6
	mov r0, r9
	ldrh r7, [r3]
	and r0, r7
	strh r0, [r3]
	ldrb r6, [r2, #1]
	lsl r1, r6, #4
	orr r0, r1
	strh r0, [r3]
_080751A6:
	ldrb r0, [r2, #2]
	cmp r0, #0
	beq _080751BC
	mov r0, r8
	ldrh r7, [r3]
	and r0, r7
	strh r0, [r3]
	ldrb r6, [r2, #2]
	lsl r1, r6, #8
	orr r0, r1
	strh r0, [r3]
_080751BC:
	ldrb r0, [r2, #3]
	cmp r0, #0
	beq _080751D2
	mov r0, ip
	ldrh r7, [r3]
	and r0, r7
	strh r0, [r3]
	ldrb r6, [r2, #3]
	lsl r1, r6, #0xC
	orr r0, r1
	strh r0, [r3]
_080751D2:
	add r3, #2
	add r2, #4
	sub r5, #1
	cmp r5, #0
	bge _0807517A
	ldr r7, [sp, #0]
	str r7, [sp, #4]
	mov r2, sl
	mov r1, #0x80
	lsl r1, r1, #9
	add r0, r2, r1
	ldr r5, _08075224 @ =0x02010001
	ldrb r5, [r5]
	ldrb r7, [r0]
	add r6, r5, #0
	mul r6, r7
	add r0, r6, #0
	ldr r1, [sp, #4]
	cmp r1, r0
	blt _0807516C
_080751FA:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0807520C: .4byte 0x02000000
_08075210: .4byte 0x00010001
_08075214: .4byte 0x0000FFF0
_08075218: .4byte 0x0000FF0F
_0807521C: .4byte 0x0000F0FF
_08075220: .4byte 0x00000FFF
_08075224: .4byte 0x02010001
	thumb_func_end TextCanvasToTiles

