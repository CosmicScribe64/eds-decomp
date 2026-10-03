	thumb_func_start DuelCmd_CompactHand
DuelCmd_CompactHand: @ 0x08010F84
	push {r4, lr}
	ldr r4, _08010FA4 @ =0x020185C0
	ldrh r1, [r4]
	lsr r0, r1, #0xF
	bl CompactHand
	ldr r0, _08010FA8 @ =0x0000080D
	add r4, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	pop {r4}
	pop {r0}
	bx r0
_08010FA4: .4byte 0x020185C0
_08010FA8: .4byte 0x0000080D
	thumb_func_end DuelCmd_CompactHand

