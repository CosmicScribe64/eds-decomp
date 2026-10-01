	thumb_func_start sub_08062F6C
sub_08062F6C: @ 0x08062F6C
	push {r4, r5, lr}
	ldr r5, _08062F84 @ =0x03000040
	ldr r0, _08062F88 @ =0x0000485A
	add r4, r5, r0
	ldrb r0, [r4]
	cmp r0, #1
	beq _08062FBA
	cmp r0, #1
	bgt _08062F8C
	cmp r0, #0
	beq _08062F96
	b _08062FE8
_08062F84: .4byte 0x03000040
_08062F88: .4byte 0x0000485A
_08062F8C:
	cmp r0, #2
	beq _08062FC0
	cmp r0, #3
	beq _08062FC6
	b _08062FE8
_08062F96:
	bl sub_0806472C
	ldr r3, _08062FAC @ =0x00004876
	add r1, r5, r3
	ldrh r0, [r1]
	cmp r0, #0
	beq _08062FB4
	ldr r0, _08062FB0 @ =0x02015262
	ldrh r1, [r1]
	b _08063022
	.align 2, 0
_08062FAC: .4byte 0x00004876
_08062FB0: .4byte 0x02015262
_08062FB4:
	bl sub_08064264
	b _08062FD8
_08062FBA:
	bl sub_08064AF4
	b _08062FCA
_08062FC0:
	bl sub_08064BA0
	b _08062FCA
_08062FC6:
	bl sub_08064DF8
_08062FCA:
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08062FDE
	ldr r1, _08062FE4 @ =0x02020310
	mov r0, #0
	str r0, [r1]
	str r0, [r1, #4]
_08062FD8:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08062FDE:
	mov r0, #0
	b _0806302C
	.align 2, 0
_08062FE4: .4byte 0x02020310
_08062FE8:
	ldr r5, _08063034 @ =0x02015160
	mov r1, #0x8E
	lsl r1, r1, #1
	add r0, r5, #0
	bl sub_08075278
	ldr r4, _08063038 @ =0x02020310
	ldr r0, [r4, #0x10]
	ldr r1, [r4, #0xC]
	add r0, r0, r1
	add r1, r4, #0
	add r1, #0x6C
	ldrh r1, [r1]
	bl __modsi3
	lsl r0, r0, #1
	add r4, #0x2C
	add r0, r0, r4
	mov r1, #0x81
	lsl r1, r1, #1
	add r5, r5, r1
	ldr r2, _0806303C @ =0x080865DC
	ldrh r3, [r0]
	lsl r1, r3, #3
	add r1, r1, r3
	lsl r1, r1, #3
	add r1, r1, r2
	ldrh r1, [r1]
	add r0, r5, #0
_08063022:
	bl sub_08062AF4
	bl sub_080754BC
	mov r0, #1
_0806302C:
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_08063034: .4byte 0x02015160
_08063038: .4byte 0x02020310
_0806303C: .4byte gUnk_080865DC
	thumb_func_end sub_08062F6C

