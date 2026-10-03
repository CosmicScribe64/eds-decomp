	thumb_func_start FindCardByPassword
FindCardByPassword: @ 0x0807C304
	push {r4, r5, lr}
	sub sp, #4
	mov r3, #0
	ldr r0, _0807C358 @ =0x0201F7B0
	mov r1, sp
	add r2, r0, #0
	add r2, #8
_0807C312:
	ldrb r4, [r2]
	lsl r0, r4, #4
	strb r0, [r1]
	ldrb r4, [r2, #1]
	orr r0, r4
	strb r0, [r1]
	add r1, #1
	add r2, #2
	add r3, #1
	cmp r3, #3
	bls _0807C312
	mov r3, #0
	mov r4, sp
	ldrb r5, [r4]
_0807C32E:
	lsl r0, r3, #2
	ldr r1, _0807C35C @ =0x08623120
	add r2, r0, r1
	ldrb r0, [r2]
	cmp r0, r5
	bne _0807C360
	ldrb r1, [r2, #1]
	ldrb r0, [r4, #1]
	cmp r1, r0
	bne _0807C360
	ldrb r1, [r2, #2]
	ldrb r0, [r4, #2]
	cmp r1, r0
	bne _0807C360
	ldrb r1, [r2, #3]
	ldrb r0, [r4, #3]
	cmp r1, r0
	bne _0807C360
	lsl r0, r3, #0x10
	lsr r0, r0, #0x10
	b _0807C36C
_0807C358: .4byte 0x0201F7B0
_0807C35C: .4byte gCardPasswords
_0807C360:
	add r3, #1
	mov r0, #0xCD
	lsl r0, r0, #2
	cmp r3, r0
	bls _0807C32E
	mov r0, #0
_0807C36C:
	add sp, #4
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end FindCardByPassword

