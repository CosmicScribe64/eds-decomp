	thumb_func_start sub_08038320
sub_08038320: @ 0x08038320
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _080383D4
	mov r3, #7
	ldrb r5, [r6, #0xA]
	and r3, r5
	cmp r3, #2
	bne _080383D4
	ldrb r7, [r6, #0xC]
	ldrh r0, [r6, #0xC]
	lsr r0, r0, #8
	mov sl, r0
	ldrb r2, [r6, #0xE]
	ldrh r1, [r6, #0xE]
	lsr r4, r1, #8
	mov r5, #1
	mov r9, r5
	add r0, r7, #0
	and r0, r5
	mov r1, #0x94
	mov r8, r1
	add r5, r1, #0
	mov r1, sl
	mul r1, r5
	ldr r5, _080383E4 @ =0x00000D64
	mov ip, r5
	mov r5, ip
	mul r5, r0
	add r0, r5, #0
	add r1, r1, r0
	ldr r5, _080383E8 @ =0x0201930C
	add r1, r1, r5
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080383D4
	add r0, r3, #0
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080383D4
	mov r0, r9
	and r2, r0
	mov r0, r8
	mul r0, r4
	mov r1, ip
	mul r1, r2
	add r0, r0, r1
	add r1, r0, r5
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080383D4
	ldrb r1, [r1, #6]
	and r3, r1
	cmp r3, #0
	beq _080383D4
	mov r0, #0x98
	cmp r7, #0
	beq _080383AA
	ldr r0, _080383EC @ =0x00008098
_080383AA:
	mov r5, sl
	add r1, r5, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	ldrb r1, [r6, #2]
	lsl r4, r1, #0x1F
	lsr r4, r4, #0x1F
	add r0, r7, #0
	add r1, r5, #0
	bl sub_0800C894
	bl sub_0807548C
	add r1, r0, #0
	ldrh r2, [r6, #0xE]
	add r0, r4, #0
	mov r3, #4
	bl sub_08017AB4
_080383D4:
	mov r0, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_080383E4: .4byte 0x00000D64
_080383E8: .4byte 0x0201930C
_080383EC: .4byte 0x00008098
	thumb_func_end sub_08038320

