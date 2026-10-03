	thumb_func_start BattleScene_LoadCardFrame
BattleScene_LoadCardFrame: @ 0x0805E054
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r7, r1, #0
	lsl r2, r2, #0x10
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r8, r3
	ldrh r3, [r7]
	lsl r1, r3, #1
	add r3, r1, #0
	add r3, #8
	add r3, r3, r7
	mov sl, r3
	add r1, #0x10
	add r4, r7, r1
	lsl r0, r0, #0xE
	ldr r5, _0805E0FC @ =0x06004000
	add r0, r0, r5
	lsr r2, r2, #0xB
	add r2, r0, r2
	mov r3, #0
	mov r6, sl
	ldrh r6, [r6]
	lsl r0, r6, #5
	cmp r3, r0
	bge _0805E0D6
	mov r0, #0xFF
	lsl r0, r0, #8
	mov r9, r0
	mov r1, r8
	lsl r0, r1, #0x18
	lsr r5, r0, #0x18
	lsl r6, r5, #8
	mov ip, r6
_0805E09E:
	ldrh r0, [r4]
	add r1, r0, #0
	mov r6, r9
	and r0, r6
	cmp r0, #0
	beq _0805E0B2
	mov r6, ip
	add r0, r1, r6
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_0805E0B2:
	mov r0, #0xFF
	and r0, r1
	cmp r0, #0
	beq _0805E0C0
	add r0, r1, r5
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_0805E0C0:
	strh r1, [r2]
	add r2, #2
	add r4, #2
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	mov r1, sl
	ldrh r1, [r1]
	lsl r0, r1, #5
	cmp r3, r0
	blt _0805E09E
_0805E0D6:
	mov r3, r8
	lsr r0, r3, #4
	lsl r0, r0, #5
	mov r1, #0xA0
	lsl r1, r1, #0x13
	add r0, r0, r1
	add r1, r7, #0
	add r1, #8
	mov r2, #0x40
	bl MemCopy16
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0805E0FC: .4byte 0x06004000
	thumb_func_end BattleScene_LoadCardFrame

