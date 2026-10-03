	thumb_func_start DuelCmd_SetNegationFlag
DuelCmd_SetNegationFlag: @ 0x08013BB0
	push {r4, lr}
	ldr r1, _08013BD0 @ =0x020185C0
	ldr r0, _08013BD4 @ =0x00000FFF
	ldrh r2, [r1]
	and r0, r2
	sub r0, #0x15
	add r3, r1, #0
	cmp r0, #6
	bls _08013BC4
	b _08013CBC
_08013BC4:
	lsl r0, r0, #2
	ldr r1, _08013BD8 @ =0x08013BDC
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08013BD0: .4byte 0x020185C0
_08013BD4: .4byte 0x00000FFF
_08013BD8: .4byte 0x08013BDC
_08013BDC:
	.4byte _08013BF8
	.4byte _08013C14
	.4byte _08013C30
	.4byte _08013C4C
	.4byte _08013C68
	.4byte _08013C88
	.4byte _08013CA4
_08013BF8:
	ldr r2, _08013C0C @ =0x020192E0
	ldr r4, _08013C10 @ =0x00001ACD
	add r2, r2, r4
	mov r1, #1
	ldrb r0, [r3, #2]
	and r1, r0
	lsl r1, r1, #1
	mov r0, #3
	neg r0, r0
	b _08013CB4
_08013C0C: .4byte 0x020192E0
_08013C10: .4byte 0x00001ACD
_08013C14:
	ldr r2, _08013C28 @ =0x020192E0
	ldr r0, _08013C2C @ =0x00001ACD
	add r2, r2, r0
	mov r1, #1
	ldrb r4, [r3, #2]
	and r1, r4
	lsl r1, r1, #2
	mov r0, #5
	neg r0, r0
	b _08013CB4
_08013C28: .4byte 0x020192E0
_08013C2C: .4byte 0x00001ACD
_08013C30:
	ldr r2, _08013C44 @ =0x020192E0
	ldr r0, _08013C48 @ =0x00001ACD
	add r2, r2, r0
	mov r1, #1
	ldrb r4, [r3, #2]
	and r1, r4
	lsl r1, r1, #4
	mov r0, #0x11
	neg r0, r0
	b _08013CB4
_08013C44: .4byte 0x020192E0
_08013C48: .4byte 0x00001ACD
_08013C4C:
	ldr r2, _08013C60 @ =0x020192E0
	ldr r0, _08013C64 @ =0x00001ACD
	add r2, r2, r0
	mov r1, #1
	ldrb r4, [r3, #2]
	and r1, r4
	lsl r1, r1, #3
	mov r0, #9
	neg r0, r0
	b _08013CB4
_08013C60: .4byte 0x020192E0
_08013C64: .4byte 0x00001ACD
_08013C68:
	ldr r0, _08013C80 @ =0x020192E0
	ldr r1, _08013C84 @ =0x00001ACC
	add r0, r0, r1
	ldrb r4, [r3, #2]
	lsl r2, r4, #7
	mov r1, #0x7F
	ldrb r4, [r0]
	and r1, r4
	orr r1, r2
	strb r1, [r0]
	b _08013CBC
	.align 2, 0
_08013C80: .4byte 0x020192E0
_08013C84: .4byte 0x00001ACC
_08013C88:
	ldr r2, _08013C9C @ =0x020192E0
	ldr r0, _08013CA0 @ =0x00001ACC
	add r2, r2, r0
	mov r1, #1
	ldrb r4, [r3, #2]
	and r1, r4
	lsl r1, r1, #6
	mov r0, #0x41
	neg r0, r0
	b _08013CB4
_08013C9C: .4byte 0x020192E0
_08013CA0: .4byte 0x00001ACC
_08013CA4:
	ldr r2, _08013CD0 @ =0x020192E0
	ldr r0, _08013CD4 @ =0x00001ACD
	add r2, r2, r0
	mov r1, #1
	ldrb r4, [r3, #2]
	and r1, r4
	mov r0, #2
	neg r0, r0
_08013CB4:
	ldrb r4, [r2]
	and r0, r4
	orr r0, r1
	strb r0, [r2]
_08013CBC:
	ldr r0, _08013CD8 @ =0x0000080D
	add r1, r3, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
_08013CD0: .4byte 0x020192E0
_08013CD4: .4byte 0x00001ACD
_08013CD8: .4byte 0x0000080D
	thumb_func_end DuelCmd_SetNegationFlag

