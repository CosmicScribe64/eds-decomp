	thumb_func_start sub_080299F8
sub_080299F8: @ 0x080299F8
	push {r4, r5, r6, lr}
	sub sp, #8
	ldr r5, _08029A38 @ =0x02020310
	ldr r1, _08029A3C @ =0x00000ABF
	add r0, r5, r1
	ldrb r0, [r0]
	ldr r2, _08029A40 @ =0x00000AF4
	add r1, r5, r2
	ldrb r1, [r1]
	mov r3, #0xAC
	lsl r3, r3, #4
	add r2, r5, r3
	ldrh r2, [r2]
	ldr r4, _08029A44 @ =0x00000AC4
	add r3, r5, r4
	str r0, [sp, #0]
	ldr r6, _08029A48 @ =0x00000ADC
	add r4, r5, r6
	str r4, [sp, #4]
	bl sub_080283BC
	ldr r0, _08029A4C @ =0x00000AF5
	add r5, r5, r0
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
	add sp, #8
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08029A38: .4byte 0x02020310
_08029A3C: .4byte 0x00000ABF
_08029A40: .4byte 0x00000AF4
_08029A44: .4byte 0x00000AC4
_08029A48: .4byte 0x00000ADC
_08029A4C: .4byte 0x00000AF5
	thumb_func_end sub_080299F8

