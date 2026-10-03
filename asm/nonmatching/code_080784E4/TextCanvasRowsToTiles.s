	thumb_func_start TextCanvasRowsToTiles
TextCanvasRowsToTiles: @ 0x08078CC8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	add r4, r0, #0
	lsl r1, r1, #0x10
	lsl r2, r2, #0x18
	lsr r5, r2, #0x18
	mov r0, #0xF0
	lsl r0, r0, #0xC
	and r0, r1
	lsr r2, r0, #0x10
	lsl r0, r2, #4
	orr r2, r0
	lsl r0, r2, #0x10
	lsr r2, r0, #0x10
	lsl r0, r2, #8
	orr r2, r0
	lsl r0, r2, #0x10
	lsr r2, r0, #0x10
	ldr r6, _08078DB0 @ =0x02000000
	mov r1, #0x80
	lsl r1, r1, #9
	add r0, r6, r1
	ldrb r3, [r0]
	lsl r0, r5, #4
	mul r0, r3
	lsl r0, r0, #1
	add r4, r4, r0
	lsl r0, r3, #1
	mov r1, #0
	cmn r0, r3
	beq _08078DA0
	str r3, [sp, #4]
	add r7, r3, #0
	mul r7, r5
	str r7, [sp, #0]
	ldr r6, _08078DB4 @ =0x0000FFF0
	and r6, r2
	ldr r0, _08078DB8 @ =0x0000FF0F
	mov sl, r0
	ldr r3, _08078DBC @ =0x0000F0FF
	mov r9, r3
	ldr r5, _08078DC0 @ =0x00000FFF
	mov r8, r5
_08078D26:
	ldr r7, [sp, #0]
	add r0, r1, r7
	lsl r0, r0, #6
	ldr r5, _08078DB0 @ =0x02000000
	add r3, r0, r5
	add r1, #1
	mov ip, r1
	mov r5, #0xF
_08078D36:
	strh r2, [r4]
	ldrb r0, [r3]
	cmp r0, #0
	beq _08078D48
	strh r6, [r4]
	add r0, r6, #0
	ldrb r7, [r3]
	orr r0, r7
	strh r0, [r4]
_08078D48:
	ldrb r0, [r3, #1]
	cmp r0, #0
	beq _08078D5E
	mov r0, sl
	ldrh r1, [r4]
	and r0, r1
	strh r0, [r4]
	ldrb r7, [r3, #1]
	lsl r1, r7, #4
	orr r0, r1
	strh r0, [r4]
_08078D5E:
	ldrb r0, [r3, #2]
	cmp r0, #0
	beq _08078D74
	mov r0, r9
	ldrh r1, [r4]
	and r0, r1
	strh r0, [r4]
	ldrb r7, [r3, #2]
	lsl r1, r7, #8
	orr r0, r1
	strh r0, [r4]
_08078D74:
	ldrb r0, [r3, #3]
	cmp r0, #0
	beq _08078D8A
	mov r0, r8
	ldrh r1, [r4]
	and r0, r1
	strh r0, [r4]
	ldrb r7, [r3, #3]
	lsl r1, r7, #0xC
	orr r0, r1
	strh r0, [r4]
_08078D8A:
	add r4, #2
	add r3, #4
	sub r5, #1
	cmp r5, #0
	bge _08078D36
	mov r1, ip
	ldr r3, [sp, #4]
	lsl r0, r3, #1
	add r0, r0, r3
	cmp r1, r0
	blt _08078D26
_08078DA0:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08078DB0: .4byte 0x02000000
_08078DB4: .4byte 0x0000FFF0
_08078DB8: .4byte 0x0000FF0F
_08078DBC: .4byte 0x0000F0FF
_08078DC0: .4byte 0x00000FFF
	thumb_func_end TextCanvasRowsToTiles

