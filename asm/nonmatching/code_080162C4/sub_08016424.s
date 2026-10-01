	thumb_func_start sub_08016424
sub_08016424: @ 0x08016424
	push {lr}
	ldr r1, _08016438 @ =0x0201CFB0
	mov r0, #4
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0801643C
	bl sub_080609C4
	b _08016454
_08016438: .4byte 0x0201CFB0
_0801643C:
	bl sub_08060B2C
	cmp r0, #0
	beq _08016454
	ldr r1, _08016458 @ =0x020185C0
	ldr r0, _0801645C @ =0x0000080D
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_08016454:
	pop {r0}
	bx r0
_08016458: .4byte 0x020185C0
_0801645C: .4byte 0x0000080D
	thumb_func_end sub_08016424

