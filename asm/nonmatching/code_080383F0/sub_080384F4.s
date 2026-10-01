	thumb_func_start sub_080384F4
sub_080384F4: @ 0x080384F4
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r6, r0, #0
	bl sub_08076F9C
	mov r1, #6
	bl __modsi3
	add r0, #1
	mov r9, r0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _080385E8
	ldr r0, _08038534 @ =0x000007FF
	ldrh r2, [r6]
	and r0, r2
	lsl r0, r0, #1
	ldr r3, _08038538 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _0803853C @ =0x000004B3
	cmp r1, r0
	beq _08038540
	add r0, #1
	cmp r1, r0
	beq _0803854A
	ldrb r1, [r6, #2]
	b _08038558
_08038534: .4byte 0x000007FF
_08038538: .4byte gUnk_08622AB4
_0803853C: .4byte 0x000004B3
_08038540:
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r7, r0, #0x1F
	mov r4, #0xE2
	b _08038558
_0803854A:
	ldrb r0, [r6, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	mov r2, #1
	sub r7, r2, r1
	mov r4, #0xE3
	add r1, r0, #0
_08038558:
	mov r0, #1
	and r0, r1
	cmp r0, #0
	beq _08038566
	mov r0, #0x80
	lsl r0, r0, #8
	orr r4, r0
_08038566:
	lsl r0, r4, #0x10
	lsr r0, r0, #0x10
	mov r2, r9
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r4, #1
	add r0, r4, #0
	ldrb r3, [r6, #2]
	and r0, r3
	mov r1, #0x12
	cmp r0, #0
	beq _08038588
	ldr r1, _080385F8 @ =0x00008012
_08038588:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r5, #0
	add r0, r7, #0
	and r0, r4
	lsl r1, r7, #0x18
	lsr r1, r1, #0x18
	mov r8, r1
	ldr r1, _080385FC @ =0x00000D64
	add r7, r0, #0
	mul r7, r1
_080385A6:
	mov r0, #0x94
	mul r0, r5
	add r0, r0, r7
	ldr r1, _08038600 @ =0x0201930C
	add r1, r0, r1
	mov r0, #2
	ldrb r2, [r1, #6]
	and r0, r2
	cmp r0, #0
	beq _080385E2
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080385E2
	ldrb r3, [r6, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	ldrh r1, [r6]
	lsl r2, r5, #0x18
	lsr r2, r2, #0x10
	mov r3, r8
	orr r2, r3
	mov r3, r9
	lsl r4, r3, #0x18
	mov r3, #0xC0
	lsl r3, r3, #0xA
	orr r3, r4
	lsr r3, r3, #0x10
	bl sub_08017AB4
_080385E2:
	add r5, #1
	cmp r5, #4
	ble _080385A6
_080385E8:
	mov r0, #0
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080385F8: .4byte 0x00008012
_080385FC: .4byte 0x00000D64
_08038600: .4byte 0x0201930C
	thumb_func_end sub_080384F4

