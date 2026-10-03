	thumb_func_start AddCardToDeckTop
AddCardToDeckTop: @ 0x08007C58
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r9, r0
	mov r8, r1
	ldr r3, _08007CD4 @ =0x020192E4
	mov r0, #1
	mov r1, r9
	and r0, r1
	ldr r1, _08007CD8 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r0, r2, r3
	ldrb r4, [r0, #3]
	cmp r4, #0
	ble _08007C98
	lsl r1, r4, #2
	sub r6, r1, #4
	ldr r5, _08007CDC @ =0x000007C4
	add r0, r3, r5
	add r7, r2, r0
	add r5, r1, r7
_08007C86:
	add r1, r7, r6
	add r0, r5, #0
	bl CopyDuelCard
	sub r6, #4
	sub r5, #4
	sub r4, #1
	cmp r4, #0
	bgt _08007C86
_08007C98:
	ldr r3, _08007CD4 @ =0x020192E4
	mov r0, #1
	mov r1, r9
	and r0, r1
	ldr r1, _08007CD8 @ =0x00000D64
	mul r0, r1
	add r2, r0, r3
	ldrb r1, [r2, #3]
	add r1, #1
	strb r1, [r2, #3]
	mov r1, #5
	neg r1, r1
	mov r2, r8
	ldrb r2, [r2, #2]
	and r1, r2
	mov r4, r8
	strb r1, [r4, #2]
	ldr r5, _08007CDC @ =0x000007C4
	add r3, r3, r5
	add r0, r0, r3
	mov r1, r8
	bl CopyDuelCard
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08007CD4: .4byte 0x020192E4
_08007CD8: .4byte 0x00000D64
_08007CDC: .4byte 0x000007C4
	thumb_func_end AddCardToDeckTop

