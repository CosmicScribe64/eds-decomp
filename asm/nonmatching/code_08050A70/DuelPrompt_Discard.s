	thumb_func_start DuelPrompt_Discard
DuelPrompt_Discard: @ 0x080517BC
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r6, r0, #0
	add r7, r1, #0
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r9, r3
	ldr r0, _080517E8 @ =0x020192E0
	mov r8, r0
	ldr r5, _080517EC @ =0x00001B62
	add r5, r8
	ldrb r4, [r5]
	cmp r4, #1
	beq _08051824
	cmp r4, #1
	bgt _080517F0
	cmp r4, #0
	beq _080517FE
	b _0805193C
	.align 2, 0
_080517E8: .4byte 0x020192E0
_080517EC: .4byte 0x00001B62
_080517F0:
	cmp r4, #2
	bne _080517F6
	b _0805190C
_080517F6:
	cmp r4, #3
	bne _080517FC
	b _08051916
_080517FC:
	b _0805193C
_080517FE:
	add r0, r6, #0
	mov r1, #0xB
	bl DuelScreen_ScrollToZone
	ldr r0, _08051818 @ =0x02017A40
	ldr r2, _0805181C @ =0x000004FC
	add r1, r0, r2
	strb r4, [r1]
	ldr r1, _08051820 @ =0x000004FD
	add r0, r0, r1
	strb r7, [r0]
	b _0805192E
	.align 2, 0
_08051818: .4byte 0x02017A40
_0805181C: .4byte 0x000004FC
_08051820: .4byte 0x000004FD
_08051824:
	cmp r6, #0
	beq _080518C0
	ldr r0, _08051890 @ =0x02017A40
	ldr r2, _08051894 @ =0x000004FD
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0
	beq _080518E0
	mov r5, r8
	add r5, #4
	and r4, r6
	ldr r0, _08051898 @ =0x00000D64
	mul r0, r4
	add r0, r0, r5
	ldrb r0, [r0, #2]
	cmp r0, #0
	beq _080518E0
	bl AiPickDiscard
	add r1, r0, #0
	cmp r1, #0
	bge _080518A2
	add r0, r5, #0
	mov r1, #1
	bl AiPickWeakestHandCard
	add r1, r0, #0
	cmp r1, #0
	bge _080518A2
	mov r0, #1
	bl FindMagicInHand
	add r1, r0, #0
	cmp r1, #0
	bge _080518A2
	mov r0, #1
	bl FindTrapInHand
	add r1, r0, #0
	cmp r1, #0
	bge _080518A2
	ldr r4, _0805189C @ =0x00000D6A
	add r4, r8
	ldrb r0, [r4]
	cmp r0, #2
	bls _080518A0
	bl Random
	ldrb r1, [r4]
	bl __modsi3
	add r1, r0, #0
	b _080518A2
	.align 2, 0
_08051890: .4byte 0x02017A40
_08051894: .4byte 0x000004FD
_08051898: .4byte 0x00000D64
_0805189C: .4byte 0x00000D6A
_080518A0:
	mov r1, #0
_080518A2:
	add r0, r6, #0
	mov r2, r9
	mov r3, #1
	bl DiscardHandCard
	ldr r0, _080518B8 @ =0x02017A40
	ldr r1, _080518BC @ =0x000004FD
	add r0, r0, r1
	ldrb r1, [r0]
	sub r1, #1
	b _080518EA
_080518B8: .4byte 0x02017A40
_080518BC: .4byte 0x000004FD
_080518C0:
	mov r0, #0
	mov r1, #0xB
	mov r2, #0
	bl DuelCursor_Select
	ldr r0, _080518F0 @ =0x00000209
	ldr r1, _080518F4 @ =0x0000050E
	ldr r3, _080518F8 @ =0x08085FDC
	mov r2, #0xB
	bl TextBoxOpen
	ldr r1, _080518FC @ =0x080516D9
	ldr r2, _08051900 @ =0x08051731
	mov r0, #5
	bl TextBoxSetMenu
_080518E0:
	ldr r0, _08051904 @ =0x020192E0
	ldr r2, _08051908 @ =0x00001B62
	add r0, r0, r2
	ldrb r1, [r0]
	add r1, #1
_080518EA:
	strb r1, [r0]
	mov r0, #0
	b _0805193E
_080518F0: .4byte 0x00000209
_080518F4: .4byte 0x0000050E
_080518F8: .4byte gStrDiscardFromHand
_080518FC: .4byte DiscardPrompt_DrawRemaining
_08051900: .4byte DiscardPrompt_HandleInput
_08051904: .4byte 0x020192E0
_08051908: .4byte 0x00001B62
_0805190C:
	add r0, r6, #0
	add r1, r7, #0
	bl TriggerForcedRequisition
	b _0805192E
_08051916:
	ldr r0, _08051938 @ =0x00001B12
	add r0, r8
	ldrb r0, [r0]
	lsl r1, r0, #0x1E
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	lsl r2, r6, #0x18
	lsr r2, r2, #0x18
	mov r1, #0x1D
	bl EventResponse_Request
_0805192E:
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
	b _0805193E
_08051938: .4byte 0x00001B12
_0805193C:
	mov r0, #1
_0805193E:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end DuelPrompt_Discard
	.align 2, 0

