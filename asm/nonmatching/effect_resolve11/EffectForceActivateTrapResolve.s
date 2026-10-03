	thumb_func_start EffectForceActivateTrapResolve
EffectForceActivateTrapResolve: @ 0x0803C5A0
	push {r4, r5, r6, r7, lr}
	sub sp, #0x14
	add r7, r0, #0
	ldrb r4, [r7, #0xC]
	ldrh r0, [r7, #0xC]
	lsr r5, r0, #8
	mov r1, #1
	and r1, r4
	mov r0, #0x94
	add r2, r5, #0
	mul r2, r0
	ldr r0, _0803C5E4 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0803C5E8 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	mov r0, #4
	ldrb r1, [r7, #4]
	and r0, r1
	cmp r0, #0
	beq _0803C5EC
	ldrb r2, [r7, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldrh r7, [r7, #2]
	lsl r1, r7, #0x16
	lsr r1, r1, #0x1A
	mov r2, #1
	bl DestroyFieldCard
	b _0803C828
_0803C5E4: .4byte 0x00000D64
_0803C5E8: .4byte 0x0201930C
_0803C5EC:
	cmp r6, #0
	bne _0803C5F2
	b _0803C7FE
_0803C5F2:
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _0803C5FE
	b _0803C7FE
_0803C5FE:
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #0
	bl FlipFieldCard
	ldr r2, _0803C634 @ =0x000007FF
	and r2, r6
	lsl r0, r2, #2
	ldr r1, _0803C638 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _0803C63C
	add r0, r4, #0
	add r1, r6, #0
	bl ShowRevealedCard
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #0
	bl FlipFieldCard
	b _0803C7FE
_0803C634: .4byte 0x000007FF
_0803C638: .4byte gCardStats
_0803C63C:
	lsl r0, r2, #1
	ldr r2, _0803C67C @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _0803C680 @ =0x00000426
	cmp r1, r0
	bne _0803C64C
	b _0803C7EC
_0803C64C:
	cmp r1, r0
	bgt _0803C6EE
	sub r0, #0x48
	cmp r1, r0
	bne _0803C658
	b _0803C7EC
_0803C658:
	cmp r1, r0
	bgt _0803C6AA
	ldr r0, _0803C684 @ =0x000002B1
	cmp r1, r0
	bne _0803C664
	b _0803C7EC
_0803C664:
	cmp r1, r0
	bgt _0803C688
	sub r0, #9
	cmp r1, r0
	bge _0803C670
	b _0803C77C
_0803C670:
	add r0, #1
	cmp r1, r0
	bgt _0803C678
	b _0803C7EC
_0803C678:
	b _0803C776
	.align 2, 0
_0803C67C: .4byte gCardIdToNumber
_0803C680: .4byte 0x00000426
_0803C684: .4byte 0x000002B1
_0803C688:
	ldr r0, _0803C698 @ =0x000003B6
	cmp r1, r0
	bne _0803C690
	b _0803C7EC
_0803C690:
	cmp r1, r0
	bgt _0803C69C
	sub r0, #5
	b _0803C778
_0803C698: .4byte 0x000003B6
_0803C69C:
	mov r0, #0xF0
	lsl r0, r0, #2
	cmp r1, r0
	bne _0803C6A6
	b _0803C7EC
_0803C6A6:
	add r0, #9
	b _0803C778
_0803C6AA:
	ldr r0, _0803C6C4 @ =0x000003FE
	cmp r1, r0
	bgt _0803C6C8
	sub r0, #1
	cmp r1, r0
	blt _0803C6B8
	b _0803C7EC
_0803C6B8:
	sub r0, #0x13
	cmp r1, r0
	bne _0803C6C0
	b _0803C7EC
_0803C6C0:
	add r0, #0x11
	b _0803C778
_0803C6C4: .4byte 0x000003FE
_0803C6C8:
	ldr r0, _0803C6E4 @ =0x0000040E
	cmp r1, r0
	bne _0803C6D0
	b _0803C7EC
_0803C6D0:
	cmp r1, r0
	bgt _0803C6E8
	sub r0, #7
	cmp r1, r0
	bgt _0803C77C
	sub r0, #3
	cmp r1, r0
	blt _0803C77C
	b _0803C7EC
	.align 2, 0
_0803C6E4: .4byte 0x0000040E
_0803C6E8:
	mov r0, #0x84
	lsl r0, r0, #3
	b _0803C778
_0803C6EE:
	mov r0, #0xA3
	lsl r0, r0, #3
	cmp r1, r0
	bne _0803C6F8
	b _0803C7EC
_0803C6F8:
	cmp r1, r0
	bgt _0803C734
	sub r0, #0xA2
	cmp r1, r0
	bgt _0803C712
	sub r0, #8
	cmp r1, r0
	bge _0803C7EC
	sub r0, #0x3D
	cmp r1, r0
	beq _0803C7EC
	add r0, #0x19
	b _0803C778
_0803C712:
	ldr r0, _0803C720 @ =0x000004BE
	cmp r1, r0
	beq _0803C7EC
	cmp r1, r0
	bgt _0803C724
	sub r0, #0x41
	b _0803C778
_0803C720: .4byte 0x000004BE
_0803C724:
	ldr r0, _0803C730 @ =0x000004DF
	cmp r1, r0
	beq _0803C7EC
	add r0, #0x36
	b _0803C778
	.align 2, 0
_0803C730: .4byte 0x000004DF
_0803C734:
	ldr r0, _0803C750 @ =0x00000591
	cmp r1, r0
	bgt _0803C75C
	sub r0, #1
	cmp r1, r0
	bge _0803C7EC
	sub r0, #0x67
	cmp r1, r0
	beq _0803C7EC
	cmp r1, r0
	bgt _0803C754
	sub r0, #4
	b _0803C778
	.align 2, 0
_0803C750: .4byte 0x00000591
_0803C754:
	ldr r0, _0803C758 @ =0x0000052D
	b _0803C778
_0803C758: .4byte 0x0000052D
_0803C75C:
	ldr r0, _0803C76C @ =0x000005F9
	cmp r1, r0
	beq _0803C7EC
	cmp r1, r0
	bgt _0803C770
	sub r0, #0x65
	b _0803C778
	.align 2, 0
_0803C76C: .4byte 0x000005F9
_0803C770:
	ldr r0, _0803C7E4 @ =0x000005FB
	cmp r1, r0
	beq _0803C7EC
_0803C776:
	add r0, #4
_0803C778:
	cmp r1, r0
	beq _0803C7EC
_0803C77C:
	mov r0, sp
	strh r6, [r0]
	mov r3, sp
	mov r0, #1
	add r1, r4, #0
	and r1, r0
	ldrb r2, [r3, #2]
	mov r0, #2
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3, #2]
	mov r0, #0x3F
	add r1, r5, #0
	and r1, r0
	lsl r1, r1, #4
	ldrh r2, [r3, #2]
	ldr r0, _0803C7E8 @ =0xFFFFFC0F
	and r0, r2
	orr r0, r1
	strh r0, [r3, #2]
	mov r2, sp
	ldrb r1, [r2, #3]
	mov r0, #3
	and r0, r1
	strb r0, [r2, #3]
	mov r0, sp
	mov r1, #0
	mov r2, #0
	bl CanActivateEffect
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803C7EC
	add r0, r4, #0
	add r1, r6, #0
	bl sub_080197C0
	lsl r0, r4, #0x1F
	mov r1, #0x1F
	and r5, r1
	lsl r1, r5, #0x10
	mov r2, #0x80
	lsl r2, r2, #0xE
	orr r1, r2
	orr r0, r1
	orr r0, r6
	mov r1, #0
	bl Chain_AddPending
	b _0803C7FE
	.align 2, 0
_0803C7E4: .4byte 0x000005FB
_0803C7E8: .4byte 0xFFFFFC0F
_0803C7EC:
	add r0, r4, #0
	add r1, r6, #0
	bl ShowDestroyedCard
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #1
	bl DestroyFieldCard
_0803C7FE:
	ldrb r1, [r7, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldrh r2, [r7, #2]
	lsl r1, r2, #0x16
	lsr r1, r1, #0x1A
	bl ReturnFieldCardToDeck
	mov r0, #1
	ldrb r7, [r7, #2]
	and r0, r7
	mov r1, #0x60
	cmp r0, #0
	beq _0803C81C
	ldr r1, _0803C834 @ =0x00008060
_0803C81C:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_0803C828:
	mov r0, #0
	add sp, #0x14
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0803C834: .4byte 0x00008060
	thumb_func_end EffectForceActivateTrapResolve

