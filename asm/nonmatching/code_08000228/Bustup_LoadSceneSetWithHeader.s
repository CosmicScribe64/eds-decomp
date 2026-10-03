	thumb_func_start Bustup_LoadSceneSetWithHeader
Bustup_LoadSceneSetWithHeader: @ 0x08000570
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	add r7, r0, #0
	mov r8, r1
	add r6, r2, #0
	mov r9, r3
	ldr r0, [sp, #0x20]
	lsl r0, r0, #0x18
	ldr r1, _08000638 @ =0x08139F5C
	lsr r0, r0, #0x16
	add r0, r0, r1
	ldr r1, [r0]
	add r0, r1, #4
	ldr r4, _0800063C @ =0x0203A614
	ldrh r3, [r1, #2]
	lsl r2, r3, #0x10
	ldrh r1, [r1]
	orr r2, r1
	add r1, r4, #0
	bl LZSSDecompress
	str r4, [r6, #0xC]
	ldr r1, _08000640 @ =0x06005A00
	mov r5, #0xF0
	lsl r5, r5, #5
	add r0, r4, #0
	add r2, r5, #0
	bl CpuSet
	ldr r1, _08000644 @ =0x0600FA00
	add r0, r4, #0
	add r2, r5, #0
	bl CpuSet
	ldr r0, _08000648 @ =0x0874D5B4
	ldr r5, _0800064C @ =0x02031014
	ldr r3, _08000650 @ =0x0874D5B0
	ldr r1, _08000654 @ =0x0874D5B2
	ldrh r1, [r1]
	lsl r2, r1, #0x10
	ldrh r3, [r3]
	orr r2, r3
	add r1, r5, #0
	bl LZSSDecompress
	mov r1, #0xC0
	lsl r1, r1, #0x13
	mov r4, #0xF0
	lsl r4, r4, #3
	add r0, r5, #0
	add r2, r4, #0
	bl CpuSet
	ldr r1, _08000658 @ =0x0600A000
	add r0, r5, #0
	add r2, r4, #0
	bl CpuSet
	ldr r1, [r7]
	add r0, r1, #4
	ldrh r3, [r1, #2]
	lsl r2, r3, #0x10
	ldrh r1, [r1]
	orr r2, r1
	add r1, r5, #0
	bl LZSSDecompress
	str r5, [r6]
	ldr r1, _0800065C @ =0x06000F00
	mov r4, #0x96
	lsl r4, r4, #6
	add r0, r5, #0
	add r2, r4, #0
	bl CpuSet
	ldr r0, [r6]
	ldr r1, _08000660 @ =0x0600AF00
	add r2, r4, #0
	bl CpuSet
	ldr r1, [r7, #0xC]
	cmp r1, #0
	beq _08000668
	add r0, r1, #4
	ldrh r3, [r1, #2]
	lsl r2, r3, #0x10
	ldrh r1, [r1]
	orr r2, r1
	add r1, r5, #0
	bl LZSSDecompress
	ldr r1, _08000664 @ =0x06014000
	add r0, r5, #0
	mov r2, #0x10
	bl CopyTileSheetTo2D
	b _08000674
_08000638: .4byte gDialogueBoxGfx
_0800063C: .4byte 0x0203A614
_08000640: .4byte 0x06005A00
_08000644: .4byte 0x0600FA00
_08000648: .4byte gDialogueHeaderLzData
_0800064C: .4byte 0x02031014
_08000650: .4byte gDialogueHeaderLz
_08000654: .4byte gDialogueHeaderLzSizeHi
_08000658: .4byte 0x0600A000
_0800065C: .4byte 0x06000F00
_08000660: .4byte 0x0600AF00
_08000664: .4byte 0x06014000
_08000668:
	mov r0, sp
	strh r1, [r0]
	ldr r1, _080006E8 @ =0x06014000
	ldr r2, _080006EC @ =0x01002000
	bl CpuSet
_08000674:
	mov r0, r8
	str r0, [r6, #0x14]
	mov r4, #0xA0
	lsl r4, r4, #0x13
	ldr r1, _080006F0 @ =0x0874E104
	add r0, r4, #0
	mov r2, #0x40
	bl MemCopy16
	ldr r0, [r7, #4]
	str r0, [r6, #4]
	cmp r0, #0
	beq _08000698
	add r0, #0x20
	ldr r1, _080006F4 @ =0x05000020
	mov r2, #0xF8
	bl CpuSet
_08000698:
	ldr r0, [r7, #8]
	str r0, [r6, #8]
	cmp r0, #0
	beq _080006AA
	ldr r1, _080006F8 @ =0x05000200
	mov r2, #0x80
	lsl r2, r2, #1
	bl CpuSet
_080006AA:
	ldr r0, [r7, #0x10]
	str r0, [r6, #0x10]
	cmp r0, #0
	beq _080006CE
	mov r1, r9
	bl AnimBlockInit
	ldr r2, _080006FC @ =0x02013DE0
	ldr r1, _08000700 @ =0x000009A4
	add r2, r2, r1
	mov r1, #7
	and r0, r1
	mov r1, #8
	neg r1, r1
	ldrb r3, [r2]
	and r1, r3
	orr r1, r0
	strb r1, [r2]
_080006CE:
	ldr r1, _08000704 @ =0x0874E304
	add r0, r4, #0
	mov r2, #0x20
	bl MemCopy16
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080006E8: .4byte 0x06014000
_080006EC: .4byte 0x01002000
_080006F0: .4byte gDialogueBoxPalette
_080006F4: .4byte 0x05000020
_080006F8: .4byte 0x05000200
_080006FC: .4byte 0x02013DE0
_08000700: .4byte 0x000009A4
_08000704: .4byte gDialogueTextPalette
	thumb_func_end Bustup_LoadSceneSetWithHeader

