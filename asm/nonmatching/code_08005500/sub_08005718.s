	thumb_func_start sub_08005718
sub_08005718: @ 0x08005718
	push {r4, lr}
	ldr r1, _0800572C @ =0x0201527C
	mov r0, #2
	ldrb r1, [r1, #2]
	and r0, r1
	cmp r0, #0
	beq _08005730
	mov r0, #1
	b _080057B4
	.align 2, 0
_0800572C: .4byte 0x0201527C
_08005730:
	ldr r0, _08005748 @ =0x03000040
	ldr r1, _0800574C @ =0x00004858
	add r4, r0, r1
	ldrb r0, [r4]
	cmp r0, #1
	beq _08005768
	cmp r0, #1
	bgt _08005750
	cmp r0, #0
	beq _08005756
	b _080057AC
	.align 2, 0
_08005748: .4byte 0x03000040
_0800574C: .4byte 0x00004858
_08005750:
	cmp r0, #2
	beq _08005794
	b _080057AC
_08005756:
	mov r0, #0x64
	bl sub_08001C10
	mov r0, #1
	bl sub_08077B24
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08005768:
	bl sub_08001AE4
	cmp r0, #0
	beq _080057A8
	ldr r0, _08005788 @ =0x03000040
	ldr r2, _0800578C @ =0x00004858
	add r1, r0, r2
	ldrb r2, [r1]
	add r2, #1
	mov r3, #0
	strb r2, [r1]
	ldr r1, _08005790 @ =0x00004859
	add r0, r0, r1
	strb r3, [r0]
	b _080057A8
	.align 2, 0
_08005788: .4byte 0x03000040
_0800578C: .4byte 0x00004858
_08005790: .4byte 0x00004859
_08005794:
	bl sub_08064604
	cmp r0, #0
	beq _080057A8
	mov r0, #0x65
	bl sub_08001C10
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_080057A8:
	mov r0, #0
	b _080057B4
_080057AC:
	bl sub_08001AE4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
_080057B4:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08005718
	.align 2, 0

