	thumb_func_start TurnOrder_FlashWhite
TurnOrder_FlashWhite: @ 0x0802965C
	push {r4, r5, r6, lr}
	sub sp, #8
	ldr r1, _080296A4 @ =0x02020310
	mov r0, #0xAD
	lsl r0, r0, #4
	add r3, r1, r0
	ldr r2, _080296A8 @ =0x00000AD2
	add r4, r1, r2
	ldrh r5, [r3]
	ldrh r0, [r4]
	add r2, r5, r0
	strh r2, [r3]
	mov r5, #0xB0
	lsl r5, r5, #4
	add r6, r1, r5
	ldrb r0, [r6]
	cmp r0, #2
	bne _080296B4
	mov r0, #0
	strb r0, [r6]
	mov r0, #0
	strh r0, [r3]
	mov r0, #0x60
	strh r0, [r4]
	ldr r0, _080296AC @ =0x00000AF5
	add r1, r1, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	mov r0, #0
	bl SetBldY
	ldr r1, _080296B0 @ =0x04000050
	mov r0, #0xFF
	strh r0, [r1]
	b _0802972E
_080296A4: .4byte 0x02020310
_080296A8: .4byte 0x00000AD2
_080296AC: .4byte 0x00000AF5
_080296B0: .4byte 0x04000050
_080296B4:
	cmp r0, #1
	beq _080296FE
	lsl r1, r2, #0x10
	mov r0, #0xC0
	lsl r0, r0, #0x14
	cmp r1, r0
	bne _080296C8
	mov r0, #0x80
	lsl r0, r0, #1
	strh r0, [r4]
_080296C8:
	ldrb r1, [r6]
	cmp r1, #1
	beq _080296FE
	ldr r0, _08029738 @ =0x000010FF
	ldrh r3, [r3]
	cmp r3, r0
	bls _080296FE
	mov r4, #1
	neg r4, r4
	str r4, [sp, #0]
	mov r1, #0xA0
	lsl r1, r1, #0x13
	ldr r5, _0802973C @ =0x01000080
	mov r0, sp
	add r2, r5, #0
	bl CpuFastSet
	str r4, [sp, #4]
	add r0, sp, #4
	ldr r1, _08029740 @ =0x05000200
	add r2, r5, #0
	bl CpuFastSet
	add r0, r6, #0
	mov r1, #0x14
	bl Timer_Start
_080296FE:
	ldr r4, _08029744 @ =0x02020310
	mov r2, #0xAD
	lsl r2, r2, #4
	add r0, r4, r2
	ldrh r0, [r0]
	lsr r0, r0, #8
	bl SetBldY
	ldr r3, _08029748 @ =0x00000ABF
	add r0, r4, r3
	ldrb r0, [r0]
	ldr r5, _0802974C @ =0x00000ACE
	add r1, r4, r5
	ldrb r1, [r1]
	add r3, #0x10
	add r2, r4, r3
	ldrb r2, [r2]
	bl TurnOrder_DrawDuelLogo
	add r5, #0x32
	add r4, r4, r5
	add r0, r4, #0
	bl Timer_Tick
_0802972E:
	mov r0, #0
	add sp, #8
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08029738: .4byte 0x000010FF
_0802973C: .4byte 0x01000080
_08029740: .4byte 0x05000200
_08029744: .4byte 0x02020310
_08029748: .4byte 0x00000ABF
_0802974C: .4byte 0x00000ACE
	thumb_func_end TurnOrder_FlashWhite

