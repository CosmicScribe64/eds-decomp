	thumb_func_start sub_0807AB8C
sub_0807AB8C: @ 0x0807AB8C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r5, r0, #0
	add r4, r1, #0
	lsl r2, r2, #0x18
	lsr r6, r2, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov r9, r3
	mov r1, #0
	cmp r1, r9
	bcs _0807ABEC
	mov r0, #0x20
	sub r0, r0, r6
	lsl r0, r0, #1
	mov sl, r0
_0807ABB2:
	mov r3, #0
	add r7, r1, #1
	cmp r3, r6
	bcs _0807ABE2
	ldr r0, _0807ABFC @ =0x0000FC0F
	mov r8, r0
	mov r0, #0xFC
	lsl r0, r0, #2
	mov ip, r0
_0807ABC4:
	ldrh r2, [r5]
	mov r1, r8
	and r1, r2
	mov r0, ip
	and r0, r2
	lsl r0, r0, #1
	orr r1, r0
	strh r1, [r4]
	add r5, #2
	add r4, #2
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r3, r6
	bcc _0807ABC4
_0807ABE2:
	add r4, sl
	lsl r0, r7, #0x18
	lsr r1, r0, #0x18
	cmp r1, r9
	bcc _0807ABB2
_0807ABEC:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0807ABFC: .4byte 0x0000FC0F
	thumb_func_end sub_0807AB8C

