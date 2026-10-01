	thumb_func_start sub_080755A0
sub_080755A0: @ 0x080755A0
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	bl sub_080754BC
	ldr r2, _0807560C @ =0x03000040
	mov r1, #0x83
	lsl r1, r1, #3
	add r0, r2, r1
	mov r5, #0
	str r5, [r0]
	sub r1, #4
	add r0, r2, r1
	str r5, [r0]
	ldr r6, _08075610 @ =0x04000208
	mov r0, #0
	mov r8, r0
	strh r5, [r6]
	ldr r3, _08075614 @ =0x04000200
	ldrh r4, [r3]
	ldr r1, _08075618 @ =0x0000FFFD
	add r0, r1, #0
	and r0, r4
	strh r0, [r3]
	mov r4, #1
	strh r4, [r6]
	strh r5, [r6]
	ldrh r0, [r3]
	and r1, r0
	strh r1, [r3]
	ldr r0, _0807561C @ =0x03000000
	str r5, [r0, #4]
	strh r4, [r6]
	ldr r1, _08075620 @ =0x00004858
	add r0, r2, r1
	mov r1, r8
	strb r1, [r0]
	ldr r1, _08075624 @ =0x00004859
	add r0, r2, r1
	mov r1, r8
	strb r1, [r0]
	ldr r1, _08075628 @ =0x0000485A
	add r0, r2, r1
	mov r1, r8
	strb r1, [r0]
	ldr r0, _0807562C @ =0x0000485B
	add r2, r2, r0
	strb r1, [r2]
	mov r0, #1
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_0807560C: .4byte 0x03000040
_08075610: .4byte 0x04000208
_08075614: .4byte 0x04000200
_08075618: .4byte 0x0000FFFD
_0807561C: .4byte 0x03000000
_08075620: .4byte 0x00004858
_08075624: .4byte 0x00004859
_08075628: .4byte 0x0000485A
_0807562C: .4byte 0x0000485B
	thumb_func_end sub_080755A0

