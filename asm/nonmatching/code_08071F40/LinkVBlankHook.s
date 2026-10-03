	thumb_func_start LinkVBlankHook
LinkVBlankHook: @ 0x08072054
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r4, _080720CC @ =0x030049D0
	ldr r0, _080720D0 @ =0x00000524
	add r5, r4, r0
	ldrb r1, [r5]
	mov r6, #1
	and r6, r1
	cmp r6, #0
	beq _0807206C
	b _08072214
_0807206C:
	mov r0, #1
	orr r0, r1
	strb r0, [r5]
	ldr r2, _080720D4 @ =0x08087600
	ldr r0, _080720D8 @ =0x04000128
	ldrh r1, [r0]
	mov r0, #0x30
	and r0, r1
	lsr r0, r0, #4
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x18
	asr r0, r0, #0x18
	ldr r1, _080720DC @ =0x0000030E
	add r7, r4, r1
	add r1, r7, #0
	mov r2, #0xC
	bl LinkSioRecv
	ldr r2, _080720E0 @ =0x00000526
	add r2, r2, r4
	mov r8, r2
	strh r0, [r2]
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _080720A2
	b _080721D4
_080720A2:
	ldr r3, _080720E4 @ =0x0000051C
	add r2, r4, r3
	ldrh r0, [r7]
	lsr r1, r0, #8
	mov r0, #0xF
	and r0, r1
	ldrh r3, [r2]
	cmp r3, r0
	bne _080720EC
	ldr r0, _080720E8 @ =0x081A7382
	bl LinkSendPacket
	mov r0, r8
	strh r6, [r0]
	mov r0, #2
	neg r0, r0
	ldrb r1, [r5]
	and r0, r1
	strb r0, [r5]
	b _08072214
	.align 2, 0
_080720CC: .4byte 0x030049D0
_080720D0: .4byte 0x00000524
_080720D4: .4byte gLinkPartnerSlot
_080720D8: .4byte 0x04000128
_080720DC: .4byte 0x0000030E
_080720E0: .4byte 0x00000526
_080720E4: .4byte 0x0000051C
_080720E8: .4byte gLinkPacketDupAck
_080720EC:
	strh r0, [r2]
	mov r0, #0xF0
	and r0, r1
	cmp r0, #0xB0
	beq _080721A4
	cmp r0, #0xB0
	bgt _08072104
	cmp r0, #0x90
	beq _0807218C
	cmp r0, #0xA0
	beq _08072170
	b _080721C8
_08072104:
	cmp r0, #0xE0
	beq _08072118
	cmp r0, #0xE0
	bgt _08072112
	cmp r0, #0xD0
	beq _08072148
	b _080721C8
_08072112:
	cmp r0, #0xF0
	beq _08072148
	b _080721C8
_08072118:
	ldr r0, _0807213C @ =0x08087604
	bl DebugPrintf
	bl DebugPrintFlush
	ldr r2, _08072140 @ =0x00000302
	add r0, r4, r2
	bl LinkSendPacket
	ldr r3, _08072144 @ =0x0000051E
	add r0, r4, r3
	strh r6, [r0]
	mov r0, #2
	neg r0, r0
	ldrb r1, [r5]
	and r0, r1
	strb r0, [r5]
	b _08072214
_0807213C: .4byte gStrDebugLinkReceiveRetry
_08072140: .4byte 0x00000302
_08072144: .4byte 0x0000051E
_08072148:
	bl LinkSendNextQueued
	ldr r1, _08072164 @ =0x030049D0
	ldr r3, _08072168 @ =0x0000051E
	add r2, r1, r3
	mov r0, #0
	strh r0, [r2]
	ldr r0, _0807216C @ =0x00000524
	add r1, r1, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	b _08072212
_08072164: .4byte 0x030049D0
_08072168: .4byte 0x0000051E
_0807216C: .4byte 0x00000524
_08072170:
	add r0, r7, #0
	bl LinkStoreRecvPacket
	ldr r3, _08072188 @ =0x0000051E
	add r0, r4, r3
	strh r6, [r0]
	mov r0, #2
	neg r0, r0
	ldrb r1, [r5]
	and r0, r1
	strb r0, [r5]
	b _08072214
_08072188: .4byte 0x0000051E
_0807218C:
	add r0, r7, #0
	bl LinkStoreRecvPacket
	mov r0, #4
	ldrb r2, [r5]
	orr r0, r2
	ldr r3, _080721A0 @ =0x0000051E
	add r1, r4, r3
	b _080721B4
	.align 2, 0
_080721A0: .4byte 0x0000051E
_080721A4:
	add r0, r7, #0
	bl LinkStoreRecvPacket
	mov r0, #4
	ldrb r1, [r5]
	orr r0, r1
	ldr r2, _080721C4 @ =0x0000051E
	add r1, r4, r2
_080721B4:
	strh r6, [r1]
	mov r1, #2
	neg r1, r1
	and r0, r1
	strb r0, [r5]
	mov r0, #1
	b _08072216
	.align 2, 0
_080721C4: .4byte 0x0000051E
_080721C8:
	ldr r0, _080721D0 @ =0x081A7390
	bl LinkSendPacket
	b _080721DA
_080721D0: .4byte gLinkPacketResend
_080721D4:
	ldr r0, _08072220 @ =0x081A7374
	bl LinkSendPacket
_080721DA:
	ldr r5, _08072224 @ =0x030049D0
	ldr r3, _08072228 @ =0x0000051E
	add r4, r5, r3
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x78
	bls _08072206
	ldr r0, _0807222C @ =0x08087614
	ldrh r1, [r4]
	bl DebugPrintf
	bl DebugPrintFlush
	mov r0, #0
	strh r0, [r4]
	ldr r0, _08072230 @ =0x00000522
	add r1, r5, r0
	mov r0, #1
	strh r0, [r1]
_08072206:
	ldr r2, _08072234 @ =0x00000524
	add r1, r5, r2
	mov r0, #2
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
_08072212:
	strb r0, [r1]
_08072214:
	mov r0, #0
_08072216:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08072220: .4byte gLinkPacketAck
_08072224: .4byte 0x030049D0
_08072228: .4byte 0x0000051E
_0807222C: .4byte gStrDebugLinkRecvTimeout
_08072230: .4byte 0x00000522
_08072234: .4byte 0x00000524
	thumb_func_end LinkVBlankHook

