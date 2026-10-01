	thumb_func_start sub_08026BD0
sub_08026BD0: @ 0x08026BD0
	push {lr}
	bl sub_080269FC
	ldr r3, _08026BF0 @ =0x02020310
	ldr r0, _08026BF4 @ =0x00000B1E
	add r1, r3, r0
	ldrb r2, [r1]
	cmp r2, #3
	beq _08026BFC
	ldr r1, _08026BF8 @ =0x00000B18
	add r0, r3, r1
	bl sub_0807883C
	mov r0, #0
	b _08026C22
	.align 2, 0
_08026BF0: .4byte 0x02020310
_08026BF4: .4byte 0x00000B1E
_08026BF8: .4byte 0x00000B18
_08026BFC:
	mov r0, #0
	strb r0, [r1]
	mov r1, #0x40
	neg r1, r1
	ldr r2, _08026C28 @ =0x00000B18
	add r3, r3, r2
	mov r0, #1
	mov r2, #0
	bl sub_080787F4
	ldr r1, _08026C2C @ =0x04000050
	ldr r2, _08026C30 @ =0x00003F41
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08026C34 @ =0x0000080D
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #1
_08026C22:
	pop {r1}
	bx r1
	.align 2, 0
_08026C28: .4byte 0x00000B18
_08026C2C: .4byte 0x04000050
_08026C30: .4byte 0x00003F41
_08026C34: .4byte 0x0000080D
	thumb_func_end sub_08026BD0

