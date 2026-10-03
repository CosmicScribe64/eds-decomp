	thumb_func_start LinkSendPacket
LinkSendPacket: @ 0x08071F40
	push {r4, r5, lr}
	add r1, r0, #0
	ldr r4, _08071F88 @ =0x03005204
	add r0, r4, #0
	mov r2, #0xC
	bl MemCopy16
	ldr r0, _08071F8C @ =0x0000F0FF
	ldrh r1, [r4]
	and r0, r1
	ldr r2, _08071F90 @ =0xFFFFFCE6
	add r5, r4, r2
	ldrh r2, [r5]
	lsl r1, r2, #8
	orr r0, r1
	strh r0, [r4]
	add r0, r4, #0
	mov r1, #0xC
	bl LinkSioSend
	cmp r0, #0
	beq _08071F98
	ldr r1, _08071F94 @ =0xFFFFFACE
	add r0, r4, r1
	add r1, r4, #0
	mov r2, #0xC
	bl MemCopy16
	ldrh r0, [r5]
	add r0, #1
	mov r1, #0xF
	and r0, r1
	strh r0, [r5]
	mov r0, #1
	b _08071F9A
	.align 2, 0
_08071F88: .4byte 0x03005204
_08071F8C: .4byte 0x0000F0FF
_08071F90: .4byte 0xFFFFFCE6
_08071F94: .4byte 0xFFFFFACE
_08071F98:
	mov r0, #0
_08071F9A:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end LinkSendPacket

