	thumb_func_start sub_0803A52C
sub_0803A52C: @ 0x0803A52C
	push {r4, lr}
	add r3, r0, #0
	ldr r2, _0803A56C @ =0x0201D810
	ldrb r1, [r2, #5]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1E
	ldrh r1, [r2, #6]
	add r0, r1, r0
	lsl r0, r0, #2
	add r1, r2, #0
	add r1, #0xC
	add r4, r0, r1
	mov r0, #4
	ldrb r1, [r3, #4]
	and r0, r1
	cmp r0, #0
	beq _0803A550
	b _0803A64C
_0803A550:
	ldr r0, _0803A570 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	sub r0, #0x7C
	cmp r0, #4
	bhi _0803A64C
	lsl r0, r0, #2
	ldr r1, _0803A574 @ =0x0803A578
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0803A56C: .4byte 0x0201D810
_0803A570: .4byte 0x02017A40
_0803A574: .4byte 0x0803A578
_0803A578:
	.4byte _0803A62A
	.4byte _0803A608
	.4byte _0803A5E4
	.4byte _0803A5C8
	.4byte _0803A58C
_0803A58C:
	mov r0, #7
	ldrb r1, [r3, #0xA]
	and r0, r1
	cmp r0, #1
	bne _0803A64C
	ldrb r1, [r3, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0803A5B8 @ =0x00000526
	ldrh r2, [r3, #0xC]
	bl sub_08044224
	cmp r0, #0
	beq _0803A64C
	ldr r0, _0803A5BC @ =0x00000206
	ldr r1, _0803A5C0 @ =0x00000712
	ldr r3, _0803A5C4 @ =0x08083588
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #0x7F
	b _0803A64E
_0803A5B8: .4byte 0x00000526
_0803A5BC: .4byte 0x00000206
_0803A5C0: .4byte 0x00000712
_0803A5C4: .4byte gUnk_08083588
_0803A5C8:
	ldrb r1, [r3, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _0803A5E0 @ =0x00000526
	ldrh r3, [r3, #0xC]
	bl sub_0802AF34
	mov r0, #0x7E
	b _0803A64E
	.align 2, 0
_0803A5E0: .4byte 0x00000526
_0803A5E4:
	mov r0, #1
	ldrb r3, [r3, #2]
	and r0, r3
	mov r3, #0x65
	cmp r0, #0
	beq _0803A5F2
	ldr r3, _0803A604 @ =0x00008065
_0803A5F2:
	ldrh r1, [r4]
	ldrh r2, [r4, #2]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x7D
	b _0803A64E
	.align 2, 0
_0803A604: .4byte 0x00008065
_0803A608:
	ldrb r3, [r3, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	ldrb r3, [r2, #5]
	lsl r1, r3, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r2, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r2, #0xC
	add r1, r1, r2
	mov r2, #0
	mov r3, #0x20
	bl sub_08056094
	mov r0, #0x7C
	b _0803A64E
_0803A62A:
	mov r0, #1
	ldrb r3, [r3, #2]
	and r0, r3
	mov r1, #0x60
	cmp r0, #0
	beq _0803A638
	ldr r1, _0803A648 @ =0x00008060
_0803A638:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x64
	b _0803A64E
_0803A648: .4byte 0x00008060
_0803A64C:
	mov r0, #0
_0803A64E:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0803A52C

