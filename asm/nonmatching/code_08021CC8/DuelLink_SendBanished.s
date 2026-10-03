	thumb_func_start DuelLink_SendBanished
DuelLink_SendBanished: @ 0x08022CAC
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r7, r0, #0
	ldr r2, _08022D38 @ =0x02017FB0
	mov r0, #0x81
	lsl r0, r0, #2
	add r1, r2, r0
	ldr r0, _08022D3C @ =0x0000F025
	strh r0, [r1]
	ldr r5, _08022D40 @ =0x020192E4
	mov r0, #1
	and r0, r7
	ldr r1, _08022D44 @ =0x00000D64
	mul r1, r0
	add r3, r1, r5
	lsl r0, r7, #0x18
	lsr r0, r0, #0x10
	ldrb r4, [r3, #6]
	orr r0, r4
	ldr r4, _08022D48 @ =0x00000206
	add r2, r2, r4
	strh r0, [r2]
	mov r4, #0
	ldrb r0, [r3, #6]
	cmp r4, r0
	bge _08022D04
	add r6, r1, #0
	ldr r2, _08022D4C @ =0x00000B84
	add r2, r2, r5
	mov r8, r2
	add r5, r3, #0
_08022CEC:
	lsl r2, r4, #2
	ldr r0, _08022D50 @ =0x020181B8
	add r0, r2, r0
	mov r3, r8
	add r1, r6, r3
	add r1, r1, r2
	bl CopyDuelCard
	add r4, #1
	ldrb r0, [r5, #6]
	cmp r4, r0
	blt _08022CEC
_08022D04:
	ldr r4, _08022D54 @ =0x020181B4
	ldr r2, _08022D40 @ =0x020192E4
	mov r0, #1
	and r0, r7
	ldr r1, _08022D44 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #6]
	lsl r1, r0, #2
	add r1, #4
	add r0, r4, #0
	bl LinkQueueMessage
	ldr r2, _08022D58 @ =0x00000101
	add r1, r4, r2
	mov r0, #0x11
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08022D38: .4byte 0x02017FB0
_08022D3C: .4byte 0x0000F025
_08022D40: .4byte 0x020192E4
_08022D44: .4byte 0x00000D64
_08022D48: .4byte 0x00000206
_08022D4C: .4byte 0x00000B84
_08022D50: .4byte 0x020181B8
_08022D54: .4byte 0x020181B4
_08022D58: .4byte 0x00000101
	thumb_func_end DuelLink_SendBanished

