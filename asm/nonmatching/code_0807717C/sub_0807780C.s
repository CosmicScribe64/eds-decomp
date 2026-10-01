	thumb_func_start sub_0807780C
sub_0807780C: @ 0x0807780C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r6, #0
	ldr r0, _08077874 @ =0x02011C20
	ldr r2, _08077878 @ =0x000020C8
	add r1, r0, r2
	mov sl, r0
	ldrh r0, [r1]
	cmp r6, r0
	beq _080778AA
	mov r9, sl
_0807782A:
	lsl r1, r6, #1
	ldr r0, _0807787C @ =0x00002008
	add r0, r9
	add r3, r1, r0
	ldrh r0, [r3]
	lsl r1, r0, #2
	add r1, r9
	ldrb r2, [r1, #9]
	lsl r1, r2, #0x1C
	lsr r5, r1, #0x1E
	lsl r2, r2, #0x1A
	lsr r4, r2, #0x1E
	str r3, [sp, #0]
	bl sub_0807717C
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	add r0, r5, r4
	add r6, #1
	mov r8, r6
	ldr r3, [sp, #0]
	cmp r0, r7
	ble _0807789C
	add r6, r3, #0
_0807785A:
	cmp r4, #0
	beq _08077880
	ldrh r0, [r6]
	bl sub_080776F8
	ldrh r0, [r6]
	bl sub_08077498
	sub r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	b _08077896
	.align 2, 0
_08077874: .4byte 0x02011C20
_08077878: .4byte 0x000020C8
_0807787C: .4byte 0x00002008
_08077880:
	cmp r5, #0
	beq _08077896
	ldrh r0, [r6]
	bl sub_0807766C
	ldrh r0, [r6]
	bl sub_08077498
	sub r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
_08077896:
	add r0, r5, r4
	cmp r0, r7
	bgt _0807785A
_0807789C:
	mov r1, r8
	lsl r0, r1, #0x10
	lsr r6, r0, #0x10
	ldr r2, _08077904 @ =0x02013CE8
	ldrh r2, [r2]
	cmp r6, r2
	bne _0807782A
_080778AA:
	mov r6, #0
	ldr r0, _08077908 @ =0x000020CA
	add r0, sl
	ldrh r1, [r0]
	cmp r6, r1
	beq _08077936
	mov r9, sl
	mov sl, r0
_080778BA:
	lsl r1, r6, #1
	mov r0, #0x82
	lsl r0, r0, #6
	add r0, r9
	add r3, r1, r0
	ldrh r0, [r3]
	lsl r1, r0, #2
	add r1, r9
	ldrb r2, [r1, #9]
	lsl r1, r2, #0x1C
	lsr r5, r1, #0x1E
	lsl r2, r2, #0x1A
	lsr r4, r2, #0x1E
	str r3, [sp, #0]
	bl sub_0807717C
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	add r0, r5, r4
	add r6, #1
	mov r8, r6
	ldr r3, [sp, #0]
	cmp r0, r7
	ble _08077928
	add r6, r3, #0
_080778EC:
	cmp r4, #0
	beq _0807790C
	ldrh r0, [r6]
	bl sub_080776F8
	ldrh r0, [r6]
	bl sub_08077498
	sub r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	b _08077922
_08077904: .4byte 0x02013CE8
_08077908: .4byte 0x000020CA
_0807790C:
	cmp r5, #0
	beq _08077922
	ldrh r0, [r6]
	bl sub_0807766C
	ldrh r0, [r6]
	bl sub_08077498
	sub r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
_08077922:
	add r0, r5, r4
	cmp r0, r7
	bgt _080778EC
_08077928:
	mov r2, r8
	lsl r0, r2, #0x10
	lsr r6, r0, #0x10
	mov r0, sl
	ldrh r0, [r0]
	cmp r6, r0
	bne _080778BA
_08077936:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_0807780C
	.align 2, 0

