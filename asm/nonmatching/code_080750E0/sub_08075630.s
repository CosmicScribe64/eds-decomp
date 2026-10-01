	thumb_func_start sub_08075630
sub_08075630: @ 0x08075630
	push {r4, r5, r6, lr}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	mov r4, #0xA0
	lsl r4, r4, #0x13
	ldr r0, _08075688 @ =0x0822C300
	mov r8, r0
	add r0, r4, #0
	mov r1, r8
	mov r2, #0x20
	bl sub_08075294
	mov r0, #0
	mov r9, r0
	strh r0, [r4]
	ldr r0, _0807568C @ =0x06004000
	ldr r5, _08075690 @ =0x0822C320
	mov r6, #0x80
	lsl r6, r6, #2
	add r1, r5, #0
	add r2, r6, #0
	bl sub_08075294
	ldr r4, _08075694 @ =0x05000200
	add r0, r4, #0
	mov r1, r8
	mov r2, #0x20
	bl sub_08075294
	ldr r0, _08075698 @ =0x06010000
	add r1, r5, #0
	add r2, r6, #0
	bl sub_08075294
	mov r0, r9
	strh r0, [r4]
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08075688: .4byte gUnk_0822C300
_0807568C: .4byte 0x06004000
_08075690: .4byte gUnk_0822C320
_08075694: .4byte 0x05000200
_08075698: .4byte 0x06010000
	thumb_func_end sub_08075630

