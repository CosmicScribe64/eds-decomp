	thumb_func_start sub_0807B150
sub_0807B150: @ 0x0807B150
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	add r5, r0, #0
	add r4, r3, #0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov ip, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	ldr r1, _0807B214 @ =0x00000C04
	add r0, r4, r1
	str r5, [r0]
	mov r6, #0
	cmp r6, ip
	bcs _0807B1EA
	mov r0, #0x1F
	add r7, r2, #0
	and r7, r0
	str r7, [sp, #4]
	mov r0, #0xF8
	lsl r0, r0, #2
	mov r9, r0
	add r0, r2, #0
	mov r1, r9
	and r0, r1
	lsr r0, r0, #5
	str r0, [sp, #0]
	mov r7, #0xF8
	lsl r7, r7, #7
	mov r8, r7
	add r0, r2, #0
	mov r1, r8
	and r0, r1
	lsr r0, r0, #0xA
	mov sl, r0
_0807B19E:
	lsl r3, r6, #1
	add r3, r4, r3
	ldrh r1, [r5]
	strh r1, [r3]
	add r5, #2
	lsl r2, r6, #2
	add r2, r4, r2
	mov r0, #0x1F
	and r0, r1
	ldr r7, [sp, #4]
	sub r0, r7, r0
	mov r7, #0x80
	lsl r7, r7, #3
	add r1, r2, r7
	strb r0, [r1]
	mov r0, r9
	ldrh r1, [r3]
	and r0, r1
	lsr r0, r0, #5
	ldr r7, [sp, #0]
	sub r0, r7, r0
	ldr r7, _0807B218 @ =0x00000401
	add r1, r2, r7
	strb r0, [r1]
	mov r0, r8
	ldrh r3, [r3]
	and r0, r3
	lsr r0, r0, #0xA
	mov r1, sl
	sub r0, r1, r0
	add r7, #1
	add r2, r2, r7
	strb r0, [r2]
	add r0, r6, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	cmp r6, ip
	bcc _0807B19E
_0807B1EA:
	ldr r0, _0807B21C @ =0x00000C08
	add r1, r4, r0
	mov r2, #0
	mov r0, #1
	strh r0, [r1]
	ldr r1, _0807B220 @ =0x00000C02
	add r0, r4, r1
	mov r7, ip
	strh r7, [r0]
	sub r1, #2
	add r0, r4, r1
	strb r2, [r0]
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0807B214: .4byte 0x00000C04
_0807B218: .4byte 0x00000401
_0807B21C: .4byte 0x00000C08
_0807B220: .4byte 0x00000C02
	thumb_func_end sub_0807B150

