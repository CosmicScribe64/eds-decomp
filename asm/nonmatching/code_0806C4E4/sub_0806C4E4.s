	thumb_func_start sub_0806C4E4
sub_0806C4E4: @ 0x0806C4E4
	push {r4, lr}
	ldr r1, _0806C520 @ =0x0201DB20
	ldr r2, _0806C524 @ =0x00001C4E
	add r0, r1, r2
	mov r3, #0
	mov r2, #0
	strh r2, [r0]
	ldr r4, _0806C528 @ =0x00001C4C
	add r0, r1, r4
	strh r2, [r0]
	ldr r2, _0806C52C @ =0x00001C49
	add r0, r1, r2
	strb r3, [r0]
	sub r4, #2
	add r0, r1, r4
	strb r3, [r0]
	add r2, #9
	add r0, r1, r2
	strb r3, [r0]
	add r4, #9
	add r0, r1, r4
	strb r3, [r0]
	ldr r0, _0806C530 @ =0x00001C4B
	add r1, r1, r0
	strb r3, [r1]
	mov r0, #1
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0806C520: .4byte 0x0201DB20
_0806C524: .4byte 0x00001C4E
_0806C528: .4byte 0x00001C4C
_0806C52C: .4byte 0x00001C49
_0806C530: .4byte 0x00001C4B
	thumb_func_end sub_0806C4E4

