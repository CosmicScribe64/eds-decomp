	thumb_func_start sub_08074B38
sub_08074B38: @ 0x08074B38
	push {r4, r5, r6, lr}
	add r5, r0, #0
	add r6, r1, #0
	lsl r2, r2, #0x10
	ldr r0, _08074B68 @ =0x02000000
	mov r1, #0x80
	lsl r1, r1, #9
	add r4, r0, r1
	strb r5, [r4]
	ldr r5, _08074B6C @ =0x00010001
	add r4, r0, r5
	strb r6, [r4]
	ldr r4, _08074B70 @ =0x00010004
	add r5, r0, r4
	mov r4, #0x7F
	and r3, r4
	lsr r2, r2, #9
	orr r2, r3
	strb r2, [r5]
	bl sub_08075278
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_08074B68: .4byte 0x02000000
_08074B6C: .4byte 0x00010001
_08074B70: .4byte 0x00010004
	thumb_func_end sub_08074B38

