	thumb_func_start DuelPrompt_PostData
DuelPrompt_PostData: @ 0x080226CC
	push {r4, r5, r6, r7, lr}
	add r4, r0, #0
	add r5, r1, #0
	add r7, r2, #0
	add r6, r3, #0
	cmp r6, #8
	ble _080226DC
	mov r6, #8
_080226DC:
	ldr r0, _0802271C @ =0x020192E0
	ldr r1, _08022720 @ =0x00001B50
	add r2, r0, r1
	mov r1, #1
	and r4, r1
	lsl r3, r4, #2
	mov r1, #5
	neg r1, r1
	ldrb r4, [r2]
	and r1, r4
	orr r1, r3
	strb r1, [r2]
	mov r1, #0x3F
	and r5, r1
	lsl r3, r5, #4
	ldr r1, _08022724 @ =0xFFFFFC0F
	ldrh r4, [r2]
	and r1, r4
	orr r1, r3
	strh r1, [r2]
	ldr r1, _08022728 @ =0x00001B52
	add r0, r0, r1
	lsl r2, r6, #1
	add r1, r7, #0
	bl MemCopy16
	bl DuelPrompt_Start
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802271C: .4byte 0x020192E0
_08022720: .4byte 0x00001B50
_08022724: .4byte 0xFFFFFC0F
_08022728: .4byte 0x00001B52
	thumb_func_end DuelPrompt_PostData

