	thumb_func_start EffectAddGazelleFromDeckResolve
EffectAddGazelleFromDeckResolve: @ 0x080397F8
	push {r4, lr}
	sub sp, #0x80
	add r4, r0, #0
	ldr r0, _08039818 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08039880
	cmp r0, #0x7F
	bgt _0803981C
	cmp r0, #0x7E
	beq _08039890
	b _080398A4
	.align 2, 0
_08039818: .4byte 0x02017A40
_0803981C:
	cmp r0, #0x80
	bne _080398A4
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0x9B
	lsl r1, r1, #3
	mov r2, #0
	bl CollectEffectTargets
	cmp r0, #0
	beq _080398A4
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	cmp r0, #0
	bne _08039888
	ldr r1, _0803986C @ =0x08083430
	ldr r0, _08039870 @ =0x086243C8
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r0, _08039874 @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	bl FormatStr
	ldr r0, _08039878 @ =0x00000206
	ldr r1, _0803987C @ =0x00000712
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r0, #0x7F
	b _080398A6
	.align 2, 0
_0803986C: .4byte gStrAddFromDeckToHandPrompt
_08039870: .4byte gUnk_086243C8
_08039874: .4byte gCardNames
_08039878: .4byte 0x00000206
_0803987C: .4byte 0x00000712
_08039880:
	ldr r0, _0803988C @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _080398A4
_08039888:
	mov r0, #0x7E
	b _080398A6
_0803988C: .4byte 0x0201AE60
_08039890:
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _080398A0 @ =0x000002EA
	bl AddDeckCardToHand
	mov r0, #0x7D
	b _080398A6
_080398A0: .4byte 0x000002EA
_080398A4:
	mov r0, #0
_080398A6:
	add sp, #0x80
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectAddGazelleFromDeckResolve
	.align 2, 0

