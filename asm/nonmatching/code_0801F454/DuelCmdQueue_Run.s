	thumb_func_start DuelCmdQueue_Run
DuelCmdQueue_Run: @ 0x0801F454
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r0, _0801F474 @ =0x020192E0
	mov r1, #0xDA
	lsl r1, r1, #5
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #1
	beq _0801F538
	cmp r0, #1
	bgt _0801F478
	cmp r0, #0
	beq _0801F47E
	b _0801F61C
	.align 2, 0
_0801F474: .4byte 0x020192E0
_0801F478:
	cmp r0, #2
	beq _0801F558
	b _0801F61C
_0801F47E:
	ldr r7, _0801F5A0 @ =0x020185C0
	ldr r2, _0801F5A4 @ =0x00000808
	add r5, r7, r2
	ldrh r0, [r5]
	cmp r0, #0
	bne _0801F48C
	b _0801F61C
_0801F48C:
	add r6, r7, #0
	add r6, #8
	add r0, r7, #0
	add r1, r6, #0
	mov r2, #8
	bl MemCopy16
	ldrh r0, [r5]
	sub r0, #1
	strh r0, [r5]
	mov r4, #0
	cmp r4, r0
	bge _0801F4C4
	mov r8, r5
	add r5, r7, #0
	add r5, #0x10
_0801F4AC:
	add r0, r6, #0
	add r1, r5, #0
	mov r2, #8
	bl MemCopy16
	add r6, #8
	add r5, #8
	add r4, #1
	mov r0, r8
	ldrh r0, [r0]
	cmp r4, r0
	blt _0801F4AC
_0801F4C4:
	ldr r6, _0801F5A8 @ =0x02017FB0
	ldr r1, _0801F5AC @ =0x00000307
	add r4, r6, r1
	mov r0, #2
	ldrb r2, [r4]
	orr r0, r2
	strb r0, [r4]
	ldr r5, _0801F5A0 @ =0x020185C0
	ldr r0, _0801F5B0 @ =0x0000080D
	add r1, r5, r0
	mov r0, #0x20
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	ldr r1, _0801F5B4 @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _0801F510
	ldr r0, _0801F5B8 @ =0x0000F041
	add r1, r5, #0
	mov r2, #8
	bl DuelLink_SendMessageData
	mov r0, #3
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	mov r2, #0x80
	lsl r2, r2, #2
	add r0, r6, r2
	mov r1, #0
	strh r1, [r0]
	add r2, #2
	add r0, r6, r2
	strh r1, [r0]
_0801F510:
	ldr r0, _0801F5BC @ =0x0000080A
	add r1, r5, r0
	mov r0, #0x80
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r0, _0801F5C0 @ =0x0000080C
	add r1, r5, r0
	ldr r0, _0801F5C4 @ =0xFFFFF01F
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	ldr r0, _0801F5C8 @ =0x020192E0
	mov r1, #0xDA
	lsl r1, r1, #5
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
_0801F538:
	bl DuelCmd_Dispatch
	ldr r0, _0801F5A0 @ =0x020185C0
	ldr r2, _0801F5B0 @ =0x0000080D
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	cmp r0, #0
	blt _0801F602
	ldr r0, _0801F5C8 @ =0x020192E0
	mov r1, #0xDA
	lsl r1, r1, #5
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
_0801F558:
	ldr r1, _0801F5A8 @ =0x02017FB0
	ldr r2, _0801F5AC @ =0x00000307
	add r0, r1, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r2, r0, #0x1F
	cmp r2, #0
	bne _0801F5D4
	ldr r0, _0801F5CC @ =0x00000202
	add r1, r1, r0
	ldrh r0, [r1]
	add r0, #1
	strh r0, [r1]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x77
	bls _0801F602
	strh r2, [r1]
	mov r0, #0xEE
	lsl r0, r0, #8
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelLink_SendMessage
	bl FadeOutBGM
	ldr r1, _0801F5C8 @ =0x020192E0
	ldr r2, _0801F5D0 @ =0x00001B12
	add r1, r1, r2
	mov r0, #0x20
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	b _0801F61C
	.align 2, 0
_0801F5A0: .4byte 0x020185C0
_0801F5A4: .4byte 0x00000808
_0801F5A8: .4byte 0x02017FB0
_0801F5AC: .4byte 0x00000307
_0801F5B0: .4byte 0x0000080D
_0801F5B4: .4byte 0x02015EE8
_0801F5B8: .4byte 0x0000F041
_0801F5BC: .4byte 0x0000080A
_0801F5C0: .4byte 0x0000080C
_0801F5C4: .4byte 0xFFFFF01F
_0801F5C8: .4byte 0x020192E0
_0801F5CC: .4byte 0x00000202
_0801F5D0: .4byte 0x00001B12
_0801F5D4:
	ldr r0, _0801F608 @ =0x00000202
	add r1, r1, r0
	mov r0, #0
	strh r0, [r1]
	ldr r4, _0801F60C @ =0x020185C0
	ldr r0, _0801F610 @ =0x00000FFF
	ldrh r1, [r4]
	and r0, r1
	cmp r0, #4
	beq _0801F5EC
	bl PlayDuelBGM
_0801F5EC:
	ldr r0, _0801F614 @ =0x020192E0
	mov r2, #0xDA
	lsl r2, r2, #5
	add r0, r0, r2
	mov r1, #0
	strb r1, [r0]
	ldr r1, _0801F618 @ =0x00000808
	add r0, r4, r1
	ldrh r0, [r0]
	cmp r0, #0
	beq _0801F61C
_0801F602:
	mov r0, #1
	b _0801F61E
	.align 2, 0
_0801F608: .4byte 0x00000202
_0801F60C: .4byte 0x020185C0
_0801F610: .4byte 0x00000FFF
_0801F614: .4byte 0x020192E0
_0801F618: .4byte 0x00000808
_0801F61C:
	mov r0, #0
_0801F61E:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end DuelCmdQueue_Run

