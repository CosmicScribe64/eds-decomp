	thumb_func_start BattleScene_HBlank
BattleScene_HBlank: @ 0x0805DC38
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r2, _0805DD0C @ =0x03000040
	ldr r0, _0805DD10 @ =0x04000006
	ldrh r0, [r0]
	ldr r1, _0805DD14 @ =0x00004862
	add r2, r2, r1
	strh r0, [r2]
	ldr r3, _0805DD18 @ =0x04000028
	ldr r6, _0805DD1C @ =0x0819DD94
	lsl r1, r0, #2
	ldr r0, _0805DD20 @ =0x02018450
	ldr r4, _0805DD24 @ =0x0000015D
	add r4, r4, r0
	mov ip, r4
	ldrb r5, [r4]
	lsl r0, r5, #2
	add r7, r5, #0
	add r0, r0, r7
	lsl r0, r0, #7
	add r1, r1, r0
	add r1, r1, r6
	ldr r0, [r1]
	str r0, [r3]
	add r3, #4
	ldr r5, _0805DD28 @ =0x081A0594
	ldrh r0, [r2]
	lsl r1, r0, #2
	ldrb r4, [r4]
	lsl r0, r4, #2
	mov r7, ip
	ldrb r7, [r7]
	add r0, r0, r7
	lsl r0, r0, #7
	add r1, r1, r0
	add r1, r1, r5
	ldr r0, [r1]
	str r0, [r3]
	ldr r0, _0805DD2C @ =0x04000020
	mov r8, r0
	ldr r4, _0805DD30 @ =0x081A2D94
	ldrh r3, [r2]
	lsl r1, r3, #1
	mov r7, ip
	ldrb r7, [r7]
	lsl r0, r7, #2
	mov r3, ip
	ldrb r3, [r3]
	add r0, r0, r3
	lsl r0, r0, #6
	add r1, r1, r0
	add r1, r1, r4
	ldrh r0, [r1]
	mov r7, r8
	strh r0, [r7]
	ldr r0, _0805DD34 @ =0x04000038
	mov r8, r0
	ldrh r3, [r2]
	lsl r1, r3, #2
	mov r7, ip
	ldrb r7, [r7]
	lsl r0, r7, #2
	mov r3, ip
	ldrb r3, [r3]
	add r0, r0, r3
	lsl r0, r0, #7
	add r1, r1, r0
	add r1, r1, r6
	ldr r0, [r1]
	mov r6, r8
	str r0, [r6]
	ldr r3, _0805DD38 @ =0x0400003C
	ldrh r7, [r2]
	lsl r1, r7, #2
	mov r6, ip
	ldrb r6, [r6]
	lsl r0, r6, #2
	mov r7, ip
	ldrb r7, [r7]
	add r0, r0, r7
	lsl r0, r0, #7
	add r1, r1, r0
	add r1, r1, r5
	ldr r0, [r1]
	str r0, [r3]
	sub r3, #0xC
	ldrh r2, [r2]
	lsl r1, r2, #1
	mov r2, ip
	ldrb r2, [r2]
	lsl r0, r2, #2
	mov r5, ip
	ldrb r5, [r5]
	add r0, r0, r5
	lsl r0, r0, #6
	add r1, r1, r0
	add r1, r1, r4
	ldrh r0, [r1]
	strh r0, [r3]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0805DD0C: .4byte 0x03000040
_0805DD10: .4byte 0x04000006
_0805DD14: .4byte 0x00004862
_0805DD18: .4byte 0x04000028
_0805DD1C: .4byte gBattleSceneOpenBgX
_0805DD20: .4byte 0x02018450
_0805DD24: .4byte 0x0000015D
_0805DD28: .4byte gBattleSceneOpenBgY
_0805DD2C: .4byte 0x04000020
_0805DD30: .4byte gBattleSceneOpenBgPA
_0805DD34: .4byte 0x04000038
_0805DD38: .4byte 0x0400003C
	thumb_func_end BattleScene_HBlank

