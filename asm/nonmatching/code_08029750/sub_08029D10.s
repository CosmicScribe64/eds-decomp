	thumb_func_start sub_08029D10
sub_08029D10: @ 0x08029D10
	push {r4, r5, lr}
	sub sp, #0x10
	ldr r0, _08029D6C @ =0x086AC828
	mov r1, #0x10
	mov r2, #8
	mov r3, #8
	bl sub_08028AB8
	ldr r0, _08029D70 @ =0x086AD028
	mov r1, #0x18
	mov r2, #8
	mov r3, #8
	bl sub_08028AB8
	ldr r0, _08029D74 @ =0x086AD828
	mov r1, #0x88
	lsl r1, r1, #1
	mov r2, #8
	mov r3, #8
	bl sub_08028AB8
	mov r5, #3
	str r5, [sp, #0]
	mov r0, #1
	str r0, [sp, #4]
	ldr r4, _08029D78 @ =0x02020DEC
	str r4, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xC]
	mov r1, #0
	mov r2, #0x40
	mov r3, #0xF
	bl sub_0807B9D4
	strb r5, [r4, #0x19]
	bl sub_08076F9C
	mov r1, #1
	and r0, r1
	sub r4, #0x1D
	strb r0, [r4]
	mov r0, #1
	add sp, #0x10
	pop {r4, r5}
	pop {r1}
	bx r1
_08029D6C: .4byte gUnk_086AC828
_08029D70: .4byte gUnk_086AD028
_08029D74: .4byte gUnk_086AD828
_08029D78: .4byte 0x02020DEC
	thumb_func_end sub_08029D10

