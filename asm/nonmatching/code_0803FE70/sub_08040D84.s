	thumb_func_start sub_08040D84
sub_08040D84: @ 0x08040D84
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x100
	add r6, r0, #0
	ldr r0, _08040E00 @ =0x02017A40
	ldr r1, _08040E04 @ =0x000003E5
	add r2, r0, r1
	ldrb r0, [r2]
	cmp r0, #0
	bne _08040E10
	mov r0, #8
	neg r0, r0
	ldrb r2, [r6, #0xA]
	and r0, r2
	strb r0, [r6, #0xA]
	mov r5, #0
	mov r4, #1
	ldr r0, _08040E08 @ =0x00000D64
	mov r8, r0
	ldr r7, _08040E0C @ =0x0201930C
_08040DAE:
	ldrb r1, [r6, #2]
	lsl r2, r1, #0x1F
	mov r0, #0x94
	add r3, r5, #0
	mul r3, r0
	lsr r0, r2, #0x1F
	sub r0, r4, r0
	and r0, r4
	mov r1, r8
	mul r1, r0
	add r1, r3, r1
	add r1, r1, r7
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08040DF6
	lsr r0, r2, #0x1F
	sub r0, r4, r0
	and r0, r4
	mov r1, r8
	mul r1, r0
	add r0, r1, #0
	add r0, r3, r0
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08040DF6
	lsr r0, r2, #0x1F
	sub r0, r4, r0
	add r1, r5, #0
	bl sub_0800C8BC
	cmp r0, #7
	beq _08040E6C
_08040DF6:
	add r5, #1
	cmp r5, #4
	ble _08040DAE
	mov r0, #1
	b _08040EB0
_08040E00: .4byte 0x02017A40
_08040E04: .4byte 0x000003E5
_08040E08: .4byte 0x00000D64
_08040E0C: .4byte 0x0201930C
_08040E10:
	ldr r1, _08040E24 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08040E28
	mov r0, #0
	strb r0, [r2]
	b _08040EB0
	.align 2, 0
_08040E24: .4byte 0x03000040
_08040E28:
	mov r0, #0xE0
	lsl r0, r0, #0x10
	bl sub_08052F38
	cmp r0, #0
	beq _08040EAE
	ldr r0, _08040E64 @ =0x0201CFB0
	ldr r2, _08040E68 @ =0x00000824
	add r1, r0, r2
	ldr r5, [r1]
	add r2, #4
	add r1, r0, r2
	add r2, #4
	add r0, r0, r2
	ldr r1, [r1]
	ldr r0, [r0]
	add r4, r1, r0
	add r0, r5, #0
	add r1, r4, #0
	bl sub_0800C8BC
	cmp r0, #7
	bne _08040EA8
	add r0, r6, #0
	add r1, r5, #0
	add r2, r4, #0
	bl sub_0803DDAC
	mov r0, #1
	b _08040EB0
_08040E64: .4byte 0x0201CFB0
_08040E68: .4byte 0x00000824
_08040E6C:
	ldr r1, _08040E90 @ =0x08084970
	ldr r2, _08040E94 @ =0x080849AC
	mov r0, sp
	bl sub_080753F4
	ldr r0, _08040E98 @ =0x00000206
	ldr r1, _08040E9C @ =0x00000712
	mov r2, #0xB
	mov r3, sp
	bl sub_080602A4
	ldr r0, _08040EA0 @ =0x02017A40
	ldr r1, _08040EA4 @ =0x000003E5
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _08040EAE
_08040E90: .4byte gUnk_08084970
_08040E94: .4byte gUnk_080849AC
_08040E98: .4byte 0x00000206
_08040E9C: .4byte 0x00000712
_08040EA0: .4byte 0x02017A40
_08040EA4: .4byte 0x000003E5
_08040EA8:
	mov r0, #3
	bl sub_08077AEC
_08040EAE:
	mov r0, #0
_08040EB0:
	add sp, #0x100
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08040D84

