	thumb_func_start DuelLink_RequestInterruptIfIdle
DuelLink_RequestInterruptIfIdle: @ 0x08022914
	push {r4, lr}
	ldr r0, _08022948 @ =0x02017FB0
	ldr r1, _0802294C @ =0x00000306
	add r4, r0, r1
	ldrb r2, [r4]
	lsl r0, r2, #0x19
	cmp r0, #0
	blt _08022942
	ldr r0, _08022950 @ =0x020192E0
	ldr r1, _08022954 @ =0x00001B14
	add r3, r0, r1
	ldrh r1, [r3]
	mov r0, #0xFE
	lsl r0, r0, #1
	and r0, r1
	cmp r0, #0
	bne _08022942
	mov r0, #0x40
	orr r0, r2
	strb r0, [r4]
	ldr r0, _08022958 @ =0xFFFFFE03
	and r0, r1
	strh r0, [r3]
_08022942:
	pop {r4}
	pop {r0}
	bx r0
_08022948: .4byte 0x02017FB0
_0802294C: .4byte 0x00000306
_08022950: .4byte 0x020192E0
_08022954: .4byte 0x00001B14
_08022958: .4byte 0xFFFFFE03
	thumb_func_end DuelLink_RequestInterruptIfIdle

