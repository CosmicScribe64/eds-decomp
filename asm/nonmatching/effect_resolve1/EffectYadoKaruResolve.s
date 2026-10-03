	thumb_func_start EffectYadoKaruResolve
EffectYadoKaruResolve: @ 0x08031AEC
	push {r4, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _08031BC0
	ldrb r3, [r4, #2]
	mov r1, #1
	add r0, r1, #0
	and r0, r3
	cmp r0, #0
	bne _08031BC0
	ldr r0, _08031B20 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08031B6C
	cmp r0, #0x7F
	bgt _08031B24
	cmp r0, #0x7E
	beq _08031B94
	b _08031BC0
	.align 2, 0
_08031B20: .4byte 0x02017A40
_08031B24:
	cmp r0, #0x80
	bne _08031BC0
	ldr r2, _08031B58 @ =0x020192E4
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	and r1, r0
	ldr r0, _08031B5C @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	cmp r0, #0
	beq _08031BC0
	ldr r0, _08031B60 @ =0x00000205
	ldr r1, _08031B64 @ =0x00000914
	ldr r3, _08031B68 @ =0x08082B10
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r0, #0x7F
	b _08031BC2
	.align 2, 0
_08031B58: .4byte 0x020192E4
_08031B5C: .4byte 0x00000D64
_08031B60: .4byte 0x00000205
_08031B64: .4byte 0x00000914
_08031B68: .4byte gStrYadoKaruReturnPrompt
_08031B6C:
	ldr r0, _08031B84 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _08031BC0
	ldr r0, _08031B88 @ =0x00000205
	ldr r1, _08031B8C @ =0x00000914
	ldr r3, _08031B90 @ =0x08082B58
	mov r2, #0xB
	bl TextBoxOpen
_08031B80:
	mov r0, #0x7E
	b _08031BC2
_08031B84: .4byte 0x0201AE60
_08031B88: .4byte 0x00000205
_08031B8C: .4byte 0x00000914
_08031B90: .4byte gStrYadoKaruSelectPrompt
_08031B94:
	mov r0, #1
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _08031B80
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _08031BB8 @ =0x0201CFB0
	ldr r2, _08031BBC @ =0x0000082C
	add r1, r1, r2
	ldr r1, [r1]
	mov r2, #0
	bl ReturnHandCardToDeck
	mov r0, #0x80
	b _08031BC2
	.align 2, 0
_08031BB8: .4byte 0x0201CFB0
_08031BBC: .4byte 0x0000082C
_08031BC0:
	mov r0, #0
_08031BC2:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectYadoKaruResolve

