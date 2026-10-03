	thumb_func_start DeckStats_CountCategory
DeckStats_CountCategory: @ 0x0806C590
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov r8, r0
	lsl r1, r1, #0x18
	lsr r2, r1, #0x18
	mov r0, #0
	str r0, [sp, #0]
	ldr r1, _0806C5C8 @ =0x0201DB20
	mov r3, #0xA5
	lsl r3, r3, #5
	add r0, r1, r3
	add r0, r8
	ldrb r7, [r0]
	add r3, r1, #0
	mov r4, r8
	cmp r4, #1
	beq _0806C5E4
	cmp r4, #1
	bgt _0806C5CC
	cmp r4, #0
	beq _0806C5D4
	b _0806C604
_0806C5C8: .4byte 0x0201DB20
_0806C5CC:
	mov r0, r8
	cmp r0, #2
	beq _0806C5F4
	b _0806C604
_0806C5D4:
	mov r0, #0xE5
	lsl r0, r0, #3
	add r1, r7, #0
	mul r1, r0
	ldr r4, _0806C5E0 @ =0x00000644
	b _0806C5FE
_0806C5E0: .4byte 0x00000644
_0806C5E4:
	mov r0, #0xE5
	lsl r0, r0, #3
	add r1, r7, #0
	mul r1, r0
	ldr r4, _0806C5F0 @ =0x00000CAE
	b _0806C5FE
_0806C5F0: .4byte 0x00000CAE
_0806C5F4:
	mov r0, #0xE5
	lsl r0, r0, #3
	add r1, r7, #0
	mul r1, r0
	ldr r4, _0806C618 @ =0x00000D4E
_0806C5FE:
	add r0, r3, r4
	add r1, r1, r0
	mov r9, r1
_0806C604:
	sub r0, r2, #1
	cmp r0, #5
	bls _0806C60C
	b _0806CB4A
_0806C60C:
	lsl r0, r0, #2
	ldr r1, _0806C61C @ =0x0806C620
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0806C618: .4byte 0x00000D4E
_0806C61C: .4byte 0x0806C620
_0806C620:
	.4byte _0806C638
	.4byte _0806C74C
	.4byte _0806C860
	.4byte _0806C974
	.4byte _0806C9DC
	.4byte _0806CA44
_0806C638:
	mov r5, #0
	mov r1, r8
	lsl r0, r1, #1
	lsl r2, r7, #1
	add r1, r2, r7
	lsl r1, r1, #1
	add r1, r0, r1
	ldr r4, _0806C694 @ =0x00001494
	add r3, r3, r4
	add r1, r1, r3
	str r0, [sp, #4]
	mov sl, r2
	ldrh r1, [r1]
	cmp r5, r1
	bcc _0806C658
	b _0806CB4A
_0806C658:
	lsl r2, r5, #1
	mov r1, r9
	add r0, r2, r1
	ldrh r4, [r0]
	add r3, r4, #0
	ldr r0, _0806C698 @ =0x000007FF
	and r3, r0
	lsl r0, r3, #2
	ldr r1, _0806C69C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	add r6, r2, #0
	cmp r0, #0x16
	bgt _0806C680
	cmp r0, #0x15
	bge _0806C724
_0806C680:
	add r2, r4, #0
	lsl r0, r3, #1
	ldr r3, _0806C6A0 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _0806C6A4 @ =0x00000776
	cmp r1, r0
	bne _0806C6A8
	mov r0, #3
	b _0806C70A
_0806C694: .4byte 0x00001494
_0806C698: .4byte 0x000007FF
_0806C69C: .4byte gCardStats
_0806C6A0: .4byte gCardIdToNumber
_0806C6A4: .4byte 0x00000776
_0806C6A8:
	cmp r1, r0
	blt _0806C6B8
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0806C6B8
	mov r0, #1
	b _0806C70A
_0806C6B8:
	add r0, r2, #0
	ldr r4, _0806C6DC @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0806C6E0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0806C6EA
	cmp r0, #0x16
	bgt _0806C6E4
	cmp r0, #0x15
	beq _0806C6EE
	b _0806C6F6
_0806C6DC: .4byte 0x000007FF
_0806C6E0: .4byte gCardStats
_0806C6E4:
	cmp r0, #0x17
	beq _0806C6F2
	b _0806C6F6
_0806C6EA:
	mov r0, #7
	b _0806C70A
_0806C6EE:
	mov r0, #8
	b _0806C70A
_0806C6F2:
	mov r0, #9
	b _0806C70A
_0806C6F6:
	ldr r3, _0806C740 @ =0x000007FF
	and r2, r3
	lsl r0, r2, #2
	ldr r4, _0806C744 @ =0x08621DE0
	add r0, r0, r4
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0806C70A:
	cmp r0, #0
	bne _0806C724
	mov r1, r9
	add r0, r6, r1
	ldrh r1, [r0]
	mov r0, r8
	bl GetCardCopiesInList
	ldr r2, [sp, #0]
	add r0, r2, r0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0]
_0806C724:
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	mov r3, sl
	add r0, r3, r7
	lsl r0, r0, #1
	ldr r4, [sp, #4]
	add r0, r4, r0
	ldr r1, _0806C748 @ =0x0201EFB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r5, r0
	bcc _0806C658
	b _0806CB4A
_0806C740: .4byte 0x000007FF
_0806C744: .4byte gCardStats
_0806C748: .4byte 0x0201EFB4
_0806C74C:
	mov r5, #0
	mov r2, r8
	lsl r0, r2, #1
	lsl r2, r7, #1
	add r1, r2, r7
	lsl r1, r1, #1
	add r1, r0, r1
	ldr r4, _0806C7A8 @ =0x00001494
	add r3, r3, r4
	add r1, r1, r3
	str r0, [sp, #4]
	mov sl, r2
	ldrh r1, [r1]
	cmp r5, r1
	bcc _0806C76C
	b _0806CB4A
_0806C76C:
	lsl r2, r5, #1
	mov r1, r9
	add r0, r2, r1
	ldrh r4, [r0]
	add r3, r4, #0
	ldr r0, _0806C7AC @ =0x000007FF
	and r3, r0
	lsl r0, r3, #2
	ldr r1, _0806C7B0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	add r6, r2, #0
	cmp r0, #0x16
	bgt _0806C794
	cmp r0, #0x15
	bge _0806C838
_0806C794:
	add r2, r4, #0
	lsl r0, r3, #1
	ldr r3, _0806C7B4 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _0806C7B8 @ =0x00000776
	cmp r1, r0
	bne _0806C7BC
	mov r0, #3
	b _0806C81E
_0806C7A8: .4byte 0x00001494
_0806C7AC: .4byte 0x000007FF
_0806C7B0: .4byte gCardStats
_0806C7B4: .4byte gCardIdToNumber
_0806C7B8: .4byte 0x00000776
_0806C7BC:
	cmp r1, r0
	blt _0806C7CC
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0806C7CC
	mov r0, #1
	b _0806C81E
_0806C7CC:
	add r0, r2, #0
	ldr r4, _0806C7F0 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0806C7F4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0806C7FE
	cmp r0, #0x16
	bgt _0806C7F8
	cmp r0, #0x15
	beq _0806C802
	b _0806C80A
_0806C7F0: .4byte 0x000007FF
_0806C7F4: .4byte gCardStats
_0806C7F8:
	cmp r0, #0x17
	beq _0806C806
	b _0806C80A
_0806C7FE:
	mov r0, #7
	b _0806C81E
_0806C802:
	mov r0, #8
	b _0806C81E
_0806C806:
	mov r0, #9
	b _0806C81E
_0806C80A:
	ldr r3, _0806C854 @ =0x000007FF
	and r2, r3
	lsl r0, r2, #2
	ldr r4, _0806C858 @ =0x08621DE0
	add r0, r0, r4
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0806C81E:
	cmp r0, #1
	bne _0806C838
	mov r1, r9
	add r0, r6, r1
	ldrh r1, [r0]
	mov r0, r8
	bl GetCardCopiesInList
	ldr r2, [sp, #0]
	add r0, r2, r0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0]
_0806C838:
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	mov r3, sl
	add r0, r3, r7
	lsl r0, r0, #1
	ldr r4, [sp, #4]
	add r0, r4, r0
	ldr r1, _0806C85C @ =0x0201EFB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r5, r0
	bcc _0806C76C
	b _0806CB4A
_0806C854: .4byte 0x000007FF
_0806C858: .4byte gCardStats
_0806C85C: .4byte 0x0201EFB4
_0806C860:
	mov r5, #0
	mov r2, r8
	lsl r0, r2, #1
	lsl r2, r7, #1
	add r1, r2, r7
	lsl r1, r1, #1
	add r1, r0, r1
	ldr r4, _0806C8BC @ =0x00001494
	add r3, r3, r4
	add r1, r1, r3
	str r0, [sp, #4]
	mov sl, r2
	ldrh r1, [r1]
	cmp r5, r1
	bcc _0806C880
	b _0806CB4A
_0806C880:
	lsl r2, r5, #1
	mov r1, r9
	add r0, r2, r1
	ldrh r4, [r0]
	add r3, r4, #0
	ldr r0, _0806C8C0 @ =0x000007FF
	and r3, r0
	lsl r0, r3, #2
	ldr r1, _0806C8C4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	add r6, r2, #0
	cmp r0, #0x16
	bgt _0806C8A8
	cmp r0, #0x15
	bge _0806C94C
_0806C8A8:
	add r2, r4, #0
	lsl r0, r3, #1
	ldr r3, _0806C8C8 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _0806C8CC @ =0x00000776
	cmp r1, r0
	bne _0806C8D0
	mov r0, #3
	b _0806C932
_0806C8BC: .4byte 0x00001494
_0806C8C0: .4byte 0x000007FF
_0806C8C4: .4byte gCardStats
_0806C8C8: .4byte gCardIdToNumber
_0806C8CC: .4byte 0x00000776
_0806C8D0:
	cmp r1, r0
	blt _0806C8E0
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0806C8E0
	mov r0, #1
	b _0806C932
_0806C8E0:
	add r0, r2, #0
	ldr r4, _0806C904 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0806C908 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0806C912
	cmp r0, #0x16
	bgt _0806C90C
	cmp r0, #0x15
	beq _0806C916
	b _0806C91E
_0806C904: .4byte 0x000007FF
_0806C908: .4byte gCardStats
_0806C90C:
	cmp r0, #0x17
	beq _0806C91A
	b _0806C91E
_0806C912:
	mov r0, #7
	b _0806C932
_0806C916:
	mov r0, #8
	b _0806C932
_0806C91A:
	mov r0, #9
	b _0806C932
_0806C91E:
	ldr r3, _0806C968 @ =0x000007FF
	and r2, r3
	lsl r0, r2, #2
	ldr r4, _0806C96C @ =0x08621DE0
	add r0, r0, r4
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0806C932:
	cmp r0, #2
	bne _0806C94C
	mov r1, r9
	add r0, r6, r1
	ldrh r1, [r0]
	mov r0, r8
	bl GetCardCopiesInList
	ldr r2, [sp, #0]
	add r0, r2, r0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0]
_0806C94C:
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	mov r3, sl
	add r0, r3, r7
	lsl r0, r0, #1
	ldr r4, [sp, #4]
	add r0, r4, r0
	ldr r1, _0806C970 @ =0x0201EFB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r5, r0
	bcc _0806C880
	b _0806CB4A
_0806C968: .4byte 0x000007FF
_0806C96C: .4byte gCardStats
_0806C970: .4byte 0x0201EFB4
_0806C974:
	mov r5, #0
	lsl r0, r7, #1
	add r0, r0, r7
	add r0, r8
	lsl r0, r0, #1
	ldr r2, _0806C9D0 @ =0x00001494
	add r1, r3, r2
	add r0, r0, r1
	ldrh r3, [r0]
	cmp r5, r3
	bcc _0806C98C
	b _0806CB4A
_0806C98C:
	add r4, r0, #0
_0806C98E:
	lsl r0, r5, #1
	add r0, r9
	ldrh r2, [r0]
	ldr r0, _0806C9D4 @ =0x000007FF
	add r1, r0, #0
	add r0, r2, #0
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0806C9D8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0806C9C2
	mov r0, r8
	add r1, r2, #0
	bl GetCardCopiesInList
	ldr r2, [sp, #0]
	add r0, r2, r0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0]
_0806C9C2:
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	ldrh r3, [r4]
	cmp r5, r3
	bcc _0806C98E
	b _0806CB4A
_0806C9D0: .4byte 0x00001494
_0806C9D4: .4byte 0x000007FF
_0806C9D8: .4byte gCardStats
_0806C9DC:
	mov r5, #0
	lsl r0, r7, #1
	add r0, r0, r7
	add r0, r8
	lsl r0, r0, #1
	ldr r4, _0806CA38 @ =0x00001494
	add r1, r3, r4
	add r0, r0, r1
	ldrh r1, [r0]
	cmp r5, r1
	bcc _0806C9F4
	b _0806CB4A
_0806C9F4:
	add r4, r0, #0
_0806C9F6:
	lsl r0, r5, #1
	add r0, r9
	ldrh r2, [r0]
	ldr r3, _0806CA3C @ =0x000007FF
	add r1, r3, #0
	add r0, r2, #0
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0806CA40 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _0806CA2A
	mov r0, r8
	add r1, r2, #0
	bl GetCardCopiesInList
	ldr r2, [sp, #0]
	add r0, r2, r0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0]
_0806CA2A:
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	ldrh r3, [r4]
	cmp r5, r3
	bcc _0806C9F6
	b _0806CB4A
_0806CA38: .4byte 0x00001494
_0806CA3C: .4byte 0x000007FF
_0806CA40: .4byte gCardStats
_0806CA44:
	mov r5, #0
	mov r4, r8
	lsl r0, r4, #1
	lsl r2, r7, #1
	add r1, r2, r7
	lsl r1, r1, #1
	add r1, r0, r1
	ldr r4, _0806CAA0 @ =0x00001494
	add r3, r3, r4
	add r1, r1, r3
	str r0, [sp, #4]
	mov sl, r2
	ldrh r1, [r1]
	cmp r5, r1
	bcs _0806CB4A
_0806CA62:
	lsl r2, r5, #1
	mov r1, r9
	add r0, r2, r1
	ldrh r4, [r0]
	add r3, r4, #0
	ldr r0, _0806CAA4 @ =0x000007FF
	and r3, r0
	lsl r0, r3, #2
	ldr r1, _0806CAA8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	add r6, r2, #0
	cmp r0, #0x16
	bgt _0806CA8A
	cmp r0, #0x15
	bge _0806CB30
_0806CA8A:
	add r2, r4, #0
	lsl r0, r3, #1
	ldr r3, _0806CAAC @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _0806CAB0 @ =0x00000776
	cmp r1, r0
	bne _0806CAB4
	mov r0, #3
	b _0806CB16
	.align 2, 0
_0806CAA0: .4byte 0x00001494
_0806CAA4: .4byte 0x000007FF
_0806CAA8: .4byte gCardStats
_0806CAAC: .4byte gCardIdToNumber
_0806CAB0: .4byte 0x00000776
_0806CAB4:
	cmp r1, r0
	blt _0806CAC4
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0806CAC4
	mov r0, #1
	b _0806CB16
_0806CAC4:
	add r0, r2, #0
	ldr r4, _0806CAE8 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0806CAEC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0806CAF6
	cmp r0, #0x16
	bgt _0806CAF0
	cmp r0, #0x15
	beq _0806CAFA
	b _0806CB02
_0806CAE8: .4byte 0x000007FF
_0806CAEC: .4byte gCardStats
_0806CAF0:
	cmp r0, #0x17
	beq _0806CAFE
	b _0806CB02
_0806CAF6:
	mov r0, #7
	b _0806CB16
_0806CAFA:
	mov r0, #8
	b _0806CB16
_0806CAFE:
	mov r0, #9
	b _0806CB16
_0806CB02:
	ldr r3, _0806CB5C @ =0x000007FF
	and r2, r3
	lsl r0, r2, #2
	ldr r4, _0806CB60 @ =0x08621DE0
	add r0, r0, r4
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0806CB16:
	cmp r0, #3
	bne _0806CB30
	mov r1, r9
	add r0, r6, r1
	ldrh r1, [r0]
	mov r0, r8
	bl GetCardCopiesInList
	ldr r2, [sp, #0]
	add r0, r2, r0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0]
_0806CB30:
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	mov r3, sl
	add r0, r3, r7
	lsl r0, r0, #1
	ldr r4, [sp, #4]
	add r0, r4, r0
	ldr r1, _0806CB64 @ =0x0201EFB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r5, r0
	bcc _0806CA62
_0806CB4A:
	ldr r0, [sp, #0]
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0806CB5C: .4byte 0x000007FF
_0806CB60: .4byte gCardStats
_0806CB64: .4byte 0x0201EFB4
	thumb_func_end DeckStats_CountCategory

