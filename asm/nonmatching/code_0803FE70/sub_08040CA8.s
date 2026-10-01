	thumb_func_start sub_08040CA8
sub_08040CA8: @ 0x08040CA8
	push {r4, r5, r6, lr}
	sub sp, #0x100
	add r5, r0, #0
	ldr r0, _08040CF4 @ =0x02017A40
	ldr r1, _08040CF8 @ =0x000003E5
	add r4, r0, r1
	ldrb r0, [r4]
	cmp r0, #0
	bne _08040D14
	ldr r1, _08040CFC @ =0x08084930
	ldr r0, _08040D00 @ =0x08623E1E
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r3, _08040D04 @ =0x0822C720
	add r2, r2, r3
	mov r0, sp
	bl sub_080753F4
	ldr r2, _08040D08 @ =0x08084968
	mov r0, sp
	mov r1, sp
	bl sub_080753F4
	ldr r0, _08040D0C @ =0x00000206
	ldr r1, _08040D10 @ =0x00000712
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
	b _08040D7A
_08040CF4: .4byte 0x02017A40
_08040CF8: .4byte 0x000003E5
_08040CFC: .4byte gUnk_08084930
_08040D00: .4byte gUnk_08623E1E
_08040D04: .4byte gUnk_0822C720
_08040D08: .4byte gUnk_08084968
_08040D0C: .4byte 0x00000206
_08040D10: .4byte 0x00000712
_08040D14:
	ldr r1, _08040D28 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08040D2C
	mov r0, #0
	strb r0, [r4]
	b _08040D7C
	.align 2, 0
_08040D28: .4byte 0x03000040
_08040D2C:
	mov r0, #0xE0
	bl sub_08052F38
	cmp r0, #0
	beq _08040D7A
	ldr r0, _08040D6C @ =0x0201CFB0
	ldr r3, _08040D70 @ =0x00000824
	add r2, r0, r3
	add r3, #4
	add r1, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r1, [r1]
	ldr r0, [r0]
	add r4, r1, r0
	ldr r6, [r2]
	lsl r1, r4, #0x18
	lsr r1, r1, #0x10
	ldrb r2, [r2]
	orr r1, r2
	add r0, r5, #0
	bl sub_0802C334
	cmp r0, #0
	beq _08040D74
	add r0, r5, #0
	add r1, r6, #0
	add r2, r4, #0
	bl sub_0803DDAC
	mov r0, #1
	b _08040D7C
_08040D6C: .4byte 0x0201CFB0
_08040D70: .4byte 0x00000824
_08040D74:
	mov r0, #3
	bl sub_08077AEC
_08040D7A:
	mov r0, #0
_08040D7C:
	add sp, #0x100
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08040CA8

