	thumb_func_start sub_08074430
sub_08074430: @ 0x08074430
	push {r4, lr}
	ldr r0, _08074448 @ =0x03000040
	ldr r1, _0807444C @ =0x00004859
	add r4, r0, r1
	ldrb r1, [r4]
	cmp r1, #0
	beq _08074450
	cmp r1, #1
	beq _0807445C
	mov r0, #1
	b _0807446E
	.align 2, 0
_08074448: .4byte 0x03000040
_0807444C: .4byte 0x00004859
_08074450:
	ldr r0, _08074458 @ =0x02017A30
	strb r1, [r0, #0xB]
	b _08074466
	.align 2, 0
_08074458: .4byte 0x02017A30
_0807445C:
	bl sub_08027C58
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0807446C
_08074466:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_0807446C:
	mov r0, #0
_0807446E:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08074430

