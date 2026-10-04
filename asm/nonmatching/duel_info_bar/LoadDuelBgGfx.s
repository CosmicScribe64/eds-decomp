	thumb_func_start LoadDuelBgGfx
LoadDuelBgGfx: @ 0x0806075C
	push {r4, lr}
	bl LoadSystemGfx
	ldr r0, _0806082C @ =0x05000020
	ldr r1, _08060830 @ =0x0867793C
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _08060834 @ =0x05000040
	ldr r1, _08060838 @ =0x0867795C
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _0806083C @ =0x05000060
	ldr r1, _08060840 @ =0x0867E4BC
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _08060844 @ =0x05000080
	ldr r1, _08060848 @ =0x0867EE3C
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _0806084C @ =0x05000100
	ldr r1, _08060850 @ =0x0868147C
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _08060854 @ =0x06004E00
	ldr r1, _08060858 @ =0x0867817C
	mov r4, #0x80
	lsl r4, r4, #4
	add r2, r4, #0
	bl CopyDoubleWords
	ldr r0, _0806085C @ =0x06005600
	ldr r1, _08060860 @ =0x0867897C
	add r2, r4, #0
	bl CopyDoubleWords
	ldr r0, _08060864 @ =0x06005E00
	ldr r1, _08060868 @ =0x0867917C
	add r2, r4, #0
	bl CopyDoubleWords
	ldr r0, _0806086C @ =0x06006600
	ldr r1, _08060870 @ =0x0867997C
	add r2, r4, #0
	bl CopyDoubleWords
	ldr r0, _08060874 @ =0x06006E00
	ldr r1, _08060878 @ =0x0867A17C
	add r2, r4, #0
	bl CopyDoubleWords
	ldr r0, _0806087C @ =0x06007600
	ldr r1, _08060880 @ =0x0867B17C
	add r2, r4, #0
	bl CopyDoubleWords
	ldr r0, _08060884 @ =0x06007E00
	ldr r1, _08060888 @ =0x0867A97C
	add r2, r4, #0
	bl CopyDoubleWords
	ldr r0, _0806088C @ =0x06008600
	ldr r1, _08060890 @ =0x0867B97C
	mov r4, #0x80
	lsl r4, r4, #2
	add r2, r4, #0
	bl CopyDoubleWords
	ldr r0, _08060894 @ =0x06008800
	ldr r1, _08060898 @ =0x08684EFC
	add r2, r4, #0
	bl CopyDoubleWords
	ldr r0, _0806089C @ =0x06008880
	ldr r1, _080608A0 @ =0x0867E6BC
	mov r2, #0xF0
	lsl r2, r2, #3
	bl CopyDoubleWords
	ldr r0, _080608A4 @ =0x06009000
	ldr r1, _080608A8 @ =0x0867EE5C
	mov r2, #0x40
	bl CopyDoubleWords
	ldr r0, _080608AC @ =0x06009040
	ldr r1, _080608B0 @ =0x0867EE9C
	mov r2, #0xC0
	lsl r2, r2, #1
	bl CopyDoubleWords
	ldr r0, _080608B4 @ =0x060099C0
	ldr r1, _080608B8 @ =0x0868149C
	mov r2, #0x90
	lsl r2, r2, #1
	bl CopyDoubleWords
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0806082C: .4byte 0x05000020
_08060830: .4byte gHandCursorPal
_08060834: .4byte 0x05000040
_08060838: .4byte gCardIconPal
_0806083C: .4byte 0x05000060
_08060840: .4byte gDuelDigitsPal
_08060844: .4byte 0x05000080
_08060848: .4byte gPhaseIndicatorPal
_0806084C: .4byte 0x05000100
_08060850: .4byte gTextBoxPal
_08060854: .4byte 0x06004E00
_08060858: .4byte gCardIconBackGfx
_0806085C: .4byte 0x06005600
_08060860: .4byte gCardIconNormalGfx
_08060864: .4byte 0x06005E00
_08060868: .4byte gCardIconEffectGfx
_0806086C: .4byte 0x06006600
_08060870: .4byte gCardIconFusionGfx
_08060874: .4byte 0x06006E00
_08060878: .4byte gCardIconRitualGfx
_0806087C: .4byte 0x06007600
_08060880: .4byte gCardIconMagicGfx
_08060884: .4byte 0x06007E00
_08060888: .4byte gCardIconTrapGfx
_0806088C: .4byte 0x06008600
_08060890: .4byte gThickPileGfx
_08060894: .4byte 0x06008800
_08060898: .4byte gHeldZoneMarkGfx
_0806089C: .4byte 0x06008880
_080608A0: .4byte gDuelDigitsGfx
_080608A4: .4byte 0x06009000
_080608A8: .4byte gLpLabelGfx
_080608AC: .4byte 0x06009040
_080608B0: .4byte gPhaseIndicatorGfx
_080608B4: .4byte 0x060099C0
_080608B8: .4byte gTextBoxFrameGfx
	thumb_func_end LoadDuelBgGfx

