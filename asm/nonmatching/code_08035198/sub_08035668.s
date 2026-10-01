	thumb_func_start sub_08035668
sub_08035668: @ 0x08035668
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r9, r0
	mov r0, #4
	mov r1, r9
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08035740
	mov r2, #0
_08035684:
	cmp r2, #0
	beq _08035692
	mov r3, r9
	ldrb r3, [r3, #2]
	lsl r0, r3, #0x1F
	lsr r6, r0, #0x1F
	b _0803569E
_08035692:
	mov r1, r9
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r6, r1, r0
_0803569E:
	mov r7, #0
	add r2, #1
	str r2, [sp, #0]
	mov r8, r6
	mov r2, r8
	mov r3, #1
	and r2, r3
	mov r8, r2
	lsl r0, r6, #0x10
	mov sl, r0
_080356B2:
	mov r0, #0x94
	add r1, r7, #0
	mul r1, r0
	ldr r0, _08035754 @ =0x00000D64
	mov r2, r8
	mul r2, r0
	add r0, r2, #0
	add r1, r1, r0
	ldr r0, _08035758 @ =0x0201930C
	add r5, r1, r0
	ldr r0, [r5]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08035734
	mov r0, #2
	ldrb r3, [r5, #6]
	and r0, r3
	cmp r0, #0
	bne _08035734
	mov r0, #1
	mov r1, r9
	ldrb r1, [r1, #2]
	and r0, r1
	mov r1, #8
	cmp r0, #0
	beq _080356E8
	ldr r1, _0803575C @ =0x00008008
_080356E8:
	lsl r2, r7, #0x18
	lsr r2, r2, #0x10
	add r0, r1, #0
	mov r3, sl
	lsr r1, r3, #0x10
	mov r3, #0
	bl sub_0801EC58
	mov r1, #0x7F
	cmp r6, #0
	beq _08035700
	ldr r1, _08035760 @ =0x0000807F
_08035700:
	lsl r0, r7, #0x10
	lsr r4, r0, #0x10
	add r0, r1, #0
	add r1, r4, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r1, r9
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, [r5]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	bl sub_08019788
	mov r0, #0x7F
	cmp r6, #0
	beq _0803572A
	ldr r0, _08035760 @ =0x0000807F
_0803572A:
	add r1, r4, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08035734:
	add r7, #1
	cmp r7, #0xA
	ble _080356B2
	ldr r2, [sp, #0]
	cmp r2, #1
	ble _08035684
_08035740:
	mov r0, #0
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08035754: .4byte 0x00000D64
_08035758: .4byte 0x0201930C
_0803575C: .4byte 0x00008008
_08035760: .4byte 0x0000807F
	thumb_func_end sub_08035668

