	thumb_func_start GameInit
GameInit: @ 0x08075DF4
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	mov r5, #0
	str r5, [sp, #0]
	ldr r4, _08075F08 @ =0x040000D4
	mov r0, sp
	str r0, [r4]
	mov r0, #0x80
	lsl r0, r0, #0x12
	str r0, [r4, #4]
	ldr r0, _08075F0C @ =0x85010000
	str r0, [r4, #8]
	ldr r0, [r4, #8]
	str r5, [sp, #0]
	mov r1, sp
	str r1, [r4]
	mov r0, #0xC0
	lsl r0, r0, #0x12
	str r0, [r4, #4]
	ldr r0, _08075F10 @ =0x85001E80
	str r0, [r4, #8]
	ldr r0, [r4, #8]
	mov r0, #0xE0
	lsl r0, r0, #0x14
	ldr r1, _08075F14 @ =0x02011C20
	ldr r2, _08075F18 @ =0x00002170
	bl ReadSram
	ldr r0, _08075F1C @ =0x03000000
	str r5, [r0]
	str r5, [r0, #4]
	ldr r1, _08075F20 @ =0x0807569D
	str r1, [r0, #8]
	str r5, [r0, #0xC]
	str r5, [r0, #0x10]
	str r5, [r0, #0x14]
	ldr r1, _08075F24 @ =0x0807570D
	str r1, [r0, #0x18]
	str r5, [r0, #0x1C]
	str r5, [r0, #0x20]
	ldr r1, _08075F28 @ =0x0807E325
	str r1, [r0, #0x24]
	str r5, [r0, #0x28]
	str r5, [r0, #0x2C]
	str r5, [r0, #0x30]
	ldr r1, _08075F2C @ =0x08075741
	str r1, [r0, #0x34]
	str r5, [r0, #0x38]
	str r5, [r0, #0x3C]
	ldr r0, _08075F30 @ =0x080000FC
	str r0, [r4]
	ldr r6, _08075F34 @ =0x0300004C
	str r6, [r4, #4]
	ldr r0, _08075F38 @ =0x80000200
	str r0, [r4, #8]
	ldr r0, [r4, #8]
	ldr r0, _08075F3C @ =0x03007FFC
	str r6, [r0]
	ldr r4, _08075F40 @ =0x04000208
	strh r5, [r4]
	ldr r2, _08075F44 @ =0x04000200
	mov r3, #1
	strh r3, [r2]
	ldrh r0, [r2]
	mov r7, #0x80
	lsl r7, r7, #2
	add r1, r7, #0
	orr r0, r1
	strh r0, [r2]
	ldrh r0, [r2]
	mov r7, #0x80
	lsl r7, r7, #3
	add r1, r7, #0
	orr r0, r1
	strh r0, [r2]
	ldrh r0, [r2]
	mov r7, #0x80
	lsl r7, r7, #6
	add r1, r7, #0
	orr r0, r1
	strh r0, [r2]
	ldrh r0, [r2]
	mov r1, #0x20
	orr r0, r1
	strh r0, [r2]
	ldr r2, _08075F48 @ =0x04000004
	mov r0, #8
	strh r0, [r2]
	ldrh r0, [r2]
	mov r1, #0x10
	orr r0, r1
	strh r0, [r2]
	strh r3, [r4]
	ldr r1, _08075F4C @ =0x04000204
	ldr r2, _08075F50 @ =0x00004014
	add r0, r2, #0
	strh r0, [r1]
	ldr r7, _08075F54 @ =0x00000404
	add r1, r6, r7
	ldr r0, _08075F58 @ =0x08004EAD
	str r0, [r1]
	mov r1, #0x81
	lsl r1, r1, #3
	add r0, r6, r1
	str r5, [r0]
	mov r0, #0
	bl SoundInit
	ldr r1, _08075F5C @ =0x04000108
	mov r2, #0xF4
	lsl r2, r2, #8
	add r0, r2, #0
	strh r0, [r1]
	ldr r2, _08075F60 @ =0x0400010A
	ldrh r0, [r2]
	mov r1, #0xC3
	orr r0, r1
	strh r0, [r2]
	sub r7, #2
	add r6, r6, r7
	mov r0, #3
	strh r0, [r6]
	bl ResetBgScroll
	bl SetBrightnessWhite
	mov r0, #1
	bl SetSeEnabled
	mov r0, #1
	bl SetBgmEnabled
	bl SetTextModeLatin
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08075F08: .4byte 0x040000D4
_08075F0C: .4byte 0x85010000
_08075F10: .4byte 0x85001E80
_08075F14: .4byte 0x02011C20
_08075F18: .4byte 0x00002170
_08075F1C: .4byte 0x03000000
_08075F20: .4byte VBlankIntr
_08075F24: .4byte Timer2Intr
_08075F28: .4byte SoundDma1Intr
_08075F2C: .4byte GamepakIntr
_08075F30: .4byte 0x080000FC
_08075F34: .4byte 0x0300004C
_08075F38: .4byte 0x80000200
_08075F3C: .4byte 0x03007FFC
_08075F40: .4byte 0x04000208
_08075F44: .4byte 0x04000200
_08075F48: .4byte 0x04000004
_08075F4C: .4byte 0x04000204
_08075F50: .4byte 0x00004014
_08075F54: .4byte 0x00000404
_08075F58: .4byte CB_License
_08075F5C: .4byte 0x04000108
_08075F60: .4byte 0x0400010A
	thumb_func_end GameInit

