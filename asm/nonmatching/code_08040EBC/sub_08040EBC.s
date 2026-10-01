	thumb_func_start sub_08040EBC
sub_08040EBC: @ 0x08040EBC
	push {r4, r5, lr}
	sub sp, #0x100
	add r5, r0, #0
	ldr r0, _08040F00 @ =0x02017A40
	ldr r1, _08040F04 @ =0x000003E5
	add r4, r0, r1
	ldrb r0, [r4]
	cmp r0, #0
	bne _08040F1C
	ldr r1, _08040F08 @ =0x080849B4
	ldr r0, _08040F0C @ =0x086248EE
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r0, _08040F10 @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	bl sub_080753F4
	ldr r0, _08040F14 @ =0x00000206
	ldr r1, _08040F18 @ =0x00000712
	mov r2, #0xB
	mov r3, sp
	bl sub_080602A4
	mov r0, #8
	neg r0, r0
	ldrb r1, [r5, #0xA]
	and r0, r1
	strb r0, [r5, #0xA]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _08040FA6
	.align 2, 0
_08040F00: .4byte 0x02017A40
_08040F04: .4byte 0x000003E5
_08040F08: .4byte gUnk_080849B4
_08040F0C: .4byte gUnk_086248EE
_08040F10: .4byte gUnk_0822C720
_08040F14: .4byte 0x00000206
_08040F18: .4byte 0x00000712
_08040F1C:
	mov r0, #0xE0
	bl sub_08052F38
	cmp r0, #0
	beq _08040FA6
	ldr r0, _08040F84 @ =0x0201CFB0
	ldr r2, _08040F88 @ =0x00000824
	add r1, r0, r2
	ldr r4, [r1]
	add r2, #4
	add r1, r0, r2
	add r2, #4
	add r0, r0, r2
	ldr r1, [r1]
	ldr r0, [r0]
	add r3, r1, r0
	mov r1, #1
	and r1, r4
	mov r0, #0x94
	add r2, r3, #0
	mul r2, r0
	ldr r0, _08040F8C @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _08040F90 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _08040FA0
	cmp r1, #0
	beq _08040FA0
	ldr r0, _08040F94 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _08040F98 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08040F9C @ =0x0000057D
	ldrh r0, [r0]
	cmp r0, r1
	bne _08040FA0
	add r0, r5, #0
	add r1, r4, #0
	add r2, r3, #0
	bl sub_0803DE40
	mov r0, #1
	b _08040FA8
_08040F84: .4byte 0x0201CFB0
_08040F88: .4byte 0x00000824
_08040F8C: .4byte 0x00000D64
_08040F90: .4byte 0x0201930C
_08040F94: .4byte 0x000007FF
_08040F98: .4byte gUnk_08622AB4
_08040F9C: .4byte 0x0000057D
_08040FA0:
	mov r0, #3
	bl sub_08077AEC
_08040FA6:
	mov r0, #0
_08040FA8:
	add sp, #0x100
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_08040EBC

