	thumb_func_start sub_0803EDC4
sub_0803EDC4: @ 0x0803EDC4
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r0, _0803EDF4 @ =0x02017A40
	ldr r1, _0803EDF8 @ =0x000003E5
	add r5, r0, r1
	ldrb r0, [r5]
	cmp r0, #0
	bne _0803EE08
	ldr r0, _0803EDFC @ =0x00000206
	ldr r1, _0803EE00 @ =0x00000712
	ldr r3, _0803EE04 @ =0x0808405C
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #8
	neg r0, r0
	ldrb r2, [r4, #0xA]
	and r0, r2
	strb r0, [r4, #0xA]
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	b _0803EE62
	.align 2, 0
_0803EDF4: .4byte 0x02017A40
_0803EDF8: .4byte 0x000003E5
_0803EDFC: .4byte 0x00000206
_0803EE00: .4byte 0x00000712
_0803EE04: .4byte gUnk_0808405C
_0803EE08:
	ldr r1, _0803EE1C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803EE20
	mov r0, #0
	strb r0, [r5]
	b _0803EE64
	.align 2, 0
_0803EE1C: .4byte 0x03000040
_0803EE20:
	mov r0, #0xD2
	lsl r0, r0, #0x10
	bl sub_08052F38
	cmp r0, #0
	beq _0803EE62
	ldr r0, _0803EE54 @ =0x0201CFB0
	ldr r3, _0803EE58 @ =0x00000824
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
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803EE5C
	mov r0, #1
	b _0803EE64
	.align 2, 0
_0803EE54: .4byte 0x0201CFB0
_0803EE58: .4byte 0x00000824
_0803EE5C:
	mov r0, #3
	bl sub_08077AEC
_0803EE62:
	mov r0, #0
_0803EE64:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0803EDC4
	.align 2, 0

