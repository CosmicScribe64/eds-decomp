	thumb_func_start CardTrading_DrawMenu
CardTrading_DrawMenu: @ 0x0807CCAC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	mov r8, r0
	str r2, [sp, #4]
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	str r1, [sp, #0]
	cmp r1, #0
	beq _0807CCD4
	mov r0, #2
	mov sl, r0
	mov r0, #1
	mov r1, r8
	and r1, r0
	mov r8, r1
	b _0807CCDC
_0807CCD4:
	mov r3, #1
	mov sl, r3
	mov r5, #0
	mov r8, r5
_0807CCDC:
	mov r5, #0
	cmp r5, sl
	bge _0807CD2E
_0807CCE2:
	mov r4, #0
	lsl r7, r5, #7
	add r0, r5, #1
	mov r9, r0
	lsl r0, r5, #0x15
	mov r1, #0xC0
	lsl r1, r1, #0xE
	add r6, r0, r1
_0807CCF2:
	mov r0, #0
	cmp r5, r8
	bne _0807CCFA
	mov r0, #1
_0807CCFA:
	mov r3, #0
	cmp r0, #0
	bne _0807CD02
	mov r3, #0x10
_0807CD02:
	mov r1, #0
	cmp r0, #0
	bne _0807CD0C
	mov r1, #0x80
	lsl r1, r1, #5
_0807CD0C:
	lsl r0, r4, #5
	orr r0, r6
	lsl r2, r4, #2
	add r2, r3, r2
	add r2, r2, r7
	orr r2, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r1, #0x80
	bl AddSprite
	add r4, #1
	cmp r4, #3
	ble _0807CCF2
	mov r5, r9
	cmp r5, sl
	blt _0807CCE2
_0807CD2E:
	ldr r3, [sp, #0]
	cmp r3, #0
	beq _0807CD82
	ldr r0, _0807CD94 @ =0x00380098
	mov r1, #0xC
	ldr r2, [sp, #4]
	and r2, r1
	mov r5, #0x80
	lsl r5, r5, #1
	add r1, r5, #0
	add r2, r2, r1
	mov r3, #0x80
	lsl r3, r3, #6
	add r1, r3, #0
	orr r2, r1
	ldr r5, [sp, #4]
	lsl r3, r5, #0x15
	mov r1, #0x80
	lsl r1, r1, #0x11
	add r3, r3, r1
	ldr r4, _0807CD98 @ =0x0201F780
	ldrh r5, [r4, #2]
	lsl r1, r5, #0x17
	lsr r1, r1, #0x19
	orr r3, r1
	mov r1, #0x80
	bl AddAffineSprite
	ldr r0, [sp, #4]
	cmp r0, #0
	bne _0807CD82
	ldrh r2, [r4, #2]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #2
	ldr r0, _0807CD9C @ =0xFFFFFE03
	and r0, r2
	orr r0, r1
	strh r0, [r4, #2]
_0807CD82:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0807CD94: .4byte 0x00380098
_0807CD98: .4byte 0x0201F780
_0807CD9C: .4byte 0xFFFFFE03
	thumb_func_end CardTrading_DrawMenu

