	thumb_func_start sub_08033E44
sub_08033E44: @ 0x08033E44
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldrb r1, [r0, #6]
	mov r8, r1
	ldrh r4, [r0, #6]
	lsr r2, r4, #8
	mov r3, #1
	and r3, r1
	mov r1, #0x94
	mul r1, r2
	ldr r2, _08033E98 @ =0x00000D64
	mul r2, r3
	add r1, r1, r2
	ldr r2, _08033E9C @ =0x0201930C
	add r1, r1, r2
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r4, r1, #0x14
	add r5, r4, #0
	mov r1, #4
	ldrb r0, [r0, #4]
	and r1, r0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	cmp r1, #0
	bne _08033F6C
	ldr r0, _08033EA0 @ =0x02017A40
	mov r6, #0xF8
	lsl r6, r6, #2
	add r3, r0, r6
	ldrb r2, [r3]
	add r6, r0, #0
	cmp r2, #0x7E
	beq _08033F20
	cmp r2, #0x7E
	bgt _08033EA4
	cmp r2, #0x7D
	beq _08033F40
	b _08033F6C
	.align 2, 0
_08033E98: .4byte 0x00000D64
_08033E9C: .4byte 0x0201930C
_08033EA0: .4byte 0x02017A40
_08033EA4:
	cmp r2, #0x7F
	beq _08033EBC
	cmp r2, #0x80
	bne _08033F6C
	cmp r4, #0
	beq _08033F6C
	ldr r2, _08033F10 @ =0x000003E1
	add r0, r6, r2
	strb r1, [r0]
	ldrb r0, [r3]
	sub r0, #1
	strb r0, [r3]
_08033EBC:
	ldr r4, _08033F10 @ =0x000003E1
	add r2, r6, r4
	ldr r3, _08033F14 @ =0x020192E4
	mov r0, #1
	mov r6, r8
	and r0, r6
	ldr r1, _08033F18 @ =0x00000D64
	mul r0, r1
	add r1, r0, r3
	ldrb r4, [r2]
	ldrb r6, [r1, #2]
	cmp r4, r6
	bcs _08033F0A
	add r4, r2, #0
	ldr r2, _08033F1C @ =0x00000684
	add r2, r2, r3
	mov r9, r2
	add r7, r0, #0
	add r6, r1, #0
_08033EE2:
	ldrb r1, [r4]
	lsl r0, r1, #2
	add r0, r0, r7
	add r0, r9
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r1, r5, #0
	bl sub_080074A0
	cmp r0, #0
	bne _08033F5C
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	ldrb r2, [r6, #2]
	cmp r0, r2
	bcc _08033EE2
_08033F0A:
	mov r0, #0x7E
	b _08033F6E
	.align 2, 0
_08033F10: .4byte 0x000003E1
_08033F14: .4byte 0x020192E4
_08033F18: .4byte 0x00000D64
_08033F1C: .4byte 0x00000684
_08033F20:
	ldr r0, _08033F38 @ =0x000007FF
	and r5, r0
	lsl r0, r5, #1
	ldr r4, _08033F3C @ =0x08622AB4
	add r0, r0, r4
	ldrh r1, [r0]
	mov r0, r8
	mov r2, #1
	bl sub_08019E0C
	mov r0, #0x7D
	b _08033F6E
_08033F38: .4byte 0x000007FF
_08033F3C: .4byte gUnk_08622AB4
_08033F40:
	mov r0, #0x60
	mov r6, r8
	cmp r6, #0
	beq _08033F4A
	ldr r0, _08033F58 @ =0x00008060
_08033F4A:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x78
	b _08033F6E
_08033F58: .4byte 0x00008060
_08033F5C:
	ldrb r1, [r4]
	mov r0, r8
	mov r2, #1
	mov r3, #1
	bl sub_080193D4
	mov r0, #0x7F
	b _08033F6E
_08033F6C:
	mov r0, #0
_08033F6E:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08033E44
	.align 2, 0

