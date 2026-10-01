	thumb_func_start sub_0803AB78
sub_0803AB78: @ 0x0803AB78
	push {r4, r5, lr}
	sub sp, #0x80
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _0803AC1C
	ldr r0, _0803ABC4 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x80
	bne _0803ABDC
	ldr r1, _0803ABC8 @ =0x08083634
	ldr r0, _0803ABCC @ =0x086248EE
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r0, _0803ABD0 @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	bl sub_080753F4
	ldr r0, _0803ABD4 @ =0x00000206
	ldr r1, _0803ABD8 @ =0x00000613
	mov r2, #0xB
	mov r3, sp
	bl sub_080602A4
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl sub_08060308
	mov r0, #0x7F
	b _0803AC1E
	.align 2, 0
_0803ABC4: .4byte 0x02017A40
_0803ABC8: .4byte gUnk_08083634
_0803ABCC: .4byte gUnk_086248EE
_0803ABD0: .4byte gUnk_0822C720
_0803ABD4: .4byte 0x00000206
_0803ABD8: .4byte 0x00000613
_0803ABDC:
	ldr r0, _0803AC28 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _0803AC1C
	mov r5, #1
	add r0, r5, #0
	ldrb r1, [r4, #2]
	and r0, r1
	mov r2, #0x43
	cmp r0, #0
	beq _0803ABF4
	ldr r2, _0803AC2C @ =0x00008043
_0803ABF4:
	mov r1, #0xFA
	lsl r1, r1, #2
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	add r0, r5, #0
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x4B
	cmp r0, #0
	beq _0803AC10
	ldr r1, _0803AC30 @ =0x0000804B
_0803AC10:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_0803AC1C:
	mov r0, #0
_0803AC1E:
	add sp, #0x80
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0803AC28: .4byte 0x0201AE60
_0803AC2C: .4byte 0x00008043
_0803AC30: .4byte 0x0000804B
	thumb_func_end sub_0803AB78

