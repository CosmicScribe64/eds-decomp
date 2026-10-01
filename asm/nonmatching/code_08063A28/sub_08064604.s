	thumb_func_start sub_08064604
sub_08064604: @ 0x08064604
	push {lr}
	ldr r0, _0806461C @ =0x03000040
	ldr r1, _08064620 @ =0x00004859
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #4
	bhi _0806468C
	lsl r0, r0, #2
	ldr r1, _08064624 @ =0x08064628
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0806461C: .4byte 0x03000040
_08064620: .4byte 0x00004859
_08064624: .4byte 0x08064628
_08064628:
	.4byte _0806463C
	.4byte _08064642
	.4byte _08064648
	.4byte _0806464E
	.4byte _0806467C
_0806463C:
	bl sub_080643E4
	b _08064658
_08064642:
	bl sub_08064420
	b _08064652
_08064648:
	bl sub_08064538
	b _08064652
_0806464E:
	bl sub_080645B0
_08064652:
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0806466C
_08064658:
	ldr r1, _08064670 @ =0x02020310
	mov r0, #0
	str r0, [r1]
	str r0, [r1, #4]
	ldr r0, _08064674 @ =0x03000040
	ldr r1, _08064678 @ =0x00004859
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
_0806466C:
	mov r0, #0
	b _0806468E
_08064670: .4byte 0x02020310
_08064674: .4byte 0x03000040
_08064678: .4byte 0x00004859
_0806467C:
	bl sub_080770E8
	ldr r0, _08064694 @ =0x02020310
	ldr r0, [r0, #8]
	bl sub_0800495C
	bl sub_080754BC
_0806468C:
	mov r0, #1
_0806468E:
	pop {r1}
	bx r1
	.align 2, 0
_08064694: .4byte 0x02020310
	thumb_func_end sub_08064604

