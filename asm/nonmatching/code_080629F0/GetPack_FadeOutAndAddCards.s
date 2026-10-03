	thumb_func_start GetPack_FadeOutAndAddCards
GetPack_FadeOutAndAddCards: @ 0x0806360C
	push {r4, r5, r6, lr}
	bl GetPack_ScrollBg
	mov r1, #1
	neg r1, r1
	mov r0, #5
	mov r2, #0
	bl GetPack_DrawCardSprites
	mov r0, #2
	bl FadeToBlack
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0806362E
	mov r0, #0
	b _08063698
_0806362E:
	ldr r0, _08063648 @ =0x02015160
	mov r1, #0x81
	lsl r1, r1, #1
	add r5, r0, r1
	ldr r6, _0806364C @ =0x0000FFFF
	mov r4, #4
_0806363A:
	ldrh r1, [r5]
	add r2, r1, #0
	cmp r1, r6
	bne _08063650
	mov r0, #0
	b _08063686
	.align 2, 0
_08063648: .4byte 0x02015160
_0806364C: .4byte 0x0000FFFF
_08063650:
	ldr r0, _08063668 @ =0x000007CF
	cmp r1, r0
	bhi _08063674
	ldr r2, _0806366C @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _08063670 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _08063686
	.align 2, 0
_08063668: .4byte 0x000007CF
_0806366C: .4byte 0x000007FF
_08063670: .4byte gCardNumberToId
_08063674:
	ldr r1, _080636A0 @ =0xFFFFF830
	add r0, r2, r1
	ldr r1, _080636A4 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _080636A8 @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	add r0, #1
_08063686:
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl AddCardToTrunk
	add r5, #2
	sub r4, #1
	cmp r4, #0
	bge _0806363A
	mov r0, #1
_08063698:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_080636A0: .4byte 0xFFFFF830
_080636A4: .4byte 0x000007FF
_080636A8: .4byte gCardNumberToId
	thumb_func_end GetPack_FadeOutAndAddCards

