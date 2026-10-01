	thumb_func_start sub_08035F80
sub_08035F80: @ 0x08035F80
	push {lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _08035FD4
	ldr r0, _08035FB8 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08035FBC
	cmp r0, #0x80
	bne _08035FD4
	ldrb r1, [r1, #2]
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #1
	mov r2, #1
	bl sub_08022784
	mov r0, #0x7F
	b _08035FD6
	.align 2, 0
_08035FB8: .4byte 0x02017A40
_08035FBC:
	ldrb r1, [r1, #2]
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #1
	mov r2, #0
	mov r3, #1
	bl sub_0802272C
	mov r0, #0x7E
	b _08035FD6
_08035FD4:
	mov r0, #0
_08035FD6:
	pop {r1}
	bx r1
	thumb_func_end sub_08035F80
	.align 2, 0

