	thumb_func_start sub_08030760
sub_08030760: @ 0x08030760
	push {r4, r5, lr}
	add r4, r0, #0
	mov r5, #1
	add r0, r5, #0
	ldrb r1, [r4, #2]
	and r0, r1
	mov r1, #0xD6
	cmp r0, #0
	beq _08030774
	ldr r1, _080307A4 @ =0x000080D6
_08030774:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	add r0, r5, #0
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x60
	cmp r0, #0
	beq _0803078E
	ldr r1, _080307A8 @ =0x00008060
_0803078E:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_080307A4: .4byte 0x000080D6
_080307A8: .4byte 0x00008060
	thumb_func_end sub_08030760

