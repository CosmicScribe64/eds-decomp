	thumb_func_start RollPackRarity
RollPackRarity: @ 0x08062A0C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r6, r0, #0
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	bl Random
	mov r1, #0xB4
	bl __modsi3
	add r5, r0, #0
	ldr r7, _08062A9C @ =0x02011C20
	ldr r0, _08062AA0 @ =0x00002154
	add r0, r0, r7
	mov r8, r0
	ldrh r1, [r0]
	cmp r1, r4
	bne _08062A40
	bl Random
	mov r1, #0x87
	lsl r1, r1, #1
	bl __modsi3
	add r5, r0, #0
_08062A40:
	ldr r2, _08062AA4 @ =0x00002156
	add r7, r7, r2
	ldrh r0, [r7]
	cmp r0, #5
	bls _08062A66
	mov r1, r8
	ldrh r1, [r1]
	cmp r1, r4
	bne _08062A56
	cmp r0, #0xA
	bls _08062A66
_08062A56:
	bl Random
	mov r1, #0xC
	bl __modsi3
	add r5, r0, #0
	mov r0, #0
	strh r0, [r7]
_08062A66:
	ldr r1, _08062A9C @ =0x02011C20
	ldr r2, _08062AA0 @ =0x00002154
	add r0, r1, r2
	strh r4, [r0]
	mov r4, #0
	ldr r0, _08062AA4 @ =0x00002156
	add r7, r1, r0
	add r2, r6, #0
	ldr r3, _08062AA8 @ =0x081A570C
_08062A78:
	ldr r0, [r3]
	cmp r5, r0
	bge _08062AAC
	ldr r0, [r2, #4]
	cmp r0, #0
	ble _08062AAC
	cmp r4, #4
	bgt _08062A96
	add r0, r6, #0
	bl GetPackCommonSlot
	cmp r4, r0
	bge _08062A96
	mov r0, #0
	strh r0, [r7]
_08062A96:
	add r0, r4, #0
	b _08062AC6
	.align 2, 0
_08062A9C: .4byte 0x02011C20
_08062AA0: .4byte 0x00002154
_08062AA4: .4byte 0x00002156
_08062AA8: .4byte gPackRarityThresholds
_08062AAC:
	add r2, #8
	add r3, #4
	add r4, #1
	cmp r4, #6
	ble _08062A78
	ldr r2, _08062AD0 @ =0x00002156
	add r0, r1, r2
	ldrh r1, [r0]
	add r1, #1
	strh r1, [r0]
	add r0, r6, #0
	bl GetPackCommonSlot
_08062AC6:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08062AD0: .4byte 0x00002156
	thumb_func_end RollPackRarity

