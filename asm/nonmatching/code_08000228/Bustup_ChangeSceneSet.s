	thumb_func_start Bustup_ChangeSceneSet
Bustup_ChangeSceneSet: @ 0x08000708
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	add r7, r0, #0
	add r6, r2, #0
	mov r9, r3
	ldr r4, [sp, #0x20]
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	ldr r1, [r7]
	add r0, r1, #4
	ldr r2, _08000798 @ =0x02031014
	mov r8, r2
	ldrh r3, [r1, #2]
	lsl r2, r3, #0x10
	ldrh r1, [r1]
	orr r2, r1
	mov r1, r8
	bl LZSSDecompress
	mov r0, r8
	str r0, [r6]
	mov r5, #0xB4
	lsl r5, r5, #7
	mov r0, #0
	add r1, r6, #0
	add r2, r5, #0
	bl CopyBitmapToPage
	mov r0, #1
	add r1, r6, #0
	add r2, r5, #0
	bl CopyBitmapToPage
	ldr r0, _0800079C @ =0x08139F5C
	lsl r4, r4, #2
	add r4, r4, r0
	ldr r1, [r4]
	add r0, r1, #4
	ldr r4, _080007A0 @ =0x0203A614
	ldrh r3, [r1, #2]
	lsl r2, r3, #0x10
	ldrh r1, [r1]
	orr r2, r1
	add r1, r4, #0
	bl LZSSDecompress
	str r4, [r6, #0xC]
	ldr r0, [r7, #4]
	str r0, [r6, #4]
	ldr r0, [r7, #8]
	str r0, [r6, #8]
	ldr r1, [r7, #0xC]
	cmp r1, #0
	beq _080007A8
	add r0, r1, #4
	ldrh r3, [r1, #2]
	lsl r2, r3, #0x10
	ldrh r1, [r1]
	orr r2, r1
	mov r1, r8
	bl LZSSDecompress
	ldr r1, _080007A4 @ =0x06014000
	mov r0, r8
	mov r2, #0x10
	bl CopyTileSheetTo2D
	b _080007B4
	.align 2, 0
_08000798: .4byte 0x02031014
_0800079C: .4byte gDialogueBoxGfx
_080007A0: .4byte 0x0203A614
_080007A4: .4byte 0x06014000
_080007A8:
	mov r0, sp
	strh r1, [r0]
	ldr r1, _0800081C @ =0x06014000
	ldr r2, _08000820 @ =0x01002000
	bl CpuSet
_080007B4:
	ldr r0, [r7, #0x10]
	str r0, [r6, #0x10]
	ldr r0, [r7, #4]
	str r0, [r6, #4]
	cmp r0, #0
	beq _080007CA
	add r0, #0x20
	ldr r1, _08000824 @ =0x05000020
	mov r2, #0xF8
	bl CpuSet
_080007CA:
	ldr r0, [r7, #8]
	str r0, [r6, #8]
	cmp r0, #0
	beq _080007DC
	ldr r1, _08000828 @ =0x05000200
	mov r2, #0x80
	lsl r2, r2, #1
	bl CpuSet
_080007DC:
	ldr r0, [r7, #0x10]
	str r0, [r6, #0x10]
	cmp r0, #0
	beq _08000800
	mov r1, r9
	bl AnimBlockInit
	ldr r2, _0800082C @ =0x02013DE0
	ldr r1, _08000830 @ =0x000009A4
	add r2, r2, r1
	mov r1, #7
	and r0, r1
	mov r1, #8
	neg r1, r1
	ldrb r3, [r2]
	and r1, r3
	orr r1, r0
	strb r1, [r2]
_08000800:
	mov r0, #0xA0
	lsl r0, r0, #0x13
	ldr r1, _08000834 @ =0x0874E304
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
_0800081C: .4byte 0x06014000
_08000820: .4byte 0x01002000
_08000824: .4byte 0x05000020
_08000828: .4byte 0x05000200
_0800082C: .4byte 0x02013DE0
_08000830: .4byte 0x000009A4
_08000834: .4byte gDialogueTextPalette
	thumb_func_end Bustup_ChangeSceneSet

