	thumb_func_start sub_080241F0
sub_080241F0: @ 0x080241F0
	push {r4, lr}
	ldr r4, _08024210 @ =0x0201D7F8
	add r1, r4, #0
	bl sub_0807695C
	ldr r0, _08024214 @ =0xFFFFF7B8
	add r4, r4, r0
	mov r0, #3
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08024210: .4byte 0x0201D7F8
_08024214: .4byte 0xFFFFF7B8
	thumb_func_end sub_080241F0

