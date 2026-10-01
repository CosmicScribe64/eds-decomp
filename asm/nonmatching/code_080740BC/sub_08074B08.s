	thumb_func_start sub_08074B08
sub_08074B08: @ 0x08074B08
	push {r4, r5, lr}
	add r4, r0, #0
	add r5, r1, #0
	ldr r0, _08074B30 @ =0x02000000
	mov r1, #0x80
	lsl r1, r1, #9
	add r2, r0, r1
	mov r3, #0
	strb r4, [r2]
	ldr r4, _08074B34 @ =0x00010001
	add r2, r0, r4
	strb r5, [r2]
	add r4, #3
	add r2, r0, r4
	strb r3, [r2]
	bl sub_08075278
	pop {r4, r5}
	pop {r0}
	bx r0
_08074B30: .4byte 0x02000000
_08074B34: .4byte 0x00010001
	thumb_func_end sub_08074B08

