	thumb_func_start DuelCmd_NegateActivation
DuelCmd_NegateActivation: @ 0x08011C18
	push {r4, lr}
	ldr r1, _08011C80 @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _08011C38
	ldr r0, _08011C84 @ =0x020192E0
	ldr r1, _08011C88 @ =0x00001B12
	add r0, r0, r1
	mov r1, #2
	ldrb r0, [r0]
	and r1, r0
	ldr r3, _08011C8C @ =0x020185C0
	cmp r1, #0
	bne _08011C6C
_08011C38:
	ldr r2, _08011C90 @ =0x02017A40
	mov r4, #0xF0
	lsl r4, r4, #2
	add r1, r2, r4
	ldr r3, _08011C8C @ =0x020185C0
	ldrh r0, [r1]
	cmp r0, #1
	bls _08011C6C
	lsl r0, r0, #2
	ldrh r1, [r1]
	add r0, r0, r1
	lsl r0, r0, #2
	mov r4, #0x96
	lsl r4, r4, #2
	add r1, r2, r4
	add r1, r0, r1
	mov r0, #4
	ldrb r2, [r1, #4]
	orr r2, r0
	strb r2, [r1, #4]
	ldrh r0, [r3, #2]
	cmp r0, #0
	beq _08011C6C
	mov r0, #8
	orr r2, r0
	strb r2, [r1, #4]
_08011C6C:
	ldr r0, _08011C94 @ =0x0000080D
	add r1, r3, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
_08011C80: .4byte 0x02015EE8
_08011C84: .4byte 0x020192E0
_08011C88: .4byte 0x00001B12
_08011C8C: .4byte 0x020185C0
_08011C90: .4byte 0x02017A40
_08011C94: .4byte 0x0000080D
	thumb_func_end DuelCmd_NegateActivation

