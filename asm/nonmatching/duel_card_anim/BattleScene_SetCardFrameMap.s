	thumb_func_start BattleScene_SetCardFrameMap
BattleScene_SetCardFrameMap: @ 0x0805E1D0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	mov sl, r1
	lsl r0, r0, #0xB
	mov r1, #0xC0
	lsl r1, r1, #0x13
	add r7, r0, r1
	lsr r0, r3, #0x1F
	add r0, r3, r0
	asr r3, r0, #1
	mov r1, sl
	lsr r0, r1, #0x1F
	add r0, sl
	asr r0, r0, #1
	lsl r0, r0, #1
	add r7, r7, r0
	lsl r2, r2, #5
	add r7, r7, r2
	mov r0, #1
	and r1, r0
	str r1, [sp, #0]
	mov r1, #3
	mov ip, r1
	mov r0, #0x80
	lsl r0, r0, #0x11
	mov r9, r0
	lsl r2, r3, #0x18
_0805E20E:
	ldr r1, [sp, #0]
	cmp r1, #0
	beq _0805E24E
	lsr r0, r2, #0x18
	lsl r0, r0, #8
	strh r0, [r7]
	mov r0, #0x80
	lsl r0, r0, #0x11
	add r2, r2, r0
	add r3, #1
	add r6, r7, #2
	mov r1, r9
	add r4, r2, r1
	mov r0, #0x80
	lsl r0, r0, #0x12
	mov r8, r0
	mov r5, #5
_0805E230:
	lsr r1, r2, #0x18
	lsr r0, r4, #0x18
	lsl r0, r0, #8
	orr r1, r0
	strh r1, [r6]
	add r4, r8
	mov r1, #0x80
	lsl r1, r1, #0x12
	add r2, r2, r1
	add r3, #2
	add r6, #2
	sub r5, #1
	cmp r5, #0
	bge _0805E230
	b _0805E284
_0805E24E:
	add r6, r7, #0
	mov r0, r9
	add r4, r2, r0
	mov r1, #0x80
	lsl r1, r1, #0x12
	mov r8, r1
	mov r5, #5
_0805E25C:
	lsr r1, r2, #0x18
	lsr r0, r4, #0x18
	lsl r0, r0, #8
	orr r1, r0
	strh r1, [r6]
	add r4, r8
	mov r0, #0x80
	lsl r0, r0, #0x12
	add r2, r2, r0
	add r3, #2
	add r6, #2
	sub r5, #1
	cmp r5, #0
	bge _0805E25C
	lsr r0, r2, #0x18
	strh r0, [r7, #0xC]
	mov r1, #0x80
	lsl r1, r1, #0x11
	add r2, r2, r1
	add r3, #1
_0805E284:
	add r7, #0x20
	mov r0, #1
	neg r0, r0
	add ip, r0
	mov r1, ip
	cmp r1, #0
	bge _0805E20E
	mov r2, #1
	mov r0, sl
	and r2, r0
	mov r1, #9
	mov ip, r1
_0805E29C:
	cmp r2, #0
	beq _0805E2CE
	lsl r0, r3, #0x18
	lsr r0, r0, #0x10
	ldrh r1, [r7]
	orr r0, r1
	strh r0, [r7]
	add r3, #1
	lsl r0, r3, #0x18
	lsr r0, r0, #0x18
	ldrh r1, [r7, #2]
	orr r0, r1
	strh r0, [r7, #2]
	add r3, #1
	lsl r1, r3, #0x18
	lsr r1, r1, #0x18
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r1, r0
	ldrh r0, [r7, #0xC]
	orr r1, r0
	strh r1, [r7, #0xC]
	add r3, #2
	b _0805E2FA
_0805E2CE:
	lsl r1, r3, #0x18
	lsr r1, r1, #0x18
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r1, r0
	ldrh r0, [r7]
	orr r1, r0
	strh r1, [r7]
	add r3, #2
	lsl r0, r3, #0x18
	lsr r0, r0, #0x10
	ldrh r1, [r7, #0xA]
	orr r0, r1
	strh r0, [r7, #0xA]
	add r3, #1
	lsl r0, r3, #0x18
	lsr r0, r0, #0x18
	ldrh r1, [r7, #0xC]
	orr r0, r1
	strh r0, [r7, #0xC]
	add r3, #1
_0805E2FA:
	add r7, #0x20
	mov r0, #1
	neg r0, r0
	add ip, r0
	mov r1, ip
	cmp r1, #0
	bge _0805E29C
	mov r0, #0
	mov ip, r0
	mov r1, #1
	mov r0, sl
	and r0, r1
	str r0, [sp, #4]
	mov r1, #0x80
	lsl r1, r1, #0x11
	mov sl, r1
	lsl r3, r3, #0x18
_0805E31C:
	ldr r0, [sp, #4]
	cmp r0, #0
	beq _0805E362
	lsr r0, r3, #0x18
	lsl r0, r0, #8
	strh r0, [r7]
	mov r1, #0x80
	lsl r1, r1, #0x11
	add r3, r3, r1
	mov r0, #0x20
	add r0, r0, r7
	mov r9, r0
	mov r1, #1
	add r1, ip
	mov r8, r1
	add r4, r7, #2
	mov r0, sl
	add r2, r3, r0
	mov r6, #0x80
	lsl r6, r6, #0x12
	mov r5, #5
_0805E346:
	lsr r1, r3, #0x18
	lsr r0, r2, #0x18
	lsl r0, r0, #8
	orr r1, r0
	strh r1, [r4]
	add r2, r2, r6
	mov r1, #0x80
	lsl r1, r1, #0x12
	add r3, r3, r1
	add r4, #2
	sub r5, #1
	cmp r5, #0
	bge _0805E346
	b _0805E39E
_0805E362:
	mov r0, #0x20
	add r0, r0, r7
	mov r9, r0
	mov r1, #1
	add r1, ip
	mov r8, r1
	add r4, r7, #0
	mov r0, sl
	add r2, r3, r0
	mov r6, #0x80
	lsl r6, r6, #0x12
	mov r5, #5
_0805E37A:
	lsr r1, r3, #0x18
	lsr r0, r2, #0x18
	lsl r0, r0, #8
	orr r1, r0
	strh r1, [r4]
	add r2, r2, r6
	mov r1, #0x80
	lsl r1, r1, #0x12
	add r3, r3, r1
	add r4, #2
	sub r5, #1
	cmp r5, #0
	bge _0805E37A
	lsr r0, r3, #0x18
	strh r0, [r7, #0xC]
	mov r0, #0x80
	lsl r0, r0, #0x11
	add r3, r3, r0
_0805E39E:
	mov r7, r9
	mov ip, r8
	mov r1, ip
	cmp r1, #3
	ble _0805E31C
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end BattleScene_SetCardFrameMap

