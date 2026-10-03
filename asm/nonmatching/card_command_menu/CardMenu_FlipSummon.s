	thumb_func_start CardMenu_FlipSummon
CardMenu_FlipSummon: @ 0x08048FE0
	push {r4, r5, r6, lr}
	ldr r4, _08049034 @ =0x020192E0
	ldr r0, _08049038 @ =0x00001B33
	add r6, r4, r0
	ldrb r1, [r6]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	ldr r2, _0804903C @ =0x00001B34
	add r5, r4, r2
	ldrh r2, [r5]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x18
	bl QueueFlipSummon
	ldrb r6, [r6]
	lsl r2, r6, #0x1E
	lsr r2, r2, #0x1F
	ldrh r5, [r5]
	lsl r0, r5, #0x17
	lsr r0, r0, #0x18
	mov r1, #0x94
	mul r1, r0
	ldr r0, _08049040 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	add r0, r4, #0
	add r0, #0x2C
	add r1, r1, r0
	mov r0, #4
	ldrb r2, [r1, #7]
	orr r0, r2
	strb r0, [r1, #7]
	ldr r0, _08049044 @ =0x00001B2C
	add r4, r4, r0
	mov r0, #3
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_08049034: .4byte 0x020192E0
_08049038: .4byte 0x00001B33
_0804903C: .4byte 0x00001B34
_08049040: .4byte 0x00000D64
_08049044: .4byte 0x00001B2C
	thumb_func_end CardMenu_FlipSummon

