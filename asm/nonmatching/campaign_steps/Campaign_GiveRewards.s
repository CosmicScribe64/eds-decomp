	thumb_func_start Campaign_GiveRewards
Campaign_GiveRewards: @ 0x0801BF80
	push {r4, r5, lr}
	ldr r0, _0801BFA4 @ =0x03000040
	ldr r2, _0801BFA8 @ =0x0000488A
	add r1, r0, r2
	ldrh r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x18
	add r4, r0, #0
	cmp r1, #0x19
	bls _0801BF98
	bl _0801C92E @ far jump
_0801BF98:
	lsl r0, r1, #2
	ldr r1, _0801BFAC @ =0x0801BFB0
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801BFA4: .4byte 0x03000040
_0801BFA8: .4byte 0x0000488A
_0801BFAC: .4byte 0x0801BFB0
_0801BFB0:
	.4byte _0801C018
	.4byte _0801C040
	.4byte _0801C634
	.4byte _0801C6BC
	.4byte _0801C710
	.4byte _0801C92E
	.4byte _0801C92E
	.4byte _0801C92E
	.4byte _0801C92E
	.4byte _0801C92E
	.4byte _0801C716
	.4byte _0801C7C0
	.4byte _0801C80C
	.4byte _0801C836
	.4byte _0801C844
	.4byte _0801C894
	.4byte _0801C92E
	.4byte _0801C92E
	.4byte _0801C92E
	.4byte _0801C92E
	.4byte _0801C924
	.4byte _0801C92E
	.4byte _0801C8A4
	.4byte _0801C8CE
	.4byte _0801C924
	.4byte _0801C92E
_0801C018:
	bl Campaign_RecordDuelResult
	ldr r0, _0801C034 @ =0x03000040
	ldr r3, _0801C038 @ =0x0000488A
	add r0, r0, r3
	ldr r1, _0801C03C @ =0xFFFFF00F
	ldrh r2, [r0]
	and r1, r2
	mov r2, #0x10
	orr r1, r2
	strh r1, [r0]
_0801C02E:
	mov r0, #0
	bl _0801C930 @ far jump
_0801C034: .4byte 0x03000040
_0801C038: .4byte 0x0000488A
_0801C03C: .4byte 0xFFFFF00F
_0801C040:
	ldr r0, _0801C058 @ =0x020192E0
	ldr r3, _0801C05C @ =0x00001B12
	add r0, r0, r3
	ldrb r0, [r0]
	lsr r2, r0, #6
	cmp r2, #1
	beq _0801C060
	cmp r2, #2
	bne _0801C054
	b _0801C4D4
_0801C054:
	bl _0801C92E @ far jump
_0801C058: .4byte 0x020192E0
_0801C05C: .4byte 0x00001B12
_0801C060:
	ldr r1, _0801C0AC @ =0x00004859
	add r0, r4, r1
	mov r5, #0
	strb r5, [r0]
	ldr r3, _0801C0B0 @ =0x0000485A
	add r0, r4, r3
	strb r5, [r0]
	add r1, #2
	add r0, r4, r1
	strb r5, [r0]
	add r3, #0x22
	add r0, r4, r3
	ldr r1, [r0]
	mov r0, #0x80
	lsl r0, r0, #6
	cmp r1, r0
	bne _0801C084
	b _0801C480
_0801C084:
	cmp r1, r0
	bhi _0801C10E
	cmp r1, #0x40
	bne _0801C08E
	b _0801C3F0
_0801C08E:
	cmp r1, #0x40
	bhi _0801C0CE
	cmp r1, #4
	bne _0801C098
	b _0801C334
_0801C098:
	cmp r1, #4
	bhi _0801C0B4
	cmp r1, #1
	bne _0801C0A2
	b _0801C334
_0801C0A2:
	cmp r1, #2
	bne _0801C0A8
	b _0801C35C
_0801C0A8:
	b _0801C4C8
	.align 2, 0
_0801C0AC: .4byte 0x00004859
_0801C0B0: .4byte 0x0000485A
_0801C0B4:
	cmp r1, #0x10
	bne _0801C0BA
	b _0801C3A4
_0801C0BA:
	cmp r1, #0x10
	bhi _0801C0C6
	cmp r1, #8
	bne _0801C0C4
	b _0801C380
_0801C0C4:
	b _0801C4C8
_0801C0C6:
	cmp r1, #0x20
	bne _0801C0CC
	b _0801C3C8
_0801C0CC:
	b _0801C4C8
_0801C0CE:
	mov r0, #0x80
	lsl r0, r0, #2
	cmp r1, r0
	bne _0801C0D8
	b _0801C3C8
_0801C0D8:
	cmp r1, r0
	bhi _0801C0EE
	cmp r1, #0x80
	bne _0801C0E2
	b _0801C414
_0801C0E2:
	mov r0, #0x80
	lsl r0, r0, #1
	cmp r1, r0
	bne _0801C0EC
	b _0801C438
_0801C0EC:
	b _0801C4C8
_0801C0EE:
	mov r0, #0x80
	lsl r0, r0, #4
	cmp r1, r0
	bne _0801C0F8
	b _0801C45C
_0801C0F8:
	cmp r1, r0
	bhi _0801C102
	mov r0, #0x80
	lsl r0, r0, #3
	b _0801C138
_0801C102:
	mov r0, #0x80
	lsl r0, r0, #5
	cmp r1, r0
	bne _0801C10C
	b _0801C3C8
_0801C10C:
	b _0801C4C8
_0801C10E:
	mov r0, #0x80
	lsl r0, r0, #0x10
	cmp r1, r0
	bne _0801C118
	b _0801C310
_0801C118:
	cmp r1, r0
	bhi _0801C166
	mov r0, #0x80
	lsl r0, r0, #9
	cmp r1, r0
	bne _0801C126
	b _0801C480
_0801C126:
	cmp r1, r0
	bhi _0801C140
	mov r0, #0x80
	lsl r0, r0, #7
	cmp r1, r0
	bne _0801C134
	b _0801C480
_0801C134:
	mov r0, #0x80
	lsl r0, r0, #8
_0801C138:
	cmp r1, r0
	bne _0801C13E
	b _0801C334
_0801C13E:
	b _0801C4C8
_0801C140:
	mov r0, #0x80
	lsl r0, r0, #0xB
	cmp r1, r0
	bne _0801C14A
	b _0801C4A4
_0801C14A:
	cmp r1, r0
	bhi _0801C15A
	mov r0, #0x80
	lsl r0, r0, #0xA
	cmp r1, r0
	bne _0801C158
	b _0801C480
_0801C158:
	b _0801C4C8
_0801C15A:
	mov r0, #0x80
	lsl r0, r0, #0xC
	cmp r1, r0
	bne _0801C164
	b _0801C4A4
_0801C164:
	b _0801C4C8
_0801C166:
	mov r0, #0x80
	lsl r0, r0, #0x13
	cmp r1, r0
	beq _0801C228
	cmp r1, r0
	bhi _0801C184
	mov r0, #0x80
	lsl r0, r0, #0x11
	cmp r1, r0
	beq _0801C1A8
	mov r0, #0x80
	lsl r0, r0, #0x12
	cmp r1, r0
	beq _0801C1E8
	b _0801C4C8
_0801C184:
	mov r0, #0x80
	lsl r0, r0, #0x15
	cmp r1, r0
	bne _0801C18E
	b _0801C2AC
_0801C18E:
	cmp r1, r0
	bhi _0801C19C
	mov r0, #0x80
	lsl r0, r0, #0x14
	cmp r1, r0
	beq _0801C268
	b _0801C4C8
_0801C19C:
	mov r0, #0x80
	lsl r0, r0, #0x16
	cmp r1, r0
	bne _0801C1A6
	b _0801C2E0
_0801C1A6:
	b _0801C4C8
_0801C1A8:
	mov r0, #0xC8
	bl StartDialogue
	ldr r0, _0801C1D4 @ =0x02011C20
	ldr r1, _0801C1D8 @ =0x0000215E
	add r0, r0, r1
	ldrh r1, [r0]
	add r1, #1
	strh r1, [r0]
	ldr r0, _0801C1DC @ =0x08624CD0
	ldrh r1, [r0]
	mov r2, #0x91
	lsl r2, r2, #7
	add r0, r4, r2
	strh r1, [r0]
	ldr r3, _0801C1E0 @ =0x0000488A
	add r2, r4, r3
	ldr r0, _0801C1E4 @ =0xFFFFF00F
	ldrh r1, [r2]
	and r0, r1
	mov r1, #0xA0
	b _0801C908
_0801C1D4: .4byte 0x02011C20
_0801C1D8: .4byte 0x0000215E
_0801C1DC: .4byte gCardNumberToId_SetSailForTheKingdom
_0801C1E0: .4byte 0x0000488A
_0801C1E4: .4byte 0xFFFFF00F
_0801C1E8:
	mov r0, #0xCA
	bl StartDialogue
	ldr r0, _0801C214 @ =0x02011C20
	ldr r2, _0801C218 @ =0x0000215E
	add r0, r0, r2
	ldrh r1, [r0]
	add r1, #1
	strh r1, [r0]
	ldr r0, _0801C21C @ =0x08624CCE
	ldrh r1, [r0]
	mov r3, #0x91
	lsl r3, r3, #7
	add r0, r4, r3
	strh r1, [r0]
	ldr r0, _0801C220 @ =0x0000488A
	add r2, r4, r0
	ldr r0, _0801C224 @ =0xFFFFF00F
	ldrh r1, [r2]
	and r0, r1
	mov r1, #0xA0
	b _0801C908
_0801C214: .4byte 0x02011C20
_0801C218: .4byte 0x0000215E
_0801C21C: .4byte gCardNumberToId_TheMonarchy
_0801C220: .4byte 0x0000488A
_0801C224: .4byte 0xFFFFF00F
_0801C228:
	mov r0, #0xCC
	bl StartDialogue
	ldr r0, _0801C254 @ =0x02011C20
	ldr r2, _0801C258 @ =0x0000215E
	add r0, r0, r2
	ldrh r1, [r0]
	add r1, #1
	strh r1, [r0]
	ldr r0, _0801C25C @ =0x08624CD2
	ldrh r1, [r0]
	mov r3, #0x91
	lsl r3, r3, #7
	add r0, r4, r3
	strh r1, [r0]
	ldr r0, _0801C260 @ =0x0000488A
	add r2, r4, r0
	ldr r0, _0801C264 @ =0xFFFFF00F
	ldrh r1, [r2]
	and r0, r1
	mov r1, #0xA0
	b _0801C908
_0801C254: .4byte 0x02011C20
_0801C258: .4byte 0x0000215E
_0801C25C: .4byte gCardNumberToId_GloryOfTheKingsHand
_0801C260: .4byte 0x0000488A
_0801C264: .4byte 0xFFFFF00F
_0801C268:
	mov r0, #0xCE
	bl StartDialogue
	ldr r0, _0801C298 @ =0x02011C20
	ldr r2, _0801C29C @ =0x0000215E
	add r1, r0, r2
	strh r5, [r1]
	ldr r3, _0801C2A0 @ =0x00002162
	add r0, r0, r3
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	mov r1, #0x91
	lsl r1, r1, #7
	add r0, r4, r1
	strh r5, [r0]
	ldr r3, _0801C2A4 @ =0x0000488A
	add r2, r4, r3
	ldr r0, _0801C2A8 @ =0xFFFFF00F
	ldrh r1, [r2]
	and r0, r1
	mov r1, #0xA0
	b _0801C908
	.align 2, 0
_0801C298: .4byte 0x02011C20
_0801C29C: .4byte 0x0000215E
_0801C2A0: .4byte 0x00002162
_0801C2A4: .4byte 0x0000488A
_0801C2A8: .4byte 0xFFFFF00F
_0801C2AC:
	ldr r0, _0801C2CC @ =0x02011C20
	ldr r3, _0801C2D0 @ =0x00002160
	add r0, r0, r3
	strh r2, [r0]
	ldr r0, _0801C2D4 @ =0x00004876
	add r1, r4, r0
	mov r0, #0xFC
	lsl r0, r0, #1
	strh r0, [r1]
	ldr r1, _0801C2D8 @ =0x0000488A
	add r2, r4, r1
	ldr r0, _0801C2DC @ =0xFFFFF00F
	ldrh r3, [r2]
	and r0, r3
	mov r1, #0xF0
	b _0801C908
_0801C2CC: .4byte 0x02011C20
_0801C2D0: .4byte 0x00002160
_0801C2D4: .4byte 0x00004876
_0801C2D8: .4byte 0x0000488A
_0801C2DC: .4byte 0xFFFFF00F
_0801C2E0:
	ldr r0, _0801C2FC @ =0x000002BF
	bl StartDialogue
	ldr r0, _0801C300 @ =0x00004876
	add r1, r4, r0
	ldr r0, _0801C304 @ =0x00000386
	strh r0, [r1]
	ldr r1, _0801C308 @ =0x0000488A
	add r2, r4, r1
	ldr r0, _0801C30C @ =0xFFFFF00F
	ldrh r3, [r2]
	and r0, r3
	mov r1, #0xE0
	b _0801C908
_0801C2FC: .4byte 0x000002BF
_0801C300: .4byte 0x00004876
_0801C304: .4byte 0x00000386
_0801C308: .4byte 0x0000488A
_0801C30C: .4byte 0xFFFFF00F
_0801C310:
	ldr r0, _0801C328 @ =0x00004876
	add r1, r4, r0
	mov r0, #0xFD
	lsl r0, r0, #1
	strh r0, [r1]
	ldr r1, _0801C32C @ =0x0000488A
	add r2, r4, r1
	ldr r0, _0801C330 @ =0xFFFFF00F
	ldrh r3, [r2]
	and r0, r3
	mov r1, #0xF0
	b _0801C908
_0801C328: .4byte 0x00004876
_0801C32C: .4byte 0x0000488A
_0801C330: .4byte 0xFFFFF00F
_0801C334:
	ldr r0, _0801C34C @ =0x00004876
	add r1, r4, r0
	ldr r0, _0801C350 @ =0x000001FD
	strh r0, [r1]
	ldr r1, _0801C354 @ =0x0000488A
	add r2, r4, r1
	ldr r0, _0801C358 @ =0xFFFFF00F
	ldrh r3, [r2]
	and r0, r3
	mov r1, #0xF0
	b _0801C908
	.align 2, 0
_0801C34C: .4byte 0x00004876
_0801C350: .4byte 0x000001FD
_0801C354: .4byte 0x0000488A
_0801C358: .4byte 0xFFFFF00F
_0801C35C:
	ldr r0, _0801C374 @ =0x00004876
	add r1, r4, r0
	mov r0, #5
	strh r0, [r1]
	ldr r1, _0801C378 @ =0x0000488A
	add r2, r4, r1
	ldr r0, _0801C37C @ =0xFFFFF00F
	ldrh r3, [r2]
	and r0, r3
	mov r1, #0xF0
	b _0801C908
	.align 2, 0
_0801C374: .4byte 0x00004876
_0801C378: .4byte 0x0000488A
_0801C37C: .4byte 0xFFFFF00F
_0801C380:
	ldr r0, _0801C398 @ =0x00004876
	add r1, r4, r0
	mov r0, #0x15
	strh r0, [r1]
	ldr r1, _0801C39C @ =0x0000488A
	add r2, r4, r1
	ldr r0, _0801C3A0 @ =0xFFFFF00F
	ldrh r3, [r2]
	and r0, r3
	mov r1, #0xF0
	b _0801C908
	.align 2, 0
_0801C398: .4byte 0x00004876
_0801C39C: .4byte 0x0000488A
_0801C3A0: .4byte 0xFFFFF00F
_0801C3A4:
	ldr r0, _0801C3BC @ =0x00004876
	add r1, r4, r0
	mov r0, #0xB
	strh r0, [r1]
	ldr r1, _0801C3C0 @ =0x0000488A
	add r2, r4, r1
	ldr r0, _0801C3C4 @ =0xFFFFF00F
	ldrh r3, [r2]
	and r0, r3
	mov r1, #0xF0
	b _0801C908
	.align 2, 0
_0801C3BC: .4byte 0x00004876
_0801C3C0: .4byte 0x0000488A
_0801C3C4: .4byte 0xFFFFF00F
_0801C3C8:
	ldr r0, _0801C3E0 @ =0x00004876
	add r1, r4, r0
	ldr r0, _0801C3E4 @ =0x000001F9
	strh r0, [r1]
	ldr r1, _0801C3E8 @ =0x0000488A
	add r2, r4, r1
	ldr r0, _0801C3EC @ =0xFFFFF00F
	ldrh r3, [r2]
	and r0, r3
	mov r1, #0xF0
	b _0801C908
	.align 2, 0
_0801C3E0: .4byte 0x00004876
_0801C3E4: .4byte 0x000001F9
_0801C3E8: .4byte 0x0000488A
_0801C3EC: .4byte 0xFFFFF00F
_0801C3F0:
	ldr r0, _0801C408 @ =0x00004876
	add r1, r4, r0
	mov r0, #0xC
	strh r0, [r1]
	ldr r1, _0801C40C @ =0x0000488A
	add r2, r4, r1
	ldr r0, _0801C410 @ =0xFFFFF00F
	ldrh r3, [r2]
	and r0, r3
	mov r1, #0xF0
	b _0801C908
	.align 2, 0
_0801C408: .4byte 0x00004876
_0801C40C: .4byte 0x0000488A
_0801C410: .4byte 0xFFFFF00F
_0801C414:
	ldr r0, _0801C42C @ =0x00004876
	add r1, r4, r0
	mov r0, #0x16
	strh r0, [r1]
	ldr r1, _0801C430 @ =0x0000488A
	add r2, r4, r1
	ldr r0, _0801C434 @ =0xFFFFF00F
	ldrh r3, [r2]
	and r0, r3
	mov r1, #0xF0
	b _0801C908
	.align 2, 0
_0801C42C: .4byte 0x00004876
_0801C430: .4byte 0x0000488A
_0801C434: .4byte 0xFFFFF00F
_0801C438:
	ldr r0, _0801C450 @ =0x00004876
	add r1, r4, r0
	mov r0, #0x17
	strh r0, [r1]
	ldr r1, _0801C454 @ =0x0000488A
	add r2, r4, r1
	ldr r0, _0801C458 @ =0xFFFFF00F
	ldrh r3, [r2]
	and r0, r3
	mov r1, #0xF0
	b _0801C908
	.align 2, 0
_0801C450: .4byte 0x00004876
_0801C454: .4byte 0x0000488A
_0801C458: .4byte 0xFFFFF00F
_0801C45C:
	ldr r0, _0801C474 @ =0x00004876
	add r1, r4, r0
	mov r0, #4
	strh r0, [r1]
	ldr r1, _0801C478 @ =0x0000488A
	add r2, r4, r1
	ldr r0, _0801C47C @ =0xFFFFF00F
	ldrh r3, [r2]
	and r0, r3
	mov r1, #0xF0
	b _0801C908
	.align 2, 0
_0801C474: .4byte 0x00004876
_0801C478: .4byte 0x0000488A
_0801C47C: .4byte 0xFFFFF00F
_0801C480:
	ldr r0, _0801C498 @ =0x00004876
	add r1, r4, r0
	mov r0, #0xFD
	lsl r0, r0, #1
	strh r0, [r1]
	ldr r1, _0801C49C @ =0x0000488A
	add r2, r4, r1
	ldr r0, _0801C4A0 @ =0xFFFFF00F
	ldrh r3, [r2]
	and r0, r3
	mov r1, #0xF0
	b _0801C908
_0801C498: .4byte 0x00004876
_0801C49C: .4byte 0x0000488A
_0801C4A0: .4byte 0xFFFFF00F
_0801C4A4:
	ldr r0, _0801C4BC @ =0x00004876
	add r1, r4, r0
	mov r0, #0xFC
	lsl r0, r0, #1
	strh r0, [r1]
	ldr r1, _0801C4C0 @ =0x0000488A
	add r2, r4, r1
	ldr r0, _0801C4C4 @ =0xFFFFF00F
	ldrh r3, [r2]
	and r0, r3
	mov r1, #0xF0
	b _0801C908
_0801C4BC: .4byte 0x00004876
_0801C4C0: .4byte 0x0000488A
_0801C4C4: .4byte 0xFFFFF00F
_0801C4C8:
	ldr r0, _0801C4D0 @ =0x0000488A
	add r3, r4, r0
	b _0801C78A
	.align 2, 0
_0801C4D0: .4byte 0x0000488A
_0801C4D4:
	ldr r1, _0801C510 @ =0x00004859
	add r0, r4, r1
	mov r1, #0
	strb r1, [r0]
	ldr r2, _0801C514 @ =0x0000485A
	add r0, r4, r2
	strb r1, [r0]
	ldr r3, _0801C518 @ =0x0000485B
	add r0, r4, r3
	strb r1, [r0]
	ldr r1, _0801C51C @ =0x0000487C
	add r0, r4, r1
	ldr r1, [r0]
	mov r0, #0x80
	lsl r0, r0, #0x12
	cmp r1, r0
	bne _0801C4F8
	b _0801C5F8
_0801C4F8:
	cmp r1, r0
	bhi _0801C526
	mov r0, #0x80
	lsl r0, r0, #0x10
	cmp r1, r0
	beq _0801C54E
	cmp r1, r0
	bhi _0801C520
	mov r0, #0x80
	lsl r0, r0, #0xF
	b _0801C548
	.align 2, 0
_0801C510: .4byte 0x00004859
_0801C514: .4byte 0x0000485A
_0801C518: .4byte 0x0000485B
_0801C51C: .4byte 0x0000487C
_0801C520:
	mov r0, #0x80
	lsl r0, r0, #0x11
	b _0801C536
_0801C526:
	mov r0, #0x80
	lsl r0, r0, #0x14
	cmp r1, r0
	beq _0801C5F8
	cmp r1, r0
	bhi _0801C53C
	mov r0, #0x80
	lsl r0, r0, #0x13
_0801C536:
	cmp r1, r0
	beq _0801C5F8
	b _0801C92E
_0801C53C:
	mov r0, #0x80
	lsl r0, r0, #0x15
	cmp r1, r0
	beq _0801C602
	mov r0, #0x80
	lsl r0, r0, #0x16
_0801C548:
	cmp r1, r0
	beq _0801C602
	b _0801C92E
_0801C54E:
	ldr r2, _0801C568 @ =0x00004870
	add r0, r4, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1B
	sub r0, #0xB
	cmp r0, #4
	bhi _0801C5BA
	lsl r0, r0, #2
	ldr r1, _0801C56C @ =0x0801C570
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0801C568: .4byte 0x00004870
_0801C56C: .4byte 0x0801C570
_0801C570:
	.4byte _0801C584
	.4byte _0801C590
	.4byte _0801C59C
	.4byte _0801C5A8
	.4byte _0801C5B4
_0801C584:
	ldr r0, _0801C58C @ =0x00002AF9
	bl StartDialogue
	b _0801C5BA
_0801C58C: .4byte 0x00002AF9
_0801C590:
	ldr r0, _0801C598 @ =0x00002EE1
	bl StartDialogue
	b _0801C5BA
_0801C598: .4byte 0x00002EE1
_0801C59C:
	ldr r0, _0801C5A4 @ =0x000032C9
	bl StartDialogue
	b _0801C5BA
_0801C5A4: .4byte 0x000032C9
_0801C5A8:
	ldr r0, _0801C5B0 @ =0x000036B1
	bl StartDialogue
	b _0801C5BA
_0801C5B0: .4byte 0x000036B1
_0801C5B4:
	ldr r0, _0801C5E8 @ =0x00003A99
	bl StartDialogue
_0801C5BA:
	ldr r4, _0801C5EC @ =0x03000040
	ldr r3, _0801C5F0 @ =0x0000488A
	add r2, r4, r3
	ldr r0, _0801C5F4 @ =0xFFFFF00F
	ldrh r1, [r2]
	and r0, r1
	mov r3, #0xB0
	lsl r3, r3, #1
	add r1, r3, #0
	orr r0, r1
	strh r0, [r2]
	bl PickRandomOwnedRareCard
	mov r1, #0x91
	lsl r1, r1, #7
	add r4, r4, r1
	strh r0, [r4]
	ldrh r0, [r4]
	bl RemoveCardFromTrunk
	bl SaveGame
	b _0801C02E
_0801C5E8: .4byte 0x00003A99
_0801C5EC: .4byte 0x03000040
_0801C5F0: .4byte 0x0000488A
_0801C5F4: .4byte 0xFFFFF00F
_0801C5F8:
	ldr r0, _0801C620 @ =0x02011C20
	ldr r2, _0801C624 @ =0x0000215E
	add r0, r0, r2
	mov r1, #0
	strh r1, [r0]
_0801C602:
	mov r0, #0x96
	lsl r0, r0, #1
	bl StartDialogue
	ldr r2, _0801C628 @ =0x03000040
	ldr r3, _0801C62C @ =0x0000488A
	add r2, r2, r3
	ldr r0, _0801C630 @ =0xFFFFF00F
	ldrh r1, [r2]
	and r0, r1
	mov r3, #0xA0
	lsl r3, r3, #1
	add r1, r3, #0
	b _0801C908
	.align 2, 0
_0801C620: .4byte 0x02011C20
_0801C624: .4byte 0x0000215E
_0801C628: .4byte 0x03000040
_0801C62C: .4byte 0x0000488A
_0801C630: .4byte 0xFFFFF00F
_0801C634:
	ldr r0, _0801C658 @ =0x02011C20
	ldr r1, _0801C65C @ =0x00002164
	add r2, r0, r1
	ldrh r1, [r2]
	mov r0, #2
	and r0, r1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	cmp r3, #0
	beq _0801C668
	ldr r0, _0801C660 @ =0x0000FFFD
	and r0, r1
	strh r0, [r2]
	ldr r0, _0801C664 @ =0x0000015F
	bl StartDialogue
	b _0801C69E
	.align 2, 0
_0801C658: .4byte 0x02011C20
_0801C65C: .4byte 0x00002164
_0801C660: .4byte 0x0000FFFD
_0801C664: .4byte 0x0000015F
_0801C668:
	ldr r2, _0801C6A8 @ =0x03000040
	ldr r1, _0801C6AC @ =0x00004876
	add r0, r2, r1
	mov r1, #0
	strh r3, [r0]
	ldr r3, _0801C6B0 @ =0x00004859
	add r0, r2, r3
	strb r1, [r0]
	add r3, #1
	add r0, r2, r3
	strb r1, [r0]
	add r3, #1
	add r0, r2, r3
	strb r1, [r0]
	ldr r0, _0801C6B4 @ =0x0000488A
	add r2, r2, r0
	ldrh r3, [r2]
	lsl r1, r3, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801C6B8 @ =0xFFFFF00F
	and r0, r3
	orr r0, r1
	strh r0, [r2]
_0801C69E:
	ldr r2, _0801C6A8 @ =0x03000040
	ldr r1, _0801C6B4 @ =0x0000488A
	add r2, r2, r1
	b _0801C8F6
	.align 2, 0
_0801C6A8: .4byte 0x03000040
_0801C6AC: .4byte 0x00004876
_0801C6B0: .4byte 0x00004859
_0801C6B4: .4byte 0x0000488A
_0801C6B8: .4byte 0xFFFFF00F
_0801C6BC:
	bl CB_Bustup
	cmp r0, #0
	bne _0801C6C6
	b _0801C02E
_0801C6C6:
	ldr r2, _0801C6F8 @ =0x03000040
	ldr r3, _0801C6FC @ =0x0000488A
	add r4, r2, r3
	ldrh r3, [r4]
	lsl r1, r3, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801C700 @ =0xFFFFF00F
	and r0, r3
	orr r0, r1
	strh r0, [r4]
	ldr r1, _0801C704 @ =0x00004859
	add r0, r2, r1
	mov r1, #0
	strb r1, [r0]
	ldr r3, _0801C708 @ =0x0000485A
	add r0, r2, r3
	strb r1, [r0]
	ldr r0, _0801C70C @ =0x0000485B
	add r2, r2, r0
	strb r1, [r2]
	b _0801C02E
_0801C6F8: .4byte 0x03000040
_0801C6FC: .4byte 0x0000488A
_0801C700: .4byte 0xFFFFF00F
_0801C704: .4byte 0x00004859
_0801C708: .4byte 0x0000485A
_0801C70C: .4byte 0x0000485B
_0801C710:
	bl CB_GetPack
	b _0801C928
_0801C716:
	bl CB_Bustup
	cmp r0, #0
	bne _0801C720
	b _0801C02E
_0801C720:
	ldr r5, _0801C748 @ =0x03000040
	mov r1, #0x91
	lsl r1, r1, #7
	add r0, r5, r1
	ldrh r4, [r0]
	cmp r4, #0
	beq _0801C754
	add r0, r4, #0
	bl AddCardToTrunk
	bl SaveGame
	ldr r3, _0801C74C @ =0x0000488A
	add r2, r5, r3
	ldr r0, _0801C750 @ =0xFFFFF00F
	ldrh r1, [r2]
	and r0, r1
	mov r1, #0xC0
	b _0801C908
	.align 2, 0
_0801C748: .4byte 0x03000040
_0801C74C: .4byte 0x0000488A
_0801C750: .4byte 0xFFFFF00F
_0801C754:
	ldr r0, _0801C7A4 @ =0x08624CCE
	ldrh r0, [r0]
	bl RemoveCardFromTrunk
	ldr r0, _0801C7A8 @ =0x08624CD0
	ldrh r0, [r0]
	bl RemoveCardFromTrunk
	ldr r0, _0801C7AC @ =0x08624CD2
	ldrh r0, [r0]
	bl RemoveCardFromTrunk
	bl IncrementChampionshipWins
	bl SaveGame
	ldr r2, _0801C7B0 @ =0x00004859
	add r0, r5, r2
	strb r4, [r0]
	ldr r3, _0801C7B4 @ =0x0000485A
	add r0, r5, r3
	strb r4, [r0]
	ldr r1, _0801C7B8 @ =0x0000485B
	add r0, r5, r1
	strb r4, [r0]
	add r2, #0x31
	add r3, r5, r2
_0801C78A:
	ldrh r2, [r3]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801C7BC @ =0xFFFFF00F
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	b _0801C02E
	.align 2, 0
_0801C7A4: .4byte gCardNumberToId_TheMonarchy
_0801C7A8: .4byte gCardNumberToId_SetSailForTheKingdom
_0801C7AC: .4byte gCardNumberToId_GloryOfTheKingsHand
_0801C7B0: .4byte 0x00004859
_0801C7B4: .4byte 0x0000485A
_0801C7B8: .4byte 0x0000485B
_0801C7BC: .4byte 0xFFFFF00F
_0801C7C0:
	ldr r0, _0801C7F8 @ =0x000001FD
	bl GetRewardPack
	cmp r0, #0
	bne _0801C7CE
	bl _0801C02E @ far jump
_0801C7CE:
	ldr r2, _0801C7FC @ =0x03000040
	ldr r3, _0801C800 @ =0x00004859
	add r0, r2, r3
	mov r1, #0
	strb r1, [r0]
	add r3, #1
	add r0, r2, r3
	strb r1, [r0]
	add r3, #1
	add r0, r2, r3
	strb r1, [r0]
	ldr r0, _0801C804 @ =0x0000488A
	add r2, r2, r0
	ldr r0, _0801C808 @ =0xFFFFF00F
	ldrh r1, [r2]
	and r0, r1
	mov r3, #0xC8
	lsl r3, r3, #1
	add r1, r3, #0
	b _0801C908
	.align 2, 0
_0801C7F8: .4byte 0x000001FD
_0801C7FC: .4byte 0x03000040
_0801C800: .4byte 0x00004859
_0801C804: .4byte 0x0000488A
_0801C808: .4byte 0xFFFFF00F
_0801C80C:
	mov r1, #0x91
	lsl r1, r1, #7
	add r0, r4, r1
	ldrh r0, [r0]
	mov r1, #0
	mov r2, #0
	bl CardDetail_Init
	ldr r2, _0801C83C @ =0x0000488A
	add r3, r4, r2
	ldrh r2, [r3]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801C840 @ =0xFFFFF00F
	and r0, r2
	orr r0, r1
	strh r0, [r3]
_0801C836:
	bl CardDetail_Run
	b _0801C928
_0801C83C: .4byte 0x0000488A
_0801C840: .4byte 0xFFFFF00F
_0801C844:
	bl CB_Bustup
	cmp r0, #0
	bne _0801C850
	bl _0801C02E @ far jump
_0801C850:
	ldr r2, _0801C884 @ =0x03000040
	ldr r3, _0801C888 @ =0x00004859
	add r0, r2, r3
	mov r1, #0
	strb r1, [r0]
	add r3, #1
	add r0, r2, r3
	strb r1, [r0]
	add r3, #1
	add r0, r2, r3
	strb r1, [r0]
	ldr r0, _0801C88C @ =0x0000488A
	add r2, r2, r0
	ldrh r3, [r2]
	lsl r1, r3, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801C890 @ =0xFFFFF00F
	and r0, r3
	orr r0, r1
	strh r0, [r2]
	bl _0801C02E @ far jump
_0801C884: .4byte 0x03000040
_0801C888: .4byte 0x00004859
_0801C88C: .4byte 0x0000488A
_0801C890: .4byte 0xFFFFF00F
_0801C894:
	ldr r1, _0801C8A0 @ =0x00004876
	add r0, r4, r1
	ldrh r0, [r0]
	bl GetRewardPack
	b _0801C928
_0801C8A0: .4byte 0x00004876
_0801C8A4:
	mov r2, #0x91
	lsl r2, r2, #7
	add r0, r4, r2
	ldrh r0, [r0]
	mov r1, #0
	mov r2, #0
	bl CardDetail_Init
	ldr r0, _0801C910 @ =0x0000488A
	add r3, r4, r0
	ldrh r2, [r3]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801C914 @ =0xFFFFF00F
	and r0, r2
	orr r0, r1
	strh r0, [r3]
_0801C8CE:
	bl CardDetail_Run
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0801C8DC
	bl _0801C02E @ far jump
_0801C8DC:
	ldr r2, _0801C918 @ =0x03000040
	ldr r1, _0801C91C @ =0x00004859
	add r0, r2, r1
	mov r1, #0
	strb r1, [r0]
	ldr r3, _0801C920 @ =0x0000485A
	add r0, r2, r3
	strb r1, [r0]
	add r3, #1
	add r0, r2, r3
	strb r1, [r0]
	ldr r0, _0801C910 @ =0x0000488A
	add r2, r2, r0
_0801C8F6:
	ldrh r3, [r2]
	lsl r1, r3, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801C914 @ =0xFFFFF00F
	and r0, r3
_0801C908:
	orr r0, r1
	strh r0, [r2]
	bl _0801C02E @ far jump
_0801C910: .4byte 0x0000488A
_0801C914: .4byte 0xFFFFF00F
_0801C918: .4byte 0x03000040
_0801C91C: .4byte 0x00004859
_0801C920: .4byte 0x0000485A
_0801C924:
	bl CB_Bustup
_0801C928:
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _0801C930
_0801C92E:
	mov r0, #1
_0801C930:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end Campaign_GiveRewards
	.align 2, 0

