	thumb_func_start sub_080754F8
sub_080754F8: @ 0x080754F8
	push {r4, r5, r6, lr}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	mov r9, r0
	bl sub_080754BC
	ldr r2, _08075588 @ =0x03000040
	mov r1, #0x83
	lsl r1, r1, #3
	add r0, r2, r1
	mov r6, #0
	str r6, [r0]
	sub r1, #4
	add r0, r2, r1
	str r6, [r0]
	ldr r0, _0807558C @ =0x04000208
	mov r8, r0
	mov r4, #0
	strh r6, [r0]
	ldr r3, _08075590 @ =0x04000200
	ldrh r5, [r3]
	ldr r1, _08075594 @ =0x0000FFFD
	add r0, r1, #0
	and r0, r5
	strh r0, [r3]
	mov r5, #1
	mov r0, r8
	strh r5, [r0]
	strh r6, [r0]
	ldrh r0, [r3]
	and r1, r0
	strh r1, [r3]
	ldr r0, _08075598 @ =0x03000000
	str r6, [r0, #4]
	mov r1, r8
	strh r5, [r1]
	ldr r1, _0807559C @ =0x00004878
	add r0, r2, r1
	strb r4, [r0]
	add r1, #1
	add r0, r2, r1
	strb r4, [r0]
	add r1, #1
	add r0, r2, r1
	strb r4, [r0]
	sub r1, #0x23
	add r0, r2, r1
	strb r4, [r0]
	add r1, #1
	add r0, r2, r1
	strb r4, [r0]
	add r1, #1
	add r0, r2, r1
	strb r4, [r0]
	add r1, #1
	add r0, r2, r1
	strb r4, [r0]
	add r1, #1
	add r0, r2, r1
	strb r4, [r0]
	mov r0, #0x82
	lsl r0, r0, #3
	add r2, r2, r0
	mov r1, r9
	str r1, [r2]
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_08075588: .4byte 0x03000040
_0807558C: .4byte 0x04000208
_08075590: .4byte 0x04000200
_08075594: .4byte 0x0000FFFD
_08075598: .4byte 0x03000000
_0807559C: .4byte 0x00004878
	thumb_func_end sub_080754F8

