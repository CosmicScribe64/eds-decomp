	thumb_func_start MoveDeckCardNumberToTop
MoveDeckCardNumberToTop: @ 0x08007D50
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r8, r0
	lsl r1, r1, #0x10
	lsr r7, r1, #0x10
	mov r4, #0
	ldr r2, _08007E28 @ =0x020192E4
	ldrb r0, [r2, #3]
	cmp r4, r0
	bge _08007E56
	mov r5, #1
	mov r0, r8
	and r0, r5
	ldr r3, _08007E2C @ =0x00000D64
	mul r0, r3
	ldr r1, _08007E30 @ =0x000007C4
	add r1, r1, r2
	mov ip, r1
	mov sl, ip
	add r1, r0, r1
	ldr r6, _08007E34 @ =0x000007FF
	mov r9, r6
_08007D84:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	mov r6, r9
	and r0, r6
	lsl r0, r0, #1
	ldr r6, _08007E38 @ =0x08622AB4
	add r0, r0, r6
	ldrh r0, [r0]
	cmp r0, r7
	bne _08007DAC
	add r1, #4
	add r4, #1
	add r0, r4, #0
	and r0, r5
	mul r0, r3
	add r0, r0, r2
	ldrb r0, [r0, #3]
	cmp r4, r0
	blt _08007D84
_08007DAC:
	ldr r0, _08007E3C @ =0xFFFFF83C
	add r0, sl
	mov r9, r0
	add r0, r4, #0
	and r0, r5
	mul r0, r3
	add r0, r9
	ldrb r0, [r0, #3]
	cmp r4, r0
	bge _08007E56
	mov r6, #1
	mov r0, r8
	and r0, r6
	ldr r5, _08007E2C @ =0x00000D64
	add r2, r0, #0
	mul r2, r5
	lsl r1, r4, #2
	mov r3, ip
	add r0, r2, r3
	add r3, r1, r0
	add sl, r2
_08007DD6:
	ldr r1, [r3]
	lsl r0, r1, #0x15
	lsr r0, r0, #0x14
	ldr r2, _08007E38 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r0, r7
	bne _08007E44
	mov r0, sp
	bl CopyDuelCard
	add r6, r4, #0
	cmp r4, #0
	ble _08007E1A
	mov r0, #1
	mov r3, r8
	and r0, r3
	ldr r1, _08007E2C @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	lsl r1, r4, #2
	sub r5, r1, #4
	ldr r0, _08007E40 @ =0x02019AA8
	add r7, r2, r0
	add r4, r1, r7
_08007E08:
	add r1, r7, r5
	add r0, r4, #0
	bl SwapDuelCards
	sub r5, #4
	sub r4, #4
	sub r6, #1
	cmp r6, #0
	bgt _08007E08
_08007E1A:
	mov r6, sl
	ldr r0, [r6]
	mov r1, sp
	bl CopyDuelCard
	b _08007E56
	.align 2, 0
_08007E28: .4byte 0x020192E4
_08007E2C: .4byte 0x00000D64
_08007E30: .4byte 0x000007C4
_08007E34: .4byte 0x000007FF
_08007E38: .4byte gCardIdToNumber
_08007E3C: .4byte 0xFFFFF83C
_08007E40: .4byte 0x02019AA8
_08007E44:
	add r3, #4
	add r4, #1
	add r0, r4, #0
	and r0, r6
	mul r0, r5
	add r0, r9
	ldrb r0, [r0, #3]
	cmp r4, r0
	blt _08007DD6
_08007E56:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end MoveDeckCardNumberToTop
	.align 2, 0

