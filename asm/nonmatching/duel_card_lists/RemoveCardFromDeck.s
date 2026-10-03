	thumb_func_start RemoveCardFromDeck
RemoveCardFromDeck: @ 0x08007F48
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	add r5, r0, #0
	add r7, r1, #0
	mov r4, #0
	ldr r2, _08007FC4 @ =0x020192E4
	mov r0, #1
	and r0, r5
	ldr r1, _08007FC8 @ =0x00000D64
	mul r0, r1
	add r1, r0, r2
	ldrb r3, [r1, #3]
	cmp r4, r3
	bge _08007FDE
	add r6, r0, #0
	ldr r0, _08007FCC @ =0x000007C4
	add r0, r0, r2
	mov ip, r0
	mov r8, r2
	add r2, r1, #0
	mov r3, #0
_08007F76:
	mov r1, ip
	add r0, r6, r1
	add r0, r0, r3
	ldr r1, [r7]
	ldr r0, [r0]
	cmp r1, r0
	bne _08007FD4
	ldrb r0, [r2, #3]
	sub r0, #1
	strb r0, [r2, #3]
	add r6, r4, #0
	cmp r4, r0
	bge _08007FBE
	mov r0, #1
	and r0, r5
	ldr r1, _08007FC8 @ =0x00000D64
	mul r0, r1
	ldr r1, _08007FD0 @ =0x02019AA8
	mov r5, r8
	add r2, r0, r5
	add r5, r3, #4
	add r7, r0, r1
	lsl r0, r4, #2
	add r4, r0, r7
_08007FA6:
	add r1, r7, r5
	add r0, r4, #0
	str r2, [sp, #0]
	bl CopyDuelCard
	add r5, #4
	add r4, #4
	add r6, #1
	ldr r2, [sp, #0]
	ldrb r0, [r2, #3]
	cmp r6, r0
	blt _08007FA6
_08007FBE:
	mov r0, #1
	b _08007FE0
	.align 2, 0
_08007FC4: .4byte 0x020192E4
_08007FC8: .4byte 0x00000D64
_08007FCC: .4byte 0x000007C4
_08007FD0: .4byte 0x02019AA8
_08007FD4:
	add r3, #4
	add r4, #1
	ldrb r1, [r2, #3]
	cmp r4, r1
	blt _08007F76
_08007FDE:
	mov r0, #0
_08007FE0:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end RemoveCardFromDeck

