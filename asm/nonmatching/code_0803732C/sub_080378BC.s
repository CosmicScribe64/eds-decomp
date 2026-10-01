	thumb_func_start sub_080378BC
sub_080378BC: @ 0x080378BC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r9, r0
	mov r0, #4
	mov r1, r9
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _080378D6
	b _08037A0A
_080378D6:
	ldr r0, _080378F0 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08037984
	cmp r0, #0x7F
	bgt _080378F4
	cmp r0, #0x7E
	beq _080379C0
	b _08037A0A
	.align 2, 0
_080378F0: .4byte 0x02017A40
_080378F4:
	cmp r0, #0x80
	beq _080378FA
	b _08037A0A
_080378FA:
	mov r5, #0
	mov r3, #0xC
	add r3, r9
	mov r8, r3
_08037902:
	lsl r2, r5, #1
	mov r0, r8
	add r1, r0, r2
	mov r0, #0
	strh r0, [r1]
	mov r4, #0
	add r7, r2, #0
	add r1, r5, #1
	mov sl, r1
	add r0, r5, #0
	mov r2, #1
	and r0, r2
	ldr r3, _08037970 @ =0x00000D64
	add r6, r0, #0
	mul r6, r3
_08037920:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _08037974 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08037950
	mov r0, #0x80
	cmp r5, #0
	beq _0803793A
	ldr r0, _08037978 @ =0x00008080
_0803793A:
	lsl r1, r4, #0x10
	lsr r1, r1, #0x10
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r1, r8
	add r0, r1, r7
	ldrh r1, [r0]
	add r1, #1
	strh r1, [r0]
_08037950:
	add r4, #1
	cmp r4, #4
	ble _08037920
	mov r5, sl
	cmp r5, #1
	ble _08037902
	ldr r1, _0803797C @ =0x02017A40
	mov r2, r9
	ldrb r2, [r2, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldr r3, _08037980 @ =0x000003E1
	add r1, r1, r3
	strb r0, [r1]
	mov r0, #0x7F
	b _08037A0C
_08037970: .4byte 0x00000D64
_08037974: .4byte 0x0201930C
_08037978: .4byte 0x00008080
_0803797C: .4byte 0x02017A40
_08037980: .4byte 0x000003E1
_08037984:
	mov r0, r9
	ldrb r0, [r0, #2]
	lsl r1, r0, #0x1F
	lsr r0, r1, #0x1F
	lsl r0, r0, #1
	mov r4, r9
	add r4, #0xC
	add r0, r4, r0
	ldrh r0, [r0]
	cmp r0, #0
	beq _08037A02
	lsr r0, r1, #0x1F
	bl sub_08022834
	cmp r0, #0
	beq _08037A02
	mov r1, r9
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r1, r0, #0x1F
	lsl r1, r1, #1
	add r1, r4, r1
	lsr r0, r0, #0x1F
	lsl r0, r0, #1
	add r0, r4, r0
	ldrh r0, [r0]
	sub r0, #1
	strh r0, [r1]
	mov r0, #0x7F
	b _08037A0C
_080379C0:
	mov r2, r9
	ldrb r2, [r2, #2]
	lsl r1, r2, #0x1F
	lsr r0, r1, #0x1F
	mov r5, #1
	sub r0, r5, r0
	lsl r0, r0, #1
	mov r4, r9
	add r4, #0xC
	add r0, r4, r0
	ldrh r0, [r0]
	cmp r0, #0
	beq _08037A06
	lsr r0, r1, #0x1F
	sub r0, r5, r0
	bl sub_08022834
	cmp r0, #0
	beq _08037A06
	mov r3, r9
	ldrb r3, [r3, #2]
	lsl r0, r3, #0x1F
	lsr r1, r0, #0x1F
	sub r1, r5, r1
	lsl r1, r1, #1
	add r1, r4, r1
	lsr r0, r0, #0x1F
	sub r0, r5, r0
	lsl r0, r0, #1
	add r0, r4, r0
	ldrh r0, [r0]
	sub r0, #1
	strh r0, [r1]
_08037A02:
	mov r0, #0x7E
	b _08037A0C
_08037A06:
	mov r0, #0x7D
	b _08037A0C
_08037A0A:
	mov r0, #0
_08037A0C:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_080378BC
	.align 2, 0

