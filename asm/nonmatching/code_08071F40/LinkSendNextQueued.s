	thumb_func_start LinkSendNextQueued
LinkSendNextQueued: @ 0x08071FA0
	push {r4, r5, r6, lr}
	ldr r4, _08071FFC @ =0x030049D0
	mov r0, #0xC0
	lsl r0, r0, #2
	add r6, r4, r0
	ldrh r0, [r6]
	cmp r0, #0
	beq _08072000
	add r0, r4, #0
	bl LinkSendPacket
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08072000
	ldrh r0, [r6]
	sub r0, #1
	mov r1, #0
	strh r0, [r6]
	mov r2, #0x83
	lsl r2, r2, #4
	add r0, r4, r2
	str r1, [r0]
	ldrh r2, [r6]
	cmp r1, r2
	bge _08072006
	add r5, r4, #0
	add r4, r0, #0
_08071FD6:
	ldr r0, [r4]
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #2
	add r0, r1, r5
	add r2, r5, #0
	add r2, #0xC
	add r1, r1, r2
	mov r2, #0xC
	bl MemCopy16
	ldr r0, [r4]
	add r0, #1
	str r0, [r4]
	ldrh r1, [r6]
	cmp r0, r1
	blt _08071FD6
	b _08072006
	.align 2, 0
_08071FFC: .4byte 0x030049D0
_08072000:
	ldr r0, _0807200C @ =0x081A7374
	bl LinkSendPacket
_08072006:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_0807200C: .4byte gLinkPacketAck
	thumb_func_end LinkSendNextQueued

