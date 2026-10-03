	thumb_func_start LoadBgImage
LoadBgImage: @ 0x08072FAC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	mov ip, r3
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0]
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov sl, r2
	mov r0, ip
	str r0, [sp, #4]
	ldrh r1, [r0]
	lsl r0, r1, #1
	add r1, r0, #0
	add r1, #8
	ldr r2, [sp, #4]
	add r1, r1, r2
	mov r9, r1
	add r0, #0x10
	add r2, r2, r0
	mov r3, sl
	lsl r0, r3, #5
	ldr r7, _080730A0 @ =0x06004000
	add r5, r0, r7
	ldrh r1, [r1]
	lsl r0, r1, #6
	add r0, r0, r2
	mov r8, r0
	mov r4, r8
	add r4, #8
	mov r3, #0
	cmp r1, #0
	beq _08073034
_08072FFA:
	ldrh r0, [r2]
	add r1, r0, #0
	mov r7, #0xFF
	lsl r7, r7, #8
	and r0, r7
	cmp r0, #0
	beq _08073010
	lsl r0, r6, #8
	add r0, r1, r0
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_08073010:
	mov r0, #0xFF
	and r0, r1
	cmp r0, #0
	beq _0807301E
	add r0, r1, r6
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
_0807301E:
	strh r1, [r5]
	add r5, #2
	add r2, #2
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	mov r1, r9
	ldrh r1, [r1]
	lsl r0, r1, #5
	cmp r3, r0
	blt _08072FFA
_08073034:
	lsl r0, r6, #1
	mov r1, #0xA0
	lsl r1, r1, #0x13
	add r0, r0, r1
	mov r1, ip
	add r1, #8
	ldr r3, [sp, #4]
	ldrh r3, [r3]
	lsl r2, r3, #1
	bl MemCopy16
	mov r3, #0
	mov r5, r8
	ldrh r5, [r5]
	cmp r3, r5
	bcs _0807308C
	mov r6, #0xFF
	lsl r6, r6, #8
	ldr r5, _080730A4 @ =0x0300045C
_0807305A:
	ldrh r1, [r4]
	add r4, #2
	ldrh r2, [r4]
	add r4, #2
	mov r0, #0x3F
	and r0, r1
	and r1, r6
	lsr r1, r1, #3
	orr r0, r1
	ldr r7, [sp, #0]
	add r0, r0, r7
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	add r0, r0, r5
	mov r7, sl
	lsr r1, r7, #1
	add r2, r2, r1
	strh r2, [r0]
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	mov r0, r8
	ldrh r0, [r0]
	cmp r3, r0
	bcc _0807305A
_0807308C:
	mov r1, r9
	ldrh r0, [r1]
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_080730A0: .4byte 0x06004000
_080730A4: .4byte 0x0300045C
	thumb_func_end LoadBgImage

