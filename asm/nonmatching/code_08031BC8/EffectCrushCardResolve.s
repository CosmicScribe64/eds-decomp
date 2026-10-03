	thumb_func_start EffectCrushCardResolve
EffectCrushCardResolve: @ 0x08032390
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	ldrb r6, [r0, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	mov r3, #1
	sub r4, r3, r0
	mov r0, #4
	mov r1, r8
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _080323B4
	b _08032656
_080323B4:
	ldr r1, _0803245C @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r1, r2
	ldrb r0, [r0]
	cmp r0, #0x7F
	bne _080323C4
	b _0803253C
_080323C4:
	cmp r0, #0x80
	beq _080323CA
	b _08032644
_080323CA:
	mov r7, #0
	mov r0, #1
	mov sl, r0
	mov r1, #0
	mov r9, r1
_080323D4:
	add r1, r4, #0
	mov r2, sl
	and r1, r2
	mov r0, #0x94
	add r2, r7, #0
	mul r2, r0
	ldr r0, _08032460 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _08032464 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	cmp r6, #0
	bne _080323F6
	b _08032518
_080323F6:
	ldrb r2, [r2, #6]
	lsl r0, r2, #0x1E
	lsr r5, r0, #0x1F
	mov r0, sl
	mov r1, r8
	ldrb r1, [r1, #2]
	and r0, r1
	mov r3, #8
	cmp r0, #0
	beq _0803240C
	ldr r3, _08032468 @ =0x00008008
_0803240C:
	lsl r1, r4, #0x10
	lsl r2, r7, #0x18
	lsr r2, r2, #0x10
	add r0, r3, #0
	lsr r1, r1, #0x10
	mov r3, #0
	bl DuelCmd_Push
	cmp r5, #0
	bne _080324F2
	mov r0, #0x7F
	cmp r4, #0
	beq _08032428
	ldr r0, _0803246C @ =0x0000807F
_08032428:
	mov r2, r9
	lsr r1, r2, #0x10
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r0, _08032470 @ =0x000007FF
	add r1, r0, #0
	add r0, r6, #0
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08032474 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	mov r5, r9
	cmp r0, #0x15
	blt _08032482
	cmp r0, #0x17
	ble _08032478
	cmp r0, #0x18
	beq _0803247C
	b _08032482
_0803245C: .4byte 0x02017A40
_08032460: .4byte 0x00000D64
_08032464: .4byte 0x0201930C
_08032468: .4byte 0x00008008
_0803246C: .4byte 0x0000807F
_08032470: .4byte 0x000007FF
_08032474: .4byte gCardStats
_08032478:
	mov r1, #0
	b _0803249C
_0803247C:
	mov r1, #0xFA
	lsl r1, r1, #4
	b _0803249C
_08032482:
	ldr r2, _080324C0 @ =0x000007FF
	add r1, r2, #0
	add r0, r6, #0
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _080324C4 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r1, r0, #1
_0803249C:
	ldr r0, _080324C8 @ =0x000005DB
	cmp r1, r0
	bhi _080324D0
	add r0, r4, #0
	add r1, r6, #0
	bl ShowRevealedCard
	mov r0, #0x7F
	cmp r4, #0
	beq _080324B2
	ldr r0, _080324CC @ =0x0000807F
_080324B2:
	lsr r1, r5, #0x10
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	b _08032518
	.align 2, 0
_080324C0: .4byte 0x000007FF
_080324C4: .4byte gCardStats
_080324C8: .4byte 0x000005DB
_080324CC: .4byte 0x0000807F
_080324D0:
	add r0, r4, #0
	add r1, r6, #0
	bl ShowDestroyedCard
	add r0, r4, #0
	add r1, r7, #0
	bl DestroyFieldCardByEffect
	mov r2, r8
	ldrb r2, [r2, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	add r1, r4, #0
	add r2, r7, #0
	bl OnCardDestroyedByEffect
	b _08032518
_080324F2:
	add r0, r4, #0
	add r1, r7, #0
	bl GetZoneCardAtk
	ldr r1, _08032530 @ =0x000005DB
	cmp r0, r1
	ble _08032518
	add r0, r4, #0
	add r1, r7, #0
	bl DestroyFieldCardByEffect
	mov r1, r8
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r4, #0
	add r2, r7, #0
	bl OnCardDestroyedByEffect
_08032518:
	mov r2, #0x80
	lsl r2, r2, #9
	add r9, r2
	add r7, #1
	cmp r7, #4
	bgt _08032526
	b _080323D4
_08032526:
	ldr r0, _08032534 @ =0x02017A40
	ldr r7, _08032538 @ =0x000003E1
	add r0, r0, r7
	mov r1, #0
	b _08032632
_08032530: .4byte 0x000005DB
_08032534: .4byte 0x02017A40
_08032538: .4byte 0x000003E1
_0803253C:
	ldr r0, _080325AC @ =0x000003E1
	add r2, r1, r0
	ldr r5, _080325B0 @ =0x020192E4
	add r0, r4, #0
	and r0, r3
	ldr r1, _080325B4 @ =0x00000D64
	mul r1, r0
	add r0, r1, r5
	ldrb r7, [r2]
	ldrb r0, [r0, #2]
	cmp r7, r0
	bcs _08032640
	ldrb r0, [r2]
	add r2, r0, #0
	lsl r0, r0, #2
	add r0, r0, r1
	ldr r7, _080325B8 @ =0x00000684
	add r1, r5, r7
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	add r0, r3, #0
	and r0, r6
	mov r3, #8
	cmp r0, #0
	beq _08032574
	ldr r3, _080325BC @ =0x00008008
_08032574:
	lsl r1, r4, #0x10
	lsr r1, r1, #0x10
	lsl r2, r2, #8
	mov r0, #0xB
	orr r2, r0
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r0, _080325C0 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #2
	ldr r1, _080325C4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08032620
	cmp r0, #0x15
	blt _080325D2
	cmp r0, #0x17
	ble _080325C8
	cmp r0, #0x18
	beq _080325CC
	b _080325D2
_080325AC: .4byte 0x000003E1
_080325B0: .4byte 0x020192E4
_080325B4: .4byte 0x00000D64
_080325B8: .4byte 0x00000684
_080325BC: .4byte 0x00008008
_080325C0: .4byte 0x000007FF
_080325C4: .4byte gCardStats
_080325C8:
	mov r1, #0
	b _080325E8
_080325CC:
	mov r1, #0xFA
	lsl r1, r1, #4
	b _080325E8
_080325D2:
	ldr r0, _0803260C @ =0x000007FF
	and r0, r5
	lsl r0, r0, #2
	ldr r2, _08032610 @ =0x08621DE0
	add r0, r0, r2
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r1, r0, #1
_080325E8:
	ldr r0, _08032614 @ =0x000005DB
	cmp r1, r0
	bls _08032620
	add r0, r4, #0
	add r1, r5, #0
	bl ShowDestroyedCard
	ldr r0, _08032618 @ =0x02017A40
	ldr r7, _0803261C @ =0x000003E1
	add r0, r0, r7
	ldrb r1, [r0]
	add r0, r4, #0
	mov r2, #1
	mov r3, #1
	bl DiscardHandCard
	mov r0, #0x7F
	b _08032658
_0803260C: .4byte 0x000007FF
_08032610: .4byte gCardStats
_08032614: .4byte 0x000005DB
_08032618: .4byte 0x02017A40
_0803261C: .4byte 0x000003E1
_08032620:
	add r0, r4, #0
	add r1, r5, #0
	bl ShowRevealedCard
	ldr r0, _08032638 @ =0x02017A40
	ldr r1, _0803263C @ =0x000003E1
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
_08032632:
	strb r1, [r0]
	mov r0, #0x7F
	b _08032658
_08032638: .4byte 0x02017A40
_0803263C: .4byte 0x000003E1
_08032640:
	mov r0, #0x7E
	b _08032658
_08032644:
	mov r0, #0x69
	cmp r4, #0
	beq _0803264C
	ldr r0, _08032668 @ =0x00008069
_0803264C:
	mov r1, #3
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08032656:
	mov r0, #0
_08032658:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08032668: .4byte 0x00008069
	thumb_func_end EffectCrushCardResolve

