	thumb_func_start sub_0803F69C
sub_0803F69C: @ 0x0803F69C
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r0, _0803F6CC @ =0x02017A40
	ldr r1, _0803F6D0 @ =0x000003E5
	add r5, r0, r1
	ldrb r0, [r5]
	cmp r0, #0
	bne _0803F6E0
	mov r0, #8
	neg r0, r0
	ldrb r2, [r4, #0xA]
	and r0, r2
	strb r0, [r4, #0xA]
	ldr r0, _0803F6D4 @ =0x00000206
	ldr r1, _0803F6D8 @ =0x00000712
	ldr r3, _0803F6DC @ =0x080842CC
	mov r2, #0xB
	bl sub_080602A4
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
	b _0803F72A
_0803F6CC: .4byte 0x02017A40
_0803F6D0: .4byte 0x000003E5
_0803F6D4: .4byte 0x00000206
_0803F6D8: .4byte 0x00000712
_0803F6DC: .4byte gUnk_080842CC
_0803F6E0:
	ldr r1, _0803F6F4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803F6F8
	mov r0, #0
	strb r0, [r5]
	b _0803F72A
	.align 2, 0
_0803F6F4: .4byte 0x03000040
_0803F6F8:
	ldr r0, _0803F708 @ =0x00900090
	bl sub_08052F38
	cmp r0, #0
	bne _0803F70C
	mov r0, #0
	b _0803F72A
	.align 2, 0
_0803F708: .4byte 0x00900090
_0803F70C:
	ldr r0, _0803F730 @ =0x0201CFB0
	ldr r3, _0803F734 @ =0x00000824
	add r1, r0, r3
	ldr r1, [r1]
	add r3, #4
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r4, #0
	bl sub_0803DDAC
	mov r0, #1
_0803F72A:
	pop {r4, r5}
	pop {r1}
	bx r1
_0803F730: .4byte 0x0201CFB0
_0803F734: .4byte 0x00000824
	thumb_func_end sub_0803F69C

