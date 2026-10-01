	thumb_func_start sub_08039E30
sub_08039E30: @ 0x08039E30
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	bne _08039EC8
	mov r4, #7
	ldrb r3, [r5, #0xA]
	and r4, r3
	cmp r4, #1
	bne _08039EC8
	ldrb r6, [r5, #0xC]
	ldrh r0, [r5, #0xC]
	lsr r7, r0, #8
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08008A44
	mov r8, r0
	ldrb r3, [r5, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	cmp r6, r0
	beq _08039EC8
	and r4, r6
	mov r0, #0x94
	add r1, r7, #0
	mul r1, r0
	ldr r0, _08039ED4 @ =0x00000D64
	mul r0, r4
	add r1, r1, r0
	ldr r0, _08039ED8 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08039EC8
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08039EC8
	mov r0, #1
	neg r0, r0
	cmp r8, r0
	beq _08039EC8
	add r0, r6, #0
	add r1, r7, #0
	bl sub_0800C8BC
	cmp r0, #7
	bne _08039EC8
	ldrb r0, [r5, #2]
	lsl r2, r0, #0x1F
	lsr r0, r2, #0x1F
	ldrh r1, [r5, #0xC]
	add r2, r0, #0
	mov r3, r8
	lsl r4, r3, #0x18
	lsr r4, r4, #0x10
	orr r2, r4
	bl sub_08019078
	ldrb r0, [r5, #2]
	lsl r2, r0, #0x1F
	lsr r0, r2, #0x1F
	ldrh r1, [r5]
	add r2, r0, #0
	orr r2, r4
	mov r3, #3
	bl sub_08017AB4
_08039EC8:
	mov r0, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08039ED4: .4byte 0x00000D64
_08039ED8: .4byte 0x0201930C
	thumb_func_end sub_08039E30

