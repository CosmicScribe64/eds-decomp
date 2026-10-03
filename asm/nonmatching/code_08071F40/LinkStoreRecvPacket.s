	thumb_func_start LinkStoreRecvPacket
LinkStoreRecvPacket: @ 0x08072010
	push {r4, lr}
	add r1, r0, #0
	ldr r2, _08072048 @ =0x030049D0
	mov r0, #0xA5
	lsl r0, r0, #3
	add r4, r2, r0
	ldrh r3, [r4]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #2
	ldr r3, _0807204C @ =0x0000052C
	add r2, r2, r3
	add r0, r0, r2
	mov r2, #0xC
	bl MemCopy16
	ldrh r0, [r4]
	add r0, #1
	mov r1, #0x3F
	and r0, r1
	strh r0, [r4]
	ldr r0, _08072050 @ =0x081A7374
	bl LinkSendPacket
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08072048: .4byte 0x030049D0
_0807204C: .4byte 0x0000052C
_08072050: .4byte gLinkPacketAck
	thumb_func_end LinkStoreRecvPacket

