	thumb_func_start sub_0807C924
sub_0807C924: @ 0x0807C924
	push {r4, r5, r6, r7, lr}
	ldr r7, _0807C9F0 @ =0x03000040
	mov r0, #1
	ldrh r1, [r7, #6]
	and r0, r1
	ldr r5, _0807C9F4 @ =0x0201F7B0
	cmp r0, #0
	beq _0807C944
	mov r0, #0xF
	ldrh r2, [r5, #0x12]
	and r0, r2
	mov r3, #0x96
	lsl r3, r3, #5
	add r1, r3, #0
	orr r0, r1
	strh r0, [r5, #0x12]
_0807C944:
	add r6, r5, #0
	ldrh r0, [r6, #0x12]
	lsr r1, r0, #4
	ldr r0, _0807C9F8 @ =0x0000012B
	cmp r1, r0
	bhi _0807CA14
	ldrb r2, [r6, #0x11]
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1A
	add r1, #1
	mov r0, #0x3F
	and r1, r0
	mov r0, #0x40
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r6, #0x11]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1A
	mov r4, #0x1F
	add r1, r4, #0
	and r1, r0
	cmp r1, #0x1F
	bne _0807C97A
	mov r0, #0x29
	bl sub_08077AEC
_0807C97A:
	ldrb r2, [r6, #0x11]
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1A
	add r0, r4, #0
	and r0, r1
	cmp r0, #0x1C
	bhi _0807C98C
	bl sub_0807BF68
_0807C98C:
	ldrh r3, [r6, #0x12]
	lsl r2, r3, #0x10
	lsr r0, r2, #0x14
	cmp r0, #0x1F
	bhi _0807C9DA
	ldr r1, _0807C9FC @ =0x08087E88
	lsl r0, r0, #2
	add r0, r0, r1
	ldr r1, [r0]
	ldr r3, _0807CA00 @ =0x0000442C
	add r0, r7, r3
	strh r1, [r0]
	ldr r0, _0807CA04 @ =0x001C0070
	ldr r4, _0807CA08 @ =0x08087F08
	lsr r1, r2, #0x14
	lsl r1, r1, #2
	add r1, r1, r4
	ldrh r2, [r1]
	mov r1, #0
	bl sub_080761F0
	ldr r0, _0807CA0C @ =0x004C0070
	ldrh r2, [r6, #0x12]
	lsr r1, r2, #4
	lsl r1, r1, #2
	add r1, r1, r4
	ldrh r2, [r1]
	mov r1, #0
	bl sub_080761F0
	ldr r0, _0807CA10 @ =0x007C0070
	ldrh r3, [r6, #0x12]
	lsr r1, r3, #4
	lsl r1, r1, #2
	add r1, r1, r4
	ldrh r2, [r1]
	mov r1, #0
	bl sub_080761F0
_0807C9DA:
	ldrh r2, [r6, #0x12]
	lsr r1, r2, #4
	add r1, #1
	lsl r1, r1, #4
	mov r0, #0xF
	and r0, r2
	orr r0, r1
	strh r0, [r6, #0x12]
_0807C9EA:
	mov r0, #0
	b _0807CA98
	.align 2, 0
_0807C9F0: .4byte 0x03000040
_0807C9F4: .4byte 0x0201F7B0
_0807C9F8: .4byte 0x0000012B
_0807C9FC: .4byte gUnk_08087E88
_0807CA00: .4byte 0x0000442C
_0807CA04: .4byte 0x001C0070
_0807CA08: .4byte gUnk_08087F08
_0807CA0C: .4byte 0x004C0070
_0807CA10: .4byte 0x007C0070
_0807CA14:
	ldr r0, _0807CA28 @ =0x0000485A
	add r4, r7, r0
	ldrb r0, [r4]
	cmp r0, #1
	beq _0807CA48
	cmp r0, #1
	bgt _0807CA2C
	cmp r0, #0
	beq _0807CA32
	b _0807CA96
_0807CA28: .4byte 0x0000485A
_0807CA2C:
	cmp r0, #2
	beq _0807CA84
	b _0807CA96
_0807CA32:
	bl sub_0807BF68
	mov r0, #3
	ldrh r7, [r7, #6]
	and r0, r7
	cmp r0, #0
	beq _0807C9EA
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0807C9EA
_0807CA48:
	bl sub_0807BF68
	mov r0, #4
	bl sub_08075A6C
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0807C9EA
	ldr r0, _0807CA80 @ =0x02011C20
	ldrh r2, [r5, #0x14]
	lsl r1, r2, #2
	add r1, r1, r0
	mov r0, #2
	ldrb r3, [r1, #0xA]
	orr r0, r3
	strb r0, [r1, #0xA]
	ldrh r0, [r5, #0x14]
	bl sub_08077498
	ldrh r0, [r5, #0x14]
	mov r1, #0
	mov r2, #0
	bl sub_0800688C
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0807C9EA
_0807CA80: .4byte 0x02011C20
_0807CA84:
	bl sub_08006D08
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0807C9EA
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0807C9EA
_0807CA96:
	mov r0, #1
_0807CA98:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0807C924
	.align 2, 0

