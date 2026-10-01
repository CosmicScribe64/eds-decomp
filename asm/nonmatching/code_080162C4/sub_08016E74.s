	thumb_func_start sub_08016E74
sub_08016E74: @ 0x08016E74
	push {r4, r5, r6, lr}
	ldr r5, _08016EA8 @ =0x020185C0
	ldrh r1, [r5]
	lsr r0, r1, #0xF
	ldrh r1, [r5, #2]
	ldrh r2, [r5, #4]
	ldr r4, _08016EAC @ =0x0201CFB0
	ldr r3, _08016EB0 @ =0x00000808
	add r4, r4, r3
	mov r3, #8
	ldrb r6, [r4]
	orr r3, r6
	strb r3, [r4]
	bl sub_08024134
	ldr r0, _08016EB4 @ =0x0000080D
	add r5, r5, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r5]
	and r0, r1
	strb r0, [r5]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08016EA8: .4byte 0x020185C0
_08016EAC: .4byte 0x0201CFB0
_08016EB0: .4byte 0x00000808
_08016EB4: .4byte 0x0000080D
	thumb_func_end sub_08016E74

