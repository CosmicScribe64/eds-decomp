	thumb_func_start sub_08007A4C
sub_08007A4C: @ 0x08007A4C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r9, r1
	mov sl, r2
	add r4, r3, #0
	ldr r1, [sp, #0x20]
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r8, r1
	mov r1, #1
	and r0, r1
	ldr r1, _08007B18 @ =0x00000D64
	add r7, r0, #0
	mul r7, r1
	ldr r6, _08007B1C @ =0x0201930C
	add r1, r7, r6
	mov r0, #0x94
	mov r2, r9
	mul r2, r0
	add r0, r2, #0
	add r5, r1, r0
	add r0, r5, #0
	mov r1, #0x94
	bl sub_08075278
	add r0, r5, #0
	mov r1, sl
	bl sub_08007558
	add r2, r6, #0
	sub r2, #0x2C
	ldrh r1, [r2]
	add r0, r1, #1
	strh r0, [r2]
	strh r1, [r5, #4]
	mov r2, #1
	add r1, r4, #0
	and r1, r2
	mov r0, #2
	neg r0, r0
	ldrb r3, [r5, #6]
	and r0, r3
	orr r0, r1
	mov r1, r8
	and r1, r2
	lsl r1, r1, #1
	mov r2, #3
	neg r2, r2
	and r0, r2
	orr r0, r1
	strb r0, [r5, #6]
	mov r0, #0x20
	ldrb r1, [r5, #7]
	orr r0, r1
	mov r1, #4
	orr r0, r1
	strb r0, [r5, #7]
	cmp r4, #0
	bne _08007AE6
	mov r2, r8
	cmp r2, #0
	beq _08007AE6
	add r0, r6, #0
	sub r0, #0x28
	add r0, r7, r0
	mov r1, #1
	mov r3, r9
	lsl r1, r3
	ldrh r2, [r0, #0x26]
	bic r2, r1
	add r1, r2, #0
	strh r1, [r0, #0x26]
_08007AE6:
	mov r3, sl
	ldr r0, [r3]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08007B20 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	bl sub_0800756C
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08007B0A
	add r1, r5, #0
	add r1, #0x8C
	mov r0, #0x10
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_08007B0A:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08007B18: .4byte 0x00000D64
_08007B1C: .4byte 0x0201930C
_08007B20: .4byte gUnk_08622AB4
	thumb_func_end sub_08007A4C

