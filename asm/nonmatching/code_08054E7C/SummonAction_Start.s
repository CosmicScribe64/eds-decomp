	thumb_func_start SummonAction_Start
SummonAction_Start: @ 0x08055A00
	push {r4, lr}
	ldr r3, _08055A8C @ =0x0201CF90
	ldr r0, _08055A90 @ =0xFFFFF01F
	ldrh r1, [r3, #0xE]
	and r0, r1
	strh r0, [r3, #0xE]
	mov r0, #0xF
	ldrb r2, [r3, #0xF]
	and r0, r2
	strb r0, [r3, #0xF]
	mov r0, #8
	neg r0, r0
	ldrb r1, [r3, #0x10]
	and r0, r1
	strb r0, [r3, #0x10]
	ldr r0, _08055A94 @ =0xFFFFF807
	ldrh r2, [r3, #0x10]
	and r0, r2
	strh r0, [r3, #0x10]
	ldr r0, [r3, #0x10]
	ldr r1, _08055A98 @ =0xFFF807FF
	and r0, r1
	str r0, [r3, #0x10]
	mov r0, #1
	ldrb r1, [r3, #0xE]
	orr r0, r1
	mov r1, #3
	neg r1, r1
	and r0, r1
	strb r0, [r3, #0xE]
	ldr r2, _08055A9C @ =0x020192E4
	ldrb r1, [r3]
	lsl r0, r1, #0x1F
	mov r4, #1
	lsr r0, r0, #0x1F
	ldr r1, _08055AA0 @ =0x00000D64
	mul r1, r0
	add r1, r1, r2
	mov r0, #0x20
	ldrb r2, [r1, #8]
	orr r0, r2
	strb r0, [r1, #8]
	ldrb r1, [r3]
	lsl r0, r1, #0x1F
	cmp r0, #0
	beq _08055A86
	ldr r0, _08055AA4 @ =0x02015EE8
	ldrb r0, [r0, #1]
	and r4, r0
	cmp r4, #0
	beq _08055A86
	mov r0, #2
	ldrb r2, [r3, #0xE]
	orr r0, r2
	strb r0, [r3, #0xE]
	ldr r0, _08055AA8 @ =0x0000F05A
	add r1, r3, #0
	mov r2, #0x14
	bl DuelLink_SendMessageData
	ldr r1, _08055AAC @ =0x02017FB0
	ldr r0, _08055AB0 @ =0x00000306
	add r1, r1, r0
	mov r0, #0x7F
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_08055A86:
	pop {r4}
	pop {r0}
	bx r0
_08055A8C: .4byte 0x0201CF90
_08055A90: .4byte 0xFFFFF01F
_08055A94: .4byte 0xFFFFF807
_08055A98: .4byte 0xFFF807FF
_08055A9C: .4byte 0x020192E4
_08055AA0: .4byte 0x00000D64
_08055AA4: .4byte 0x02015EE8
_08055AA8: .4byte 0x0000F05A
_08055AAC: .4byte 0x02017FB0
_08055AB0: .4byte 0x00000306
	thumb_func_end SummonAction_Start

