	thumb_func_start DeckEdit_GetSelectedCardCopies
DeckEdit_GetSelectedCardCopies: @ 0x08068434
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r4, _08068454 @ =0x0201DB20
	ldr r0, _08068458 @ =0x00001C1C
	add r5, r4, r0
	ldrb r0, [r5]
	cmp r0, #1
	beq _080684B4
	cmp r0, #1
	bgt _0806845C
	cmp r0, #0
	beq _08068464
	b _08068658
_08068454: .4byte 0x0201DB20
_08068458: .4byte 0x00001C1C
_0806845C:
	cmp r0, #2
	bne _08068462
	b _08068604
_08068462:
	b _08068658
_08068464:
	mov r1, #0xA5
	lsl r1, r1, #5
	add r0, r4, r1
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	ldr r2, _080684A8 @ =0x00001494
	add r1, r4, r2
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0
	bne _08068480
	b _08068658
_08068480:
	ldr r2, _080684AC @ =0x02011C20
	mov r7, #0xC4
	lsl r7, r7, #3
	add r0, r4, r7
	ldrh r0, [r0]
	lsl r0, r0, #1
	mov r1, #0xE5
	lsl r1, r1, #3
	mul r1, r3
	add r0, r0, r1
	ldr r3, _080684B0 @ =0x00000644
	add r1, r4, r3
	add r0, r0, r1
	ldrh r0, [r0]
	lsl r0, r0, #2
	add r0, r0, r2
	ldrh r0, [r0, #8]
	lsl r0, r0, #0x16
	lsr r0, r0, #0x16
	b _0806865A
_080684A8: .4byte 0x00001494
_080684AC: .4byte 0x02011C20
_080684B0: .4byte 0x00000644
_080684B4:
	ldr r7, _0806851C @ =0x000014A1
	add r1, r4, r7
	ldr r2, _08068520 @ =0x00000622
	add r0, r4, r2
	ldrh r0, [r0]
	lsl r0, r0, #1
	mov r6, #0xE5
	lsl r6, r6, #3
	ldrb r1, [r1]
	mul r1, r6
	add r0, r0, r1
	ldr r3, _08068524 @ =0x00000CAE
	add r3, r3, r4
	mov r9, r3
	add r0, r9
	ldrh r0, [r0]
	bl DeckEdit_IsFusionMonster
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08068574
	ldrb r2, [r5]
	lsl r5, r2, #1
	sub r7, #1
	add r0, r4, r7
	add r0, r2, r0
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r0, r5, r0
	sub r7, #0xC
	add r1, r4, r7
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0
	bne _08068500
	b _08068658
_08068500:
	ldr r0, _08068528 @ =0x02011C20
	mov r8, r0
	mov r1, #0xC4
	lsl r1, r1, #3
	add r0, r4, r1
	add r0, r5, r0
	ldrh r0, [r0]
	cmp r2, #1
	beq _08068544
	cmp r2, #1
	bgt _0806852C
	cmp r2, #0
	beq _08068532
	b _08068562
_0806851C: .4byte 0x000014A1
_08068520: .4byte 0x00000622
_08068524: .4byte 0x00000CAE
_08068528: .4byte 0x02011C20
_0806852C:
	cmp r2, #2
	beq _08068550
	b _08068562
_08068532:
	lsl r0, r0, #1
	add r1, r3, #0
	mul r1, r6
	add r0, r0, r1
	ldr r2, _08068540 @ =0x00000644
	add r1, r4, r2
	b _0806855C
_08068540: .4byte 0x00000644
_08068544:
	lsl r0, r0, #1
	add r1, r3, #0
	mul r1, r6
	add r0, r0, r1
	add r0, r9
	b _0806855E
_08068550:
	lsl r0, r0, #1
	add r1, r3, #0
	mul r1, r6
	add r0, r0, r1
	ldr r3, _08068570 @ =0x00000D4E
	add r1, r4, r3
_0806855C:
	add r0, r0, r1
_0806855E:
	ldrh r0, [r0]
	mov sl, r0
_08068562:
	mov r7, sl
	lsl r0, r7, #0x10
	lsr r0, r0, #0xE
	add r0, r8
	ldrb r0, [r0, #9]
	lsr r0, r0, #6
	b _0806865A
_08068570: .4byte 0x00000D4E
_08068574:
	ldrb r2, [r5]
	lsl r5, r2, #1
	mov r1, #0xA5
	lsl r1, r1, #5
	add r0, r4, r1
	add r0, r2, r0
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r0, r5, r0
	ldr r7, _080685B0 @ =0x00001494
	add r1, r4, r7
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #0
	beq _08068658
	ldr r7, _080685B4 @ =0x02011C20
	mov r1, #0xC4
	lsl r1, r1, #3
	add r0, r4, r1
	add r0, r5, r0
	ldrh r0, [r0]
	cmp r2, #1
	beq _080685D0
	cmp r2, #1
	bgt _080685B8
	cmp r2, #0
	beq _080685BE
	b _080685EE
_080685B0: .4byte 0x00001494
_080685B4: .4byte 0x02011C20
_080685B8:
	cmp r2, #2
	beq _080685DC
	b _080685EE
_080685BE:
	lsl r0, r0, #1
	add r1, r3, #0
	mul r1, r6
	add r0, r0, r1
	ldr r2, _080685CC @ =0x00000644
	add r1, r4, r2
	b _080685E8
_080685CC: .4byte 0x00000644
_080685D0:
	lsl r0, r0, #1
	add r1, r3, #0
	mul r1, r6
	add r0, r0, r1
	add r0, r9
	b _080685EA
_080685DC:
	lsl r0, r0, #1
	add r1, r3, #0
	mul r1, r6
	add r0, r0, r1
	ldr r3, _08068600 @ =0x00000D4E
	add r1, r4, r3
_080685E8:
	add r0, r0, r1
_080685EA:
	ldrh r0, [r0]
	mov r8, r0
_080685EE:
	mov r1, r8
	lsl r0, r1, #0x10
	lsr r0, r0, #0xE
	add r0, r7, r0
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1C
	lsr r0, r0, #0x1E
	b _0806865A
	.align 2, 0
_08068600: .4byte 0x00000D4E
_08068604:
	ldr r2, _08068644 @ =0x000014A2
	add r0, r4, r2
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r0, r4, r0
	ldr r7, _08068648 @ =0x00001498
	add r0, r0, r7
	ldrh r0, [r0]
	cmp r0, #0
	beq _08068658
	ldr r2, _0806864C @ =0x02011C20
	ldr r1, _08068650 @ =0x00000624
	add r0, r4, r1
	ldrh r0, [r0]
	lsl r0, r0, #1
	mov r1, #0xE5
	lsl r1, r1, #3
	mul r1, r3
	add r0, r0, r1
	ldr r3, _08068654 @ =0x00000D4E
	add r1, r4, r3
	add r0, r0, r1
	ldrh r0, [r0]
	lsl r0, r0, #2
	add r0, r0, r2
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1E
	b _0806865A
	.align 2, 0
_08068644: .4byte 0x000014A2
_08068648: .4byte 0x00001498
_0806864C: .4byte 0x02011C20
_08068650: .4byte 0x00000624
_08068654: .4byte 0x00000D4E
_08068658:
	mov r0, #0
_0806865A:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end DeckEdit_GetSelectedCardCopies

