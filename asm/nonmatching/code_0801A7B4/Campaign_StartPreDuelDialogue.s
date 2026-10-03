	thumb_func_start Campaign_StartPreDuelDialogue
Campaign_StartPreDuelDialogue: @ 0x0801AE2C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r3, _0801AE74 @ =0x03000040
	ldr r1, _0801AE78 @ =0x00004870
	add r0, r3, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r4, r0, #0x1B
	ldr r2, _0801AE7C @ =0x0000488A
	add r1, r3, r2
	mov r0, #0x10
	neg r0, r0
	ldrb r5, [r1]
	and r0, r5
	strb r0, [r1]
	ldr r1, _0801AE80 @ =0x0000487C
	add r0, r3, r1
	ldr r2, [r0]
	mov r0, #0x80
	lsl r0, r0, #0x10
	cmp r2, r0
	bne _0801AED8
	add r0, r4, #0
	sub r0, #0xB
	cmp r0, #4
	bls _0801AE68
	bl _0801B626 @ far jump
_0801AE68:
	lsl r0, r0, #2
	ldr r1, _0801AE84 @ =0x0801AE88
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801AE74: .4byte 0x03000040
_0801AE78: .4byte 0x00004870
_0801AE7C: .4byte 0x0000488A
_0801AE80: .4byte 0x0000487C
_0801AE84: .4byte 0x0801AE88
_0801AE88:
	.4byte _0801AE9C
	.4byte _0801AEA8
	.4byte _0801AEB4
	.4byte _0801AEC0
	.4byte _0801AECC
_0801AE9C:
	ldr r0, _0801AEA4 @ =0x00002AFA
	bl StartDialogue
	b _0801B626
_0801AEA4: .4byte 0x00002AFA
_0801AEA8:
	ldr r0, _0801AEB0 @ =0x00002EE2
	bl StartDialogue
	b _0801B626
_0801AEB0: .4byte 0x00002EE2
_0801AEB4:
	ldr r0, _0801AEBC @ =0x000032CA
	bl StartDialogue
	b _0801B626
_0801AEBC: .4byte 0x000032CA
_0801AEC0:
	ldr r0, _0801AEC8 @ =0x000036B2
	bl StartDialogue
	b _0801B626
_0801AEC8: .4byte 0x000036B2
_0801AECC:
	ldr r0, _0801AED4 @ =0x00003A9A
	bl StartDialogue
	b _0801B626
_0801AED4: .4byte 0x00003A9A
_0801AED8:
	sub r0, r4, #1
	cmp r0, #0x1D
	bls _0801AEE0
	b _0801B626
_0801AEE0:
	ldr r0, _0801AF10 @ =0x08081AE4
	lsl r1, r4, #1
	add r0, r1, r0
	ldrh r0, [r0]
	mov r9, r0
	ldr r0, _0801AF14 @ =0x00004888
	add r5, r3, r0
	mov r0, #0xC
	ldrb r3, [r5]
	and r0, r3
	add r6, r1, #0
	cmp r0, #0xC
	bne _0801AF60
	mov r0, #0xF0
	lsl r0, r0, #0x14
	and r0, r2
	cmp r0, #0
	beq _0801AF1C
	ldr r0, _0801AF18 @ =0x08081BE2
_0801AF06:
	add r0, r6, r0
	ldrh r0, [r0]
	bl StartDialogue
	b _0801B626
_0801AF10: .4byte gOpponentFirstMeetingText
_0801AF14: .4byte 0x00004888
_0801AF18: .4byte gOpponentChampionshipText
_0801AF1C:
	mov r0, #0x80
	lsl r0, r0, #0xF
	and r0, r2
	cmp r0, #0
	beq _0801AF30
	ldr r0, _0801AF2C @ =0x08081BAE
	b _0801AF06
	.align 2, 0
_0801AF2C: .4byte gOpponentWeekendDuelText
_0801AF30:
	mov r0, #0xC0
	lsl r0, r0, #0x16
	and r2, r0
	cmp r2, #0
	beq _0801AF44
	ldr r0, _0801AF40 @ =0x08081C16
	b _0801AF06
	.align 2, 0
_0801AF40: .4byte gOpponentGrandpaCupText
_0801AF44:
	ldr r0, _0801AF5C @ =0x08081B7A
	add r0, r6, r0
	ldrh r0, [r0]
	bl StartDialogue
	mov r0, #0x30
	ldrb r5, [r5]
	and r0, r5
	cmp r0, #0
	beq _0801AFBC
	mov r0, #0
	b _0801B628
_0801AF5C: .4byte gOpponentMatchChallengeText
_0801AF60:
	ldr r3, _0801AF90 @ =0x02011C20
	lsl r2, r4, #2
	add r2, r2, r3
	ldr r5, _0801AF94 @ =0x000020D0
	add r0, r2, r5
	ldrh r5, [r0]
	lsl r1, r5, #0x15
	lsr r1, r1, #0x15
	ldr r0, [r0]
	lsl r0, r0, #0xA
	lsr r0, r0, #0x15
	add r1, r1, r0
	ldr r0, _0801AF98 @ =0x000020D2
	add r2, r2, r0
	ldrh r2, [r2]
	lsr r0, r2, #6
	cmn r1, r0
	bne _0801AF9C
	cmp r4, #0x16
	bne _0801AFBC
	bl GiveMissingCards
	b _0801AFBC
	.align 2, 0
_0801AF90: .4byte 0x02011C20
_0801AF94: .4byte 0x000020D0
_0801AF98: .4byte 0x000020D2
_0801AF9C:
	ldr r1, _0801AFAC @ =0x00002158
	add r0, r3, r1
	ldrh r0, [r0]
	cmp r4, r0
	bne _0801AFB4
	ldr r0, _0801AFB0 @ =0x08081B16
	b _0801AFB6
	.align 2, 0
_0801AFAC: .4byte 0x00002158
_0801AFB0: .4byte gOpponentRematchText
_0801AFB4:
	ldr r0, _0801B004 @ =0x08081B48
_0801AFB6:
	add r0, r6, r0
	ldrh r0, [r0]
	mov r9, r0
_0801AFBC:
	ldr r1, _0801B008 @ =0x03000040
	ldr r2, _0801B00C @ =0x0000487C
	add r2, r2, r1
	mov r8, r2
	ldr r0, [r2]
	mov r7, #1
	and r0, r7
	add r5, r1, #0
	cmp r0, #0
	beq _0801B01C
	cmp r4, #0x14
	bne _0801B01C
	ldr r0, _0801B010 @ =0x00004E23
_0801AFD6:
	bl StartDialogue
	ldr r3, _0801B014 @ =0x00004888
	add r1, r5, r3
	mov r0, #0xC
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r3, r8
	str r7, [r3]
	ldr r0, _0801B018 @ =0x0000488A
	add r2, r5, r0
	mov r0, #0x10
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #0xD
_0801AFF8:
	orr r0, r1
	strb r0, [r2]
_0801AFFC:
	mov r0, #0x30
	bl PlayBGM
	b _0801B626
_0801B004: .4byte gOpponentGreetingText
_0801B008: .4byte 0x03000040
_0801B00C: .4byte 0x0000487C
_0801B010: .4byte 0x00004E23
_0801B014: .4byte 0x00004888
_0801B018: .4byte 0x0000488A
_0801B01C:
	ldr r2, _0801B058 @ =0x0000487C
	add r2, r2, r5
	mov r8, r2
	ldr r0, [r2]
	mov r7, #0x80
	lsl r7, r7, #4
	and r0, r7
	cmp r0, #0
	beq _0801B068
	cmp r4, #0xA
	bne _0801B068
	ldr r0, _0801B05C @ =0x00002713
	bl StartDialogue
	ldr r3, _0801B060 @ =0x00004888
	add r1, r5, r3
	mov r0, #0xC
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r3, r8
	str r7, [r3]
	ldr r0, _0801B064 @ =0x0000488A
	add r2, r5, r0
	mov r0, #0x10
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #3
	b _0801AFF8
_0801B058: .4byte 0x0000487C
_0801B05C: .4byte 0x00002713
_0801B060: .4byte 0x00004888
_0801B064: .4byte 0x0000488A
_0801B068:
	ldr r2, _0801B0A4 @ =0x0000487C
	add r2, r2, r5
	mov r8, r2
	ldr r0, [r2]
	mov r7, #2
	and r0, r7
	cmp r0, #0
	beq _0801B0B4
	cmp r4, #6
	bne _0801B0B4
	ldr r0, _0801B0A8 @ =0x00001772
	bl StartDialogue
	ldr r3, _0801B0AC @ =0x00004888
	add r1, r5, r3
	mov r0, #0xC
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r3, r8
	str r7, [r3]
	ldr r0, _0801B0B0 @ =0x0000488A
	add r2, r5, r0
	mov r0, #0x10
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #2
	b _0801AFF8
	.align 2, 0
_0801B0A4: .4byte 0x0000487C
_0801B0A8: .4byte 0x00001772
_0801B0AC: .4byte 0x00004888
_0801B0B0: .4byte 0x0000488A
_0801B0B4:
	ldr r2, _0801B0F0 @ =0x0000487C
	add r2, r2, r5
	mov r8, r2
	ldr r0, [r2]
	mov r7, #4
	and r0, r7
	cmp r0, #0
	beq _0801B100
	cmp r4, #8
	bne _0801B100
	ldr r0, _0801B0F4 @ =0x00001F42
	bl StartDialogue
	ldr r3, _0801B0F8 @ =0x00004888
	add r1, r5, r3
	mov r0, #0xC
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r3, r8
	str r7, [r3]
	ldr r0, _0801B0FC @ =0x0000488A
	add r2, r5, r0
	mov r0, #0x10
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #1
	b _0801AFF8
	.align 2, 0
_0801B0F0: .4byte 0x0000487C
_0801B0F4: .4byte 0x00001F42
_0801B0F8: .4byte 0x00004888
_0801B0FC: .4byte 0x0000488A
_0801B100:
	ldr r2, _0801B13C @ =0x0000487C
	add r2, r2, r5
	mov r8, r2
	ldr r0, [r2]
	mov r7, #8
	and r0, r7
	cmp r0, #0
	beq _0801B14C
	cmp r4, #5
	bne _0801B14C
	ldr r0, _0801B140 @ =0x0000138B
	bl StartDialogue
	ldr r3, _0801B144 @ =0x00004888
	add r1, r5, r3
	mov r0, #0xC
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r3, r8
	str r7, [r3]
	ldr r0, _0801B148 @ =0x0000488A
	add r2, r5, r0
	mov r0, #0x10
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #6
	b _0801AFF8
	.align 2, 0
_0801B13C: .4byte 0x0000487C
_0801B140: .4byte 0x0000138B
_0801B144: .4byte 0x00004888
_0801B148: .4byte 0x0000488A
_0801B14C:
	ldr r2, _0801B188 @ =0x0000487C
	add r2, r2, r5
	mov r8, r2
	ldr r0, [r2]
	mov r7, #0x10
	and r0, r7
	cmp r0, #0
	beq _0801B198
	cmp r4, #4
	bne _0801B198
	ldr r0, _0801B18C @ =0x00000FA3
	bl StartDialogue
	ldr r3, _0801B190 @ =0x00004888
	add r1, r5, r3
	mov r0, #0xC
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r3, r8
	str r7, [r3]
	ldr r0, _0801B194 @ =0x0000488A
	add r2, r5, r0
	mov r0, #0x10
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #2
	b _0801AFF8
	.align 2, 0
_0801B188: .4byte 0x0000487C
_0801B18C: .4byte 0x00000FA3
_0801B190: .4byte 0x00004888
_0801B194: .4byte 0x0000488A
_0801B198:
	ldr r2, _0801B1D4 @ =0x0000487C
	add r2, r2, r5
	mov r8, r2
	ldr r0, [r2]
	mov r7, #0x20
	and r0, r7
	cmp r0, #0
	beq _0801B1E4
	cmp r4, #1
	bne _0801B1E4
	ldr r0, _0801B1D8 @ =0x000003EB
	bl StartDialogue
	ldr r3, _0801B1DC @ =0x00004888
	add r1, r5, r3
	mov r0, #0xC
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r3, r8
	str r7, [r3]
	ldr r0, _0801B1E0 @ =0x0000488A
	add r2, r5, r0
	mov r0, #0x10
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #6
	b _0801AFF8
	.align 2, 0
_0801B1D4: .4byte 0x0000487C
_0801B1D8: .4byte 0x000003EB
_0801B1DC: .4byte 0x00004888
_0801B1E0: .4byte 0x0000488A
_0801B1E4:
	ldr r2, _0801B220 @ =0x0000487C
	add r2, r2, r5
	mov r8, r2
	ldr r0, [r2]
	mov r7, #0x40
	and r0, r7
	cmp r0, #0
	beq _0801B230
	cmp r4, #9
	bne _0801B230
	ldr r0, _0801B224 @ =0x0000232A
	bl StartDialogue
	ldr r3, _0801B228 @ =0x00004888
	add r1, r5, r3
	mov r0, #0xC
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r3, r8
	str r7, [r3]
	ldr r0, _0801B22C @ =0x0000488A
	add r2, r5, r0
	mov r0, #0x10
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #5
	b _0801AFF8
	.align 2, 0
_0801B220: .4byte 0x0000487C
_0801B224: .4byte 0x0000232A
_0801B228: .4byte 0x00004888
_0801B22C: .4byte 0x0000488A
_0801B230:
	ldr r2, _0801B248 @ =0x0000487C
	add r2, r2, r5
	mov r8, r2
	ldr r0, [r2]
	mov r7, #0x80
	and r0, r7
	cmp r0, #0
	beq _0801B250
	cmp r4, #0xC
	bne _0801B250
	ldr r0, _0801B24C @ =0x00002EE6
	b _0801AFD6
_0801B248: .4byte 0x0000487C
_0801B24C: .4byte 0x00002EE6
_0801B250:
	ldr r2, _0801B28C @ =0x0000487C
	add r7, r5, r2
	ldr r0, [r7]
	mov r1, #0x80
	lsl r1, r1, #5
	and r0, r1
	cmp r0, #0
	beq _0801B298
	cmp r4, #3
	bne _0801B298
	ldr r0, _0801B290 @ =0x00000BBB
	bl StartDialogue
	ldr r3, _0801B294 @ =0x00004888
	add r1, r5, r3
	mov r0, #0xC
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r0, #0x80
	lsl r0, r0, #4
	str r0, [r7]
	add r3, #2
	add r2, r5, r3
	mov r0, #0x10
	neg r0, r0
	ldrb r5, [r2]
	and r0, r5
	mov r1, #4
	b _0801AFF8
_0801B28C: .4byte 0x0000487C
_0801B290: .4byte 0x00000BBB
_0801B294: .4byte 0x00004888
_0801B298:
	ldr r0, _0801B2D8 @ =0x0000487C
	add r0, r0, r5
	mov r8, r0
	ldr r0, [r0]
	mov r7, #0x80
	lsl r7, r7, #1
	and r0, r7
	cmp r0, #0
	beq _0801B2E8
	cmp r4, #7
	bne _0801B2E8
	ldr r0, _0801B2DC @ =0x00001B5A
	bl StartDialogue
	ldr r2, _0801B2E0 @ =0x00004888
	add r1, r5, r2
	mov r2, #0xC
	ldrb r0, [r1]
	orr r0, r2
	strb r0, [r1]
	mov r3, r8
	str r7, [r3]
	ldr r0, _0801B2E4 @ =0x0000488A
	add r1, r5, r0
	mov r0, #0x10
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	orr r0, r2
	strb r0, [r1]
	b _0801AFFC
	.align 2, 0
_0801B2D8: .4byte 0x0000487C
_0801B2DC: .4byte 0x00001B5A
_0801B2E0: .4byte 0x00004888
_0801B2E4: .4byte 0x0000488A
_0801B2E8:
	ldr r0, _0801B324 @ =0x0000487C
	add r0, r0, r5
	mov r8, r0
	ldr r0, [r0]
	mov r7, #0x80
	lsl r7, r7, #2
	and r0, r7
	cmp r0, #0
	beq _0801B334
	cmp r4, #2
	bne _0801B334
	ldr r0, _0801B328 @ =0x000007D3
	bl StartDialogue
	ldr r2, _0801B32C @ =0x00004888
	add r1, r5, r2
	mov r0, #0xC
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
	mov r0, r8
	str r7, [r0]
	ldr r1, _0801B330 @ =0x0000488A
	add r2, r5, r1
	mov r0, #0x10
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	mov r1, #7
	b _0801AFF8
_0801B324: .4byte 0x0000487C
_0801B328: .4byte 0x000007D3
_0801B32C: .4byte 0x00004888
_0801B330: .4byte 0x0000488A
_0801B334:
	ldr r0, _0801B384 @ =0x0000487C
	add r0, r0, r5
	mov r8, r0
	ldr r0, [r0]
	mov r7, #0x80
	lsl r7, r7, #3
	and r0, r7
	cmp r0, #0
	beq _0801B394
	cmp r4, #0x10
	bne _0801B394
	ldr r0, _0801B388 @ =0x00003E82
	bl StartDialogue
	ldr r2, _0801B38C @ =0x00004888
	add r1, r5, r2
	mov r0, #0xC
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
	mov r0, r8
	str r7, [r0]
	bl Random
	mov r1, #7
	bl __modsi3
	add r0, #7
	ldr r1, _0801B390 @ =0x0000488A
	add r2, r5, r1
	mov r1, #0xF
	and r0, r1
	mov r1, #0x10
	neg r1, r1
	ldrb r3, [r2]
	and r1, r3
	orr r1, r0
	strb r1, [r2]
	b _0801AFFC
	.align 2, 0
_0801B384: .4byte 0x0000487C
_0801B388: .4byte 0x00003E82
_0801B38C: .4byte 0x00004888
_0801B390: .4byte 0x0000488A
_0801B394:
	ldr r0, _0801B3E4 @ =0x0000487C
	add r0, r0, r5
	mov r8, r0
	ldr r0, [r0]
	mov r7, #0x80
	lsl r7, r7, #8
	and r0, r7
	cmp r0, #0
	beq _0801B3F4
	cmp r4, #0x16
	bne _0801B3F4
	ldr r0, _0801B3E8 @ =0x000055F2
	bl StartDialogue
	ldr r2, _0801B3EC @ =0x00004888
	add r1, r5, r2
	mov r0, #0xC
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
	mov r0, r8
	str r7, [r0]
	bl Random
	mov r1, #0xD
	bl __modsi3
	add r0, #1
	ldr r1, _0801B3F0 @ =0x0000488A
	add r2, r5, r1
	mov r1, #0xF
	and r0, r1
	mov r1, #0x10
	neg r1, r1
	ldrb r3, [r2]
	and r1, r3
	orr r1, r0
	strb r1, [r2]
	b _0801B626
	.align 2, 0
_0801B3E4: .4byte 0x0000487C
_0801B3E8: .4byte 0x000055F2
_0801B3EC: .4byte 0x00004888
_0801B3F0: .4byte 0x0000488A
_0801B3F4:
	ldr r0, _0801B444 @ =0x0000487C
	add r0, r0, r5
	mov r8, r0
	ldr r0, [r0]
	mov r7, #0x80
	lsl r7, r7, #9
	and r0, r7
	cmp r0, #0
	beq _0801B454
	cmp r4, #0xB
	bne _0801B454
	ldr r0, _0801B448 @ =0x00002AFE
	bl StartDialogue
	ldr r2, _0801B44C @ =0x00004888
	add r1, r5, r2
	mov r0, #0xC
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
	mov r0, r8
	str r7, [r0]
	bl Random
	mov r1, #0xD
	bl __modsi3
	add r0, #1
	ldr r1, _0801B450 @ =0x0000488A
	add r2, r5, r1
	mov r1, #0xF
	and r0, r1
	mov r1, #0x10
	neg r1, r1
	ldrb r3, [r2]
	and r1, r3
	orr r1, r0
	strb r1, [r2]
	b _0801AFFC
	.align 2, 0
_0801B444: .4byte 0x0000487C
_0801B448: .4byte 0x00002AFE
_0801B44C: .4byte 0x00004888
_0801B450: .4byte 0x0000488A
_0801B454:
	ldr r0, _0801B4AC @ =0x0000487C
	add r0, r0, r5
	mov r8, r0
	ldr r0, [r0]
	mov r7, #0x80
	lsl r7, r7, #0xB
	and r0, r7
	cmp r0, #0
	beq _0801B4BC
	cmp r4, #2
	beq _0801B46E
	cmp r4, #0xA
	bne _0801B4BC
_0801B46E:
	ldr r0, _0801B4B0 @ =0x08081C76
	add r0, r6, r0
	ldrh r0, [r0]
	bl StartDialogue
	ldr r2, _0801B4B4 @ =0x00004888
	add r1, r5, r2
	mov r0, #0xC
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
	mov r0, r8
	str r7, [r0]
	bl Random
	mov r1, #6
	bl __modsi3
	add r0, #1
	ldr r1, _0801B4B8 @ =0x0000488A
	add r2, r5, r1
	mov r1, #0xF
	and r0, r1
	mov r1, #0x10
	neg r1, r1
	ldrb r3, [r2]
	and r1, r3
	orr r1, r0
	strb r1, [r2]
	b _0801AFFC
	.align 2, 0
_0801B4AC: .4byte 0x0000487C
_0801B4B0: .4byte gOpponentFieldDuelText
_0801B4B4: .4byte 0x00004888
_0801B4B8: .4byte 0x0000488A
_0801B4BC:
	ldr r1, _0801B520 @ =0x0000487C
	add r0, r5, r1
	ldr r0, [r0]
	mov r1, #0x80
	lsl r1, r1, #0xC
	and r0, r1
	cmp r0, #0
	beq _0801B534
	cmp r4, #1
	beq _0801B4DC
	cmp r4, #3
	beq _0801B4DC
	cmp r4, #4
	beq _0801B4DC
	cmp r4, #5
	bne _0801B534
_0801B4DC:
	ldr r0, _0801B524 @ =0x08081C76
	add r0, r6, r0
	ldrh r0, [r0]
	bl StartDialogue
	ldr r4, _0801B528 @ =0x03000040
	ldr r2, _0801B52C @ =0x00004888
	add r1, r4, r2
	mov r0, #0xC
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
	ldr r5, _0801B520 @ =0x0000487C
	add r1, r4, r5
	mov r0, #0x80
	lsl r0, r0, #0xC
	str r0, [r1]
	bl Random
	mov r1, #6
	bl __modsi3
	add r0, #1
	ldr r1, _0801B530 @ =0x0000488A
	add r4, r4, r1
	mov r1, #0xF
	and r0, r1
	mov r1, #0x10
	neg r1, r1
	ldrb r2, [r4]
	and r1, r2
	orr r1, r0
	strb r1, [r4]
	b _0801AFFC
_0801B520: .4byte 0x0000487C
_0801B524: .4byte gOpponentFieldDuelText
_0801B528: .4byte 0x03000040
_0801B52C: .4byte 0x00004888
_0801B530: .4byte 0x0000488A
_0801B534:
	cmp r4, #0x15
	beq _0801B604
	ldr r3, _0801B574 @ =0x0000487C
	add r4, r5, r3
	ldr r1, [r4]
	mov r7, #0x80
	lsl r7, r7, #0xA
	add r0, r1, #0
	and r0, r7
	cmp r0, #0
	beq _0801B584
	ldr r0, _0801B578 @ =0x08081C42
	add r0, r6, r0
	ldrh r0, [r0]
	bl StartDialogue
	ldr r0, _0801B57C @ =0x00004888
	add r1, r5, r0
	mov r0, #0xC
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	str r7, [r4]
	ldr r3, _0801B580 @ =0x0000488A
	add r2, r5, r3
	mov r0, #0x10
	neg r0, r0
	ldrb r5, [r2]
	and r0, r5
	mov r1, #7
	b _0801AFF8
	.align 2, 0
_0801B574: .4byte 0x0000487C
_0801B578: .4byte gOpponentChristmasText
_0801B57C: .4byte 0x00004888
_0801B580: .4byte 0x0000488A
_0801B584:
	mov r7, #0x80
	lsl r7, r7, #6
	add r0, r1, #0
	and r0, r7
	cmp r0, #0
	beq _0801B5C4
	ldr r0, _0801B5B8 @ =0x08081B7A
	add r0, r6, r0
	ldrh r0, [r0]
	bl StartDialogue
	ldr r0, _0801B5BC @ =0x00004888
	add r1, r5, r0
	mov r0, #0xC
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	str r7, [r4]
	ldr r3, _0801B5C0 @ =0x0000488A
	add r2, r5, r3
	mov r0, #0x10
	neg r0, r0
	ldrb r5, [r2]
	and r0, r5
	mov r1, #4
	b _0801AFF8
_0801B5B8: .4byte gOpponentMatchChallengeText
_0801B5BC: .4byte 0x00004888
_0801B5C0: .4byte 0x0000488A
_0801B5C4:
	mov r7, #0x80
	lsl r7, r7, #7
	and r1, r7
	cmp r1, #0
	beq _0801B604
	ldr r0, _0801B5F8 @ =0x08081B7A
	add r0, r6, r0
	ldrh r0, [r0]
	bl StartDialogue
	ldr r0, _0801B5FC @ =0x00004888
	add r1, r5, r0
	mov r0, #0xC
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	str r7, [r4]
	ldr r3, _0801B600 @ =0x0000488A
	add r2, r5, r3
	mov r0, #0x10
	neg r0, r0
	ldrb r5, [r2]
	and r0, r5
	mov r1, #3
	b _0801AFF8
	.align 2, 0
_0801B5F8: .4byte gOpponentMatchChallengeText
_0801B5FC: .4byte 0x00004888
_0801B600: .4byte 0x0000488A
_0801B604:
	ldr r0, _0801B634 @ =0x0000487C
	add r1, r5, r0
	mov r0, #0
	str r0, [r1]
	ldr r2, _0801B638 @ =0x080819F6
	ldr r3, _0801B63C @ =0x00004870
	add r1, r5, r3
	mov r0, #0x3E
	ldrb r1, [r1]
	and r0, r1
	add r0, r0, r2
	ldrh r0, [r0]
	bl PlayBGM
	mov r0, r9
	bl StartDialogue
_0801B626:
	mov r0, #1
_0801B628:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0801B634: .4byte 0x0000487C
_0801B638: .4byte gOpponentDialogueBGM
_0801B63C: .4byte 0x00004870
	thumb_func_end Campaign_StartPreDuelDialogue

