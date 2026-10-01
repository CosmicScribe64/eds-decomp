	thumb_func_start sub_08022A9C
sub_08022A9C: @ 0x08022A9C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r7, r0, #0
	ldr r2, _08022B28 @ =0x02017FB0
	mov r0, #0x81
	lsl r0, r0, #2
	add r1, r2, r0
	ldr r0, _08022B2C @ =0x0000F022
	strh r0, [r1]
	ldr r5, _08022B30 @ =0x020192E4
	mov r0, #1
	and r0, r7
	ldr r1, _08022B34 @ =0x00000D64
	mul r1, r0
	add r3, r1, r5
	lsl r0, r7, #0x18
	lsr r0, r0, #0x10
	ldrb r4, [r3, #3]
	orr r0, r4
	ldr r4, _08022B38 @ =0x00000206
	add r2, r2, r4
	strh r0, [r2]
	mov r4, #0
	ldrb r0, [r3, #3]
	cmp r4, r0
	bge _08022AF4
	add r6, r1, #0
	ldr r2, _08022B3C @ =0x000007C4
	add r2, r2, r5
	mov r8, r2
	add r5, r3, #0
_08022ADC:
	lsl r2, r4, #2
	ldr r0, _08022B40 @ =0x020181B8
	add r0, r2, r0
	mov r3, r8
	add r1, r6, r3
	add r1, r1, r2
	bl sub_08007558
	add r4, #1
	ldrb r0, [r5, #3]
	cmp r4, r0
	blt _08022ADC
_08022AF4:
	ldr r4, _08022B44 @ =0x020181B4
	ldr r2, _08022B30 @ =0x020192E4
	mov r0, #1
	and r0, r7
	ldr r1, _08022B34 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #3]
	lsl r1, r0, #2
	add r1, #4
	add r0, r4, #0
	bl sub_080723B4
	ldr r2, _08022B48 @ =0x00000101
	add r1, r4, r2
	mov r0, #3
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
_08022B28: .4byte 0x02017FB0
_08022B2C: .4byte 0x0000F022
_08022B30: .4byte 0x020192E4
_08022B34: .4byte 0x00000D64
_08022B38: .4byte 0x00000206
_08022B3C: .4byte 0x000007C4
_08022B40: .4byte 0x020181B8
_08022B44: .4byte 0x020181B4
_08022B48: .4byte 0x00000101
	thumb_func_end sub_08022A9C

