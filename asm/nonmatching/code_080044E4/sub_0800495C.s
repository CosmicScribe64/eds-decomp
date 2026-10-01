	thumb_func_start sub_0800495C
sub_0800495C: @ 0x0800495C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x80
	mov sl, r0
	ldr r6, _080049F8 @ =0x08198744
	mov r2, #0
_0800496E:
	ldr r1, [r6]
	ldrh r3, [r6, #4]
	lsl r0, r3, #0x16
	mov r5, #0
	mov r4, #8
	add r4, r4, r6
	mov r9, r4
	add r2, #1
	mov r8, r2
	cmp r0, #0
	beq _0800499C
	add r2, r1, #0
	mov r1, sp
_08004988:
	ldrh r0, [r2]
	strh r0, [r1]
	add r2, #2
	add r1, #2
	add r5, #1
	ldrh r3, [r6, #4]
	lsl r0, r3, #0x16
	lsr r0, r0, #0x16
	cmp r5, r0
	blt _08004988
_0800499C:
	ldrh r4, [r6, #4]
	lsl r0, r4, #0x16
	mov r5, #0
	cmp r0, #0
	beq _080049E2
_080049A6:
	bl sub_08076F9C
	ldrh r2, [r6, #4]
	lsl r1, r2, #0x16
	lsr r1, r1, #0x16
	bl __modsi3
	add r4, r0, #0
	bl sub_08076F9C
	ldrh r3, [r6, #4]
	lsl r1, r3, #0x16
	lsr r1, r1, #0x16
	bl __modsi3
	lsl r4, r4, #1
	mov r1, sp
	add r2, r1, r4
	ldrh r3, [r2]
	lsl r0, r0, #1
	add r1, r1, r0
	ldrh r0, [r1]
	strh r0, [r2]
	strh r3, [r1]
	add r5, #1
	ldrh r2, [r6, #4]
	lsl r0, r2, #0x16
	lsr r0, r0, #0x14
	cmp r5, r0
	blt _080049A6
_080049E2:
	mov r0, sl
	mov r1, #3
	bl __modsi3
	cmp r0, #1
	beq _08004A08
	cmp r0, #1
	bgt _080049FC
	cmp r0, #0
	beq _08004A02
	b _08004A14
_080049F8: .4byte gUnk_08198744
_080049FC:
	cmp r0, #2
	beq _08004A0E
	b _08004A14
_08004A02:
	ldrb r3, [r6, #5]
	lsl r0, r3, #0x19
	b _08004A12
_08004A08:
	ldr r0, [r6, #4]
	lsl r0, r0, #0xC
	b _08004A12
_08004A0E:
	ldrh r4, [r6, #6]
	lsl r0, r4, #0x17
_08004A12:
	lsr r7, r0, #0x1B
_08004A14:
	mov r5, #0
	cmp r5, r7
	bge _08004A9A
_08004A1A:
	ldrh r0, [r6, #4]
	lsl r1, r0, #0x16
	lsr r1, r1, #0x16
	add r0, r5, #0
	bl __modsi3
	lsl r0, r0, #1
	add r0, sp
	ldrh r2, [r0]
	ldr r0, _08004A38 @ =0x0000FFFF
	cmp r2, r0
	bne _08004A3C
	mov r0, #0
	b _08004A72
	.align 2, 0
_08004A38: .4byte 0x0000FFFF
_08004A3C:
	ldr r0, _08004A54 @ =0x000007CF
	cmp r2, r0
	bhi _08004A60
	ldr r3, _08004A58 @ =0x000007FF
	add r1, r3, #0
	add r0, r2, #0
	and r0, r1
	lsl r0, r0, #1
	ldr r4, _08004A5C @ =0x08623DF4
	add r0, r0, r4
	ldrh r0, [r0]
	b _08004A72
_08004A54: .4byte 0x000007CF
_08004A58: .4byte 0x000007FF
_08004A5C: .4byte gUnk_08623DF4
_08004A60:
	ldr r1, _08004A80 @ =0xFFFFF830
	add r0, r2, r1
	ldr r1, _08004A84 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _08004A88 @ =0x08623DF4
	add r0, r0, r3
	ldrh r0, [r0]
	add r0, #1
_08004A72:
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0
	beq _08004A8C
	bl sub_080774EC
	b _08004A94
_08004A80: .4byte 0xFFFFF830
_08004A84: .4byte 0x000007FF
_08004A88: .4byte gUnk_08623DF4
_08004A8C:
	ldr r0, _08004AB8 @ =0x080813E4
	add r1, r2, #0
	bl sub_0801A7DC
_08004A94:
	add r5, #1
	cmp r5, r7
	blt _08004A1A
_08004A9A:
	mov r6, r9
	mov r2, r8
	cmp r2, #0xA
	bhi _08004AA4
	b _0800496E
_08004AA4:
	bl sub_0801A7E8
	add sp, #0x80
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08004AB8: .4byte gUnk_080813E4
	thumb_func_end sub_0800495C

