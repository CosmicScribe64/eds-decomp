	thumb_func_start LoadBgImage4bppMap1
LoadBgImage4bppMap1: @ 0x080731D0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r9, r0
	lsl r7, r1, #0x10
	lsr r0, r7, #0x10
	lsl r2, r2, #0x10
	lsr r6, r2, #0x10
	ldrh r2, [r3]
	lsl r1, r2, #1
	add r2, r1, #0
	add r2, #8
	add r2, r2, r3
	mov r8, r2
	add r1, #0x10
	add r1, r3, r1
	ldrh r4, [r2]
	lsl r2, r4, #5
	add r5, r1, r2
	add r4, r5, #0
	add r4, #8
	add r1, r6, #0
	add r2, r3, #0
	bl LoadBgImage4bppGfx
	mov r3, #0
	ldrh r0, [r5]
	cmp r3, r0
	bcs _08073254
	mov r1, #0xFF
	lsl r1, r1, #8
	mov sl, r1
	ldr r2, _08073268 @ =0x03000C5C
	mov ip, r2
	lsr r0, r7, #0x14
	lsl r0, r0, #0xC
	str r0, [sp, #0]
_08073224:
	ldrh r1, [r4]
	add r4, #2
	ldrh r2, [r4]
	add r4, #2
	mov r0, #0x3F
	and r0, r1
	mov r7, sl
	and r1, r7
	lsr r1, r1, #3
	orr r0, r1
	add r0, r9
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	add r0, ip
	add r2, r2, r6
	ldr r1, [sp, #0]
	orr r2, r1
	strh r2, [r0]
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	ldrh r2, [r5]
	cmp r3, r2
	bcc _08073224
_08073254:
	mov r4, r8
	ldrh r0, [r4]
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08073268: .4byte 0x03000C5C
	thumb_func_end LoadBgImage4bppMap1

