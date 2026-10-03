	thumb_func_start IsFusionMonster
IsFusionMonster: @ 0x08007994
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	ldr r2, _080079C4 @ =0x000007FF
	and r2, r3
	lsl r0, r2, #2
	ldr r1, _080079C8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08007A48
	lsl r0, r2, #1
	ldr r1, _080079CC @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _080079D0 @ =0x00000776
	cmp r1, r0
	bne _080079D4
	mov r0, #3
	b _08007A36
	.align 2, 0
_080079C4: .4byte 0x000007FF
_080079C8: .4byte gCardStats
_080079CC: .4byte gCardIdToNumber
_080079D0: .4byte 0x00000776
_080079D4:
	cmp r1, r0
	blt _080079E4
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _080079E4
	mov r0, #1
	b _08007A36
_080079E4:
	ldr r0, _08007A08 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08007A0C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _08007A16
	cmp r0, #0x16
	bgt _08007A10
	cmp r0, #0x15
	beq _08007A1A
	b _08007A22
	.align 2, 0
_08007A08: .4byte 0x000007FF
_08007A0C: .4byte gCardStats
_08007A10:
	cmp r0, #0x17
	beq _08007A1E
	b _08007A22
_08007A16:
	mov r0, #7
	b _08007A36
_08007A1A:
	mov r0, #8
	b _08007A36
_08007A1E:
	mov r0, #9
	b _08007A36
_08007A22:
	ldr r0, _08007A40 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08007A44 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_08007A36:
	cmp r0, #2
	bne _08007A48
	mov r0, #1
	b _08007A4A
	.align 2, 0
_08007A40: .4byte 0x000007FF
_08007A44: .4byte gCardStats
_08007A48:
	mov r0, #0
_08007A4A:
	bx lr
	thumb_func_end IsFusionMonster

