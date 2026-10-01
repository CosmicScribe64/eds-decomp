	thumb_func_start sub_0807332C
sub_0807332C: @ 0x0807332C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	mov sl, r0
	ldr r5, [sp, #0x28]
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	str r1, [sp, #0]
	lsl r2, r2, #0x10
	mov r9, r2
	lsr r4, r2, #0x10
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r8, r3
	ldrh r0, [r5]
	lsl r1, r0, #1
	add r0, r1, #0
	add r0, #8
	add r0, r5, r0
	str r0, [sp, #4]
	add r1, #0x10
	add r1, r5, r1
	lsl r0, r3, #5
	ldr r2, _080733EC @ =0x06004000
	add r0, r0, r2
	ldr r3, [sp, #4]
	ldrh r3, [r3]
	lsl r2, r3, #5
	add r7, r1, r2
	add r6, r7, #0
	add r6, #8
	bl sub_08075294
	lsl r4, r4, #1
	mov r0, #0xA0
	lsl r0, r0, #0x13
	add r4, r4, r0
	add r1, r5, #0
	add r1, #8
	ldrh r5, [r5]
	lsl r2, r5, #1
	add r0, r4, #0
	bl sub_08075294
	mov r3, #0
	ldrh r5, [r7]
	cmp r3, r5
	bcs _080733D8
	mov r0, #0xFF
	lsl r0, r0, #8
	mov ip, r0
	mov r1, sl
	lsl r0, r1, #0xB
	ldr r1, _080733F0 @ =0x0300045C
	add r0, r0, r1
	mov sl, r0
	mov r2, r9
	lsr r0, r2, #0x14
	lsl r4, r0, #0xC
_080733A8:
	ldrh r1, [r6]
	add r6, #2
	ldrh r2, [r6]
	add r6, #2
	mov r0, #0x3F
	and r0, r1
	mov r5, ip
	and r1, r5
	lsr r1, r1, #3
	orr r0, r1
	ldr r1, [sp, #0]
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	add r0, sl
	add r2, r8
	orr r2, r4
	strh r2, [r0]
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	ldrh r2, [r7]
	cmp r3, r2
	bcc _080733A8
_080733D8:
	ldr r3, [sp, #4]
	ldrh r0, [r3]
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_080733EC: .4byte 0x06004000
_080733F0: .4byte 0x0300045C
	thumb_func_end sub_0807332C

