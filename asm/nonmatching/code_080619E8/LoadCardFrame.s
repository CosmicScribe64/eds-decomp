	thumb_func_start LoadCardFrame
LoadCardFrame: @ 0x08061A1C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	ldr r0, _08061A50 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08061A54 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _08061A68
	cmp r0, #0x16
	bgt _08061A58
	cmp r0, #0x15
	beq _08061A5E
	b _08061A78
	.align 2, 0
_08061A50: .4byte 0x000007FF
_08061A54: .4byte gCardStats
_08061A58:
	cmp r0, #0x17
	beq _08061A70
	b _08061A78
_08061A5E:
	ldr r1, _08061A64 @ =0x08631558
	b _08061B36
	.align 2, 0
_08061A64: .4byte gCardFrameTrapGfx
_08061A68:
	ldr r1, _08061A6C @ =0x0862EEC0
	b _08061B36
_08061A6C: .4byte gCardFrameMagicGfx
_08061A70:
	ldr r1, _08061A74 @ =0x08633BF0
	b _08061B36
_08061A74: .4byte gCardFrameTicketGfx
_08061A78:
	ldr r0, _08061A90 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #1
	ldr r6, _08061A94 @ =0x08622AB4
	add r0, r0, r6
	ldrh r1, [r0]
	ldr r0, _08061A98 @ =0x00000776
	cmp r1, r0
	bne _08061A9C
	mov r0, #3
	b _08061AFE
	.align 2, 0
_08061A90: .4byte 0x000007FF
_08061A94: .4byte gCardIdToNumber
_08061A98: .4byte 0x00000776
_08061A9C:
	cmp r1, r0
	blt _08061AAC
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _08061AAC
	mov r0, #1
	b _08061AFE
_08061AAC:
	ldr r0, _08061AD0 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r7, _08061AD4 @ =0x08621DE0
	add r0, r0, r7
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _08061ADE
	cmp r0, #0x16
	bgt _08061AD8
	cmp r0, #0x15
	beq _08061AE2
	b _08061AEA
	.align 2, 0
_08061AD0: .4byte 0x000007FF
_08061AD4: .4byte gCardStats
_08061AD8:
	cmp r0, #0x17
	beq _08061AE6
	b _08061AEA
_08061ADE:
	mov r0, #7
	b _08061AFE
_08061AE2:
	mov r0, #8
	b _08061AFE
_08061AE6:
	mov r0, #9
	b _08061AFE
_08061AEA:
	ldr r0, _08061B0C @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r1, _08061B10 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_08061AFE:
	cmp r0, #2
	beq _08061B24
	cmp r0, #2
	bgt _08061B14
	cmp r0, #1
	beq _08061B1A
	b _08061B34
_08061B0C: .4byte 0x000007FF
_08061B10: .4byte gCardStats
_08061B14:
	cmp r0, #3
	beq _08061B2C
	b _08061B34
_08061B1A:
	ldr r1, _08061B20 @ =0x08627AF8
	b _08061B36
	.align 2, 0
_08061B20: .4byte gCardFrameEffectGfx
_08061B24:
	ldr r1, _08061B28 @ =0x0862A190
	b _08061B36
_08061B28: .4byte gCardFrameFusionGfx
_08061B2C:
	ldr r1, _08061B30 @ =0x0862C828
	b _08061B36
_08061B30: .4byte gCardFrameRitualGfx
_08061B34:
	ldr r1, _08061D18 @ =0x08625460
_08061B36:
	ldrh r2, [r1]
	lsl r0, r2, #1
	add r0, #0x10
	add r4, r1, r0
	ldr r0, _08061D1C @ =0x05000220
	add r1, #8
	mov r2, #0x40
	bl MemCopy16
	mov r7, #0
	mov r6, #0xFF
	lsl r6, r6, #8
	mov sl, r6
	mov r0, #0x10
	mov r9, r0
	lsl r1, r0, #8
	mov r8, r1
_08061B58:
	mov r5, #0
	add r2, r7, #2
	lsl r0, r2, #0x10
	lsr r0, r0, #0xB
	mov ip, r0
_08061B62:
	lsl r0, r5, #0x11
	lsr r0, r0, #0x10
	add r0, ip
	lsl r0, r0, #5
	ldr r6, _08061D20 @ =0x06010000
	add r2, r0, r6
	mov r3, #0
_08061B70:
	ldrh r0, [r4]
	add r1, r0, #0
	mov r6, sl
	and r0, r6
	cmp r0, #0
	beq _08061B84
	mov r6, r8
	add r0, r1, r6
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_08061B84:
	mov r0, #0xFF
	and r0, r1
	cmp r0, #0
	beq _08061B94
	mov r6, r9
	add r0, r1, r6
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_08061B94:
	strh r1, [r2]
	add r4, #2
	add r2, #2
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	cmp r3, #0x1F
	bls _08061B70
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	cmp r5, #0xC
	bls _08061B62
	add r0, r7, #1
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	cmp r7, #3
	bls _08061B58
	mov r7, #4
	mov r0, #0x10
	mov sl, r0
	mov r1, sl
	lsl r1, r1, #8
	str r1, [sp, #0]
_08061BC4:
	mov r5, #0
	add r2, r7, #2
	str r2, [sp, #4]
	lsl r0, r2, #0x10
	lsr r0, r0, #0xB
	mov ip, r0
_08061BD0:
	lsl r0, r5, #0x11
	lsr r0, r0, #0x10
	add r0, ip
	lsl r0, r0, #5
	ldr r6, _08061D20 @ =0x06010000
	add r2, r0, r6
	mov r3, #0
_08061BDE:
	ldrh r0, [r4]
	add r1, r0, #0
	mov r6, #0xFF
	lsl r6, r6, #8
	and r0, r6
	cmp r0, #0
	beq _08061BF4
	ldr r6, [sp, #0]
	add r0, r1, r6
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_08061BF4:
	mov r0, #0xFF
	and r0, r1
	cmp r0, #0
	beq _08061C04
	mov r6, sl
	add r0, r1, r6
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_08061C04:
	strh r1, [r2]
	add r4, #2
	add r2, #2
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	cmp r3, #0x1F
	bls _08061BDE
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	cmp r5, #1
	bls _08061BD0
	mov r5, #0
	ldr r1, [sp, #4]
	lsl r0, r1, #0x10
	lsr r0, r0, #0xB
	mov r8, r0
	mov r2, #0xFF
	lsl r2, r2, #8
	mov r9, r2
	mov r6, #0x10
	str r6, [sp, #4]
	lsl r6, r6, #8
	mov ip, r6
_08061C36:
	lsl r0, r5, #0x11
	mov r1, #0xB0
	lsl r1, r1, #0xD
	add r0, r0, r1
	lsr r0, r0, #0x10
	add r0, r8
	lsl r0, r0, #5
	ldr r6, _08061D20 @ =0x06010000
	add r2, r0, r6
	mov r3, #0
_08061C4A:
	ldrh r0, [r4]
	add r1, r0, #0
	mov r6, r9
	and r0, r6
	cmp r0, #0
	beq _08061C5E
	mov r6, ip
	add r0, r1, r6
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_08061C5E:
	mov r0, #0xFF
	and r0, r1
	cmp r0, #0
	beq _08061C6E
	ldr r6, [sp, #4]
	add r0, r1, r6
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_08061C6E:
	strh r1, [r2]
	add r4, #2
	add r2, #2
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	cmp r3, #0x1F
	bls _08061C4A
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	cmp r5, #1
	bls _08061C36
	add r0, r7, #1
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	cmp r7, #0xD
	bls _08061BC4
	mov r7, #0xE
	mov r0, #0xFF
	lsl r0, r0, #8
	mov r9, r0
	mov r1, #0x10
	mov ip, r1
	mov r2, ip
	lsl r2, r2, #8
	mov r8, r2
_08061CA4:
	mov r5, #0
	add r6, r7, #2
	add r7, #1
	mov sl, r7
	lsl r0, r6, #0x10
	lsr r6, r0, #0xB
_08061CB0:
	lsl r0, r5, #0x11
	lsr r0, r0, #0x10
	add r0, r0, r6
	lsl r0, r0, #5
	ldr r7, _08061D20 @ =0x06010000
	add r2, r0, r7
	mov r3, #0
	add r5, #1
_08061CC0:
	ldrh r0, [r4]
	add r1, r0, #0
	mov r7, r9
	and r0, r7
	cmp r0, #0
	beq _08061CD4
	mov r7, r8
	add r0, r1, r7
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_08061CD4:
	mov r0, #0xFF
	and r0, r1
	cmp r0, #0
	beq _08061CE4
	mov r7, ip
	add r0, r1, r7
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_08061CE4:
	strh r1, [r2]
	add r4, #2
	add r2, #2
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	cmp r3, #0x1F
	bls _08061CC0
	lsl r0, r5, #0x10
	lsr r5, r0, #0x10
	cmp r5, #0xC
	bls _08061CB0
	mov r1, sl
	lsl r0, r1, #0x10
	lsr r7, r0, #0x10
	cmp r7, #0x11
	bls _08061CA4
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08061D18: .4byte gCardFrameNormalGfx
_08061D1C: .4byte 0x05000220
_08061D20: .4byte 0x06010000
	thumb_func_end LoadCardFrame

